#include <bits/stdc++.h>


#include "stats.h"
#include "plan.h"
#include "predicate.h"
#include "cost.h"

using namespace std;

int main() {
    // Base relation R
    Relation R;
    R.name = "R";
    R.num_tuples = 100000;
    R.num_pages = 1000;
    R.distinct["a"] = 1000;

    // Base relation S
    Relation S;
    S.name = "S";
    S.num_tuples = 50000;
    S.num_pages = 500;
    S.distinct["b"] = 500;

    // Base scan plan for R
    Plan p;
    p.relations = {"R"};
    p.cardinality = R.num_tuples;
    p.distinct = R.distinct;

    // Join predicate R.a = S.b
    JoinPredicate pred{"R", "a", "S", "b"};

    long long card = estimate_join_cardinality(p, S, pred);

    // Expected: (100000 * 50000) / max(1000, 500) = 5,000,000
    assert(card == 5000000);

    cout << "[PASS] Join cardinality estimation test\n";
    return 0;
}
