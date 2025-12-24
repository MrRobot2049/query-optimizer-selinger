#ifndef STATS_H
#define STATS_H

#include <string>
#include <unordered_map>
using namespace std;

struct Relation {
    string name;
    long long num_tuples;
    long long num_pages;
    unordered_map<string, long long> distinct;
};

#endif
