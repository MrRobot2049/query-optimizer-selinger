#include <bits/stdc++.h>

#include "optimizer.h"

using namespace std;

int main() {
    map<string, Relation> relations;

    relations["R"] = {"R", 100000, 1000, {{"a", 1000}}};
    relations["S"] = {"S", 50000, 500, {{"b", 500}}};
    relations["T"] = {"T", 10000, 100, {{"c", 100}}};

    vector<JoinPredicate> preds;
    preds.push_back({"R","a","S","b"});
    preds.push_back({"S","b","T","c"});

    vector<string> naive_order = {"R","S","T"};

    Plan naive = naive_plan(relations, preds, naive_order);
    Plan opt   = optimize_query(relations, preds);

    assert(opt.cost <= naive.cost);

    cout << "[PASS] Optimized plan cheaper than naive plan\n";
    return 0;
}
