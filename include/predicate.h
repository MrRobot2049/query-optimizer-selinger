#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
using namespace std;

struct JoinPredicate {
    string left_rel;
    string left_attr;
    string right_rel;
    string right_attr;
};

#endif
