#ifndef PLAN_H
#define PLAN_H
#include <bits/stdc++.h>
using namespace std;


struct Plan {
    set<string> relations;
    double cost;
    long long cardinality;
    string repr;
    unordered_map<string, long long> distinct;
};

#endif
