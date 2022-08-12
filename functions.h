#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "communications.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

/*int elite_educate(vector<vector<person>>&, vector<vector<pair<int, double>>> &res);
int common_educate(vector<vector<person>>&);
int marginal_educate(vector<vector<person>>&);
int educate(vector<vector<person>>&, vector<vector<pair<int, double>>> &res);
pair<int, double> soc_influence(vector<person>&, set<int>&);
int outer_influence(vector<vector<person>> &, vector<vector<pair<int, double>>> &, communications *, int);

int flow_influence(vector<vector<person>> &, communications *);*/

int count_fixed_points(vector<vector<person>>&, vector<int>&, communications *);

void check(vector<vector<person>>&);
void check2(vector<vector<person>>&);

void get_real_data(vector<vector<person>> &people, vector<vector<int>> &cultures);


#endif // FUNCTIONS_H
