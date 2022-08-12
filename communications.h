#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H
#include <vector>
#include "person.h"
using namespace std;

class graph{
    vector<vector<double>> ways;
public:
    graph(){}
    graph(vector<vector<double>>&);
    int get_size();
    double get_way(int, int);
};


class communications
{
    graph * gr;
    int cur_max;
public:
    communications(){}
    communications(graph * g, vector<vector<int>>&);
    void exec(vector<vector<int>> &, vector<vector<person>>&);
    double get_way(int, int);
};

int image(person, person, int);

#endif // COMMUNICATIONS_H
