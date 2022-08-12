#ifndef PERSON_H
#define PERSON_H
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <math.h>
#include "time.h"
using namespace std;

#define _AGE_EDGE 18
#define _MIN_EDUC 15
#define _MAX_EDUC 24

class functor{
public:
    bool operator()(double, double);
};

class person
{
    int age;
    int culture;
    int educ_type;
    int max;
    map<int, double> values;
    map<int, double> main_values;
    vector<int> sets;
public:
    person(){}
    person(int, int, int, vector<int>&);
    person(int, int, int, vector<double>&, vector<int>&);
    void get_older();
    void grow(vector<person>&);
    void money(vector<person>&);
    int get_age();
    int get_culture();
    int get_size();
    double get_val(int);
    double get_main_val(int);
    void get_all_values(vector<double>&);
    void get_points(vector<int> &);
    void get_best(vector<int> &);
    int get_type();
    void set_val(int, double);
    void set_main_val(int, double);
    int set_values(vector<vector<int>> &);
    void set_best(int);
    void clear_best();
    int max_point();
};



#endif // PERSON_H
