// #include <bits/stdc++.h>
// #include <map>
// using namespace std;
// #include "stats.h"
// #include "plan.h"
// #include "cost.h"
// #include "predicate.h"
// const int INF = INT_MAX;
// vector<JoinPredicate> join_preds;

// int main() {
//     map<string, Relation> relations;

//     relations["R"] = {"R", 100000, 1000, {}};
//     relations["S"] = {"S", 50000, 500, {}};
//     relations["T"] = {"T", 10000, 100, {}};
//     relations["R"].distinct["a"] = 1000;
//     relations["S"].distinct["b"] = 500;
//     JoinPredicate rs_join {
//         "R", "a",
//         "S", "b"
//     };
//     join_preds.push_back({"R","a","S","b"});
//     join_preds.push_back({"S","b","T","c"});


//     map<set<string>, Plan> best;

//     for (auto &[name, rel] : relations) {
//         Plan p;
//         p.relations = {name};
//         p.cost = rel.num_pages;          // scan cost
//         p.cardinality = rel.num_tuples;  // scan cardinality
//         p.repr = name;

//         best[{name}] = p;
//     }
//     //vector<string> rel_names = {"R", "S", "T"};
    


    

//     cout << "Base scan plans:\n";
//     for (auto &[k, p] : best) {
//         cout << "Scan " << p.repr << " | cost = " << p.cost << " | card = " << p.cardinality << "\n";
//     }
//     // candidate 1: (R ⨝ S)
//     // Plan p1;
//     // p1.relations = {"R", "S"};
//     // p1.cost = estimate_hash_join_cost(best[{"R"}], relations["S"]);
//     // p1.cardinality = estimate_join_cardinality(best[{"R"}], relations["S"], rs_join);
//     // p1.repr = "(R ⨝ S)";

//     // // candidate 2: (S ⨝ R)
//     // Plan p2;
//     // p2.relations = {"R", "S"};
//     // p2.cost = estimate_hash_join_cost(best[{"S"}], relations["R"]);
//     // p2.cardinality = estimate_join_cardinality(best[{"S"}], relations["R"], rs_join);
//     // p2.repr = "(S ⨝ R)";

//     // // choose cheaper
//     // best[{"R","S"}] = (p1.cost < p2.cost) ? p1 : p2;
//     // auto &p = best[{"R","S"}];
//     // cout << "Best plan for {R,S}: " << p.repr
//     //       << " | cost = " << p.cost
//     //       << " | card = " << p.cardinality << "\n";



//     return 0;
// }

#include <iostream>
#include <map>
#include <vector>

#include "optimizer.h"

using namespace std;

int main() {
    map<string, Relation> relations;

    relations["R"] = {"R", 100000, 1000, {{"a", 1000}}};
    relations["S"] = {"S", 50000, 500, {{"b", 500}}};
    relations["T"] = {"T", 10000, 100, {{"c", 100}}};

    vector<JoinPredicate> join_preds;
    join_preds.push_back({"R","a","S","b"});
    join_preds.push_back({"S","b","T","c"});

    Plan best = optimize_query(relations, join_preds);

    cout << "Optimal plan: " << best.repr << "\n";
    cout << "Estimated cost: " << best.cost << "\n";
    cout << "Estimated rows: " << best.cardinality << "\n";

    vector<string> naive_order = {"R", "S", "T"};
    Plan naive = naive_plan(relations, join_preds, naive_order);
    cout << "\nNaive plan:\n";
    cout << naive.repr << "\n";
    cout << "Cost: " << naive.cost << "\n";
    cout << "Rows: " << naive.cardinality << "\n";

    cout << "\nOptimized plan (tree view):\n";
    print_plan_tree(best.repr);


    return 0;
}
