#ifndef COST_H
#define COST_H

#include "stats.h"
#include "plan.h"
#include "predicate.h"

long long estimate_join_cardinality(
    const Plan& left,
    const Relation& right,
    const JoinPredicate& pred
);

double estimate_hash_join_cost(
    const Plan& left,
    const Relation& right
);

#endif
