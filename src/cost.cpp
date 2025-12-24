
#include "cost.h"
#include <bits/stdc++.h>
using namespace std;

static long long get_distinct(
    const Plan& left,
    const Relation& right,
    const JoinPredicate& pred,
    bool left_side
) {
    if (left_side) {
        if (left.distinct.count(pred.left_attr))
            return left.distinct.at(pred.left_attr);
        if (left.distinct.count(pred.right_attr))
            return left.distinct.at(pred.right_attr);
    } else {
        if (right.distinct.count(pred.left_attr))
            return right.distinct.at(pred.left_attr);
        if (right.distinct.count(pred.right_attr))
            return right.distinct.at(pred.right_attr);
    }
    return 1; // safe fallback
}


long long estimate_join_cardinality(
    const Plan& left,
    const Relation& right,
    const JoinPredicate& pred
) {
    long long left_card = left.cardinality;
    long long right_card = right.num_tuples;

    long long v_left = get_distinct(left, right, pred, true);
    long long v_right = get_distinct(left, right, pred, false);

    long long denom = max(v_left, v_right);
    if (denom == 0) denom = 1;

    return (left_card * right_card) / denom;
}

double estimate_hash_join_cost(
    const Plan& left,
    const Relation& right
) {
    // very simple model
    return left.cost + left.cardinality + right.num_pages;
}
