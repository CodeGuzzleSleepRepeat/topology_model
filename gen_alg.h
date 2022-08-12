#ifndef GEN_ALG_H
#define GEN_ALG_H
#include "functions.h"

class gen_alg
{   
    vector<vector<vector<int>>>* sets;
public:
    gen_alg(unsigned int);
    void create_new_sets(vector<int>&, int);
    void build_functions(map<double, int>&, int, person);
    void count(vector<person>&, int, vector<int>&);
    void breed(vector<person> &, int);
    int mix(int, int, int, person);
};

#endif // GEN_ALG_H
