#include <bits/stdc++.h>
using namespace std;
#include "optimizer.h"
#include "cost.h"

static unordered_map<string, long long> propagate_stats(const Plan& left,const Relation& right,const JoinPredicate& pred){
    unordered_map<string, long long> result;

    // copy left stats
    for (auto& [attr, v] : left.distinct)
        result[attr] = v;

    // copy right stats
    for (auto& [attr, v] : right.distinct)
        result[attr] = v;

    long long v_left = 1, v_right = 1;

    if (left.distinct.count(pred.left_attr))
        v_left = left.distinct.at(pred.left_attr);
    else if (left.distinct.count(pred.right_attr))
        v_left = left.distinct.at(pred.right_attr);

    if (right.distinct.count(pred.left_attr))
        v_right = right.distinct.at(pred.left_attr);
    else if (right.distinct.count(pred.right_attr))
        v_right = right.distinct.at(pred.right_attr);

    long long v = min(v_left, v_right);

    result[pred.left_attr] = v;
    result[pred.right_attr] = v;

    return result;
}


void gen_subsets(const vector<string>& rels,int idx,int k,set<string>& curr,vector<set<string>>& result){
    if (curr.size() == k) {
        result.push_back(curr);
        return;
    }
    if (idx == rels.size()) return;

    curr.insert(rels[idx]);
    gen_subsets(rels, idx + 1, k, curr, result);
    curr.erase(rels[idx]);
    
    gen_subsets(rels, idx + 1, k, curr, result);
}

bool has_join_predicate(const set<string>& left,const string& right,const vector<JoinPredicate>& join_preds,JoinPredicate& out){
    for (auto& p : join_preds) {
        if (left.count(p.left_rel) && p.right_rel == right) {
            out = p;
            return true;
        }
        if (left.count(p.right_rel) && p.left_rel == right) {
            out = p;
            return true;
        }
    }
    return false;
}

Plan optimize_query(const map<string, Relation>& relations,const vector<JoinPredicate>& join_preds){
    map<set<string>, Plan> best;

    // ---- Base scan plans ----
    for (auto& [name, rel] : relations) {
        Plan p;
        p.relations = {name};
        p.cost = rel.num_pages;
        p.cardinality = rel.num_tuples;
        p.repr = name;
        p.distinct = rel.distinct;
        best[{name}] = p;
    }

    vector<string> rel_names;
    for (auto& [name, _] : relations)
        rel_names.push_back(name);

    // ---- Selinger DP ----
    for (int size = 2; size <= rel_names.size(); size++) {
        vector<set<string>> subsets;
        set<string> temp;
        gen_subsets(rel_names, 0, size, temp, subsets);

        for (auto& S : subsets) {
            Plan best_plan;
            best_plan.cost = 1e18;

            for (auto& r : S) {
                set<string> S_minus_r = S;
                S_minus_r.erase(r);

                if (!best.count(S_minus_r)) continue;

                JoinPredicate pred;
                if (!has_join_predicate(S_minus_r, r, join_preds, pred))
                    continue;

                const Plan& left = best[S_minus_r];
                const Relation& right = relations.at(r);

                double cost = estimate_hash_join_cost(left, right);
                long long card =
                    estimate_join_cardinality(left, right, pred);

                if (cost < best_plan.cost) {
                    best_plan.relations = S;
                    best_plan.cost = cost;
                    best_plan.cardinality = card;
                    best_plan.repr = "(" + left.repr + " ⨝ " + r + ")";
                    best_plan.distinct = propagate_stats(left, right, pred);
                }
            }

            if (best_plan.cost < 1e18)
                best[S] = best_plan;
        }
    }

    // full set = answer
    set<string> all;
    for (auto& n : rel_names) all.insert(n);

    return best[all];
}

Plan naive_plan(const map<string, Relation>& relations,const vector<JoinPredicate>& join_preds,const vector<string>& order){
    Plan curr;

    // base scan
    const Relation& first = relations.at(order[0]);
    curr.relations = {order[0]};
    curr.cost = first.num_pages;
    curr.cardinality = first.num_tuples;
    curr.repr = order[0];
    curr.distinct = first.distinct;

    // left-to-right joins
    for (int i = 1; i < order.size(); i++) {
        const string& r = order[i];
        const Relation& right = relations.at(r);

        JoinPredicate pred;
        bool found = false;

        for (auto& p : join_preds) {
            if ((curr.relations.count(p.left_rel) && p.right_rel == r) ||
                (curr.relations.count(p.right_rel) && p.left_rel == r)) {
                pred = p;
                found = true;
                break;
            }
        }

        if (!found) {
            throw runtime_error("No join predicate for naive plan");
        }

        double cost = estimate_hash_join_cost(curr, right);
        long long card = estimate_join_cardinality(curr, right, pred);

        curr.relations.insert(r);
        curr.cost = cost;
        curr.cardinality = card;
        curr.repr = "(" + curr.repr + " ⨝ " + r + ")";
        curr.distinct = propagate_stats(curr, right, pred);
    }

    return curr;
}

struct Node {
    string val;
    Node* left;
    Node* right;

    Node(string v) : val(v), left(nullptr), right(nullptr) {}
};
static Node* parse_plan(const string& s, int& i) {
    // Skip whitespace
    while (i < s.size() && s[i] == ' ')
        i++;

    // Case 1: join expression
    if (s[i] == '(') {
        i++; // skip '('

        Node* left = parse_plan(s, i);

        // Skip until we hit the right operand
        while (i < s.size() && s[i] != '(' && !isalnum(s[i]))
            i++;

        Node* right = parse_plan(s, i);

        // Skip closing ')'
        while (i < s.size() && s[i] != ')')
            i++;
        i++; // skip ')'

        Node* root = new Node("⨝");
        root->left = left;
        root->right = right;
        return root;
    }

    // Case 2: leaf (relation name)
    string name;
    while (i < s.size() && isalnum(s[i])) {
        name += s[i++];
    }

    return new Node(name);
}

static void print_tree(Node* root, int depth = 0) {
    if (!root) return;

    print_tree(root->right, depth + 1);

    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << root->val << "\n";

    print_tree(root->left, depth + 1);
}
void print_plan_tree(const string& repr) {
    int idx = 0;
    Node* root = parse_plan(repr, idx);
    print_tree(root);
}

