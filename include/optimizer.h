#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <bits/stdc++.h>
using namespace std;
#include "plan.h"
#include "stats.h"
#include "predicate.h"

Plan optimize_query(
    const map<string, Relation>& relations,
    const vector<JoinPredicate>& join_preds
);

Plan naive_plan(
    const map<string, Relation>& relations,
    const vector<JoinPredicate>& join_preds,
    const vector<string>& order
);
void print_plan_tree(const string& repr);


#endif