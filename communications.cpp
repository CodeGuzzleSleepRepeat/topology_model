#include "communications.h"


graph::graph(vector<vector<double>> &vec){
    unsigned int size = vec.size();
    vector<double> tmp(size);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++)
            tmp[j] = vec[i][j];
        ways.push_back(tmp);
    }
}

double graph::get_way(int i, int j){
    return ways[i][j];
}

int graph::get_size(){
    return ways.size();
}

communications::communications(graph * g, vector<vector<int>> &cultures){
    gr = g;
    cur_max = 0;
    for (auto &el : cultures)
        for (auto &el2 : el)
            if (el2 > cur_max)
                cur_max = el2;
}

void communications::exec(vector<vector<int>> &cultures, vector<vector<person>> &people){
    int size = gr->get_size();
    for (int i = 0; i < size; i++){
        for (int j = i + 1; j < size; j++){
            if (gr->get_way(i, j) == 0. || cultures[i].size() == cultures[j].size())
                continue;
            if (cultures[i].size() > cultures[j].size()){
                cultures[j].push_back(cur_max + 1);
                for (auto &el : people[j])
                    el.set_val(cur_max + 1, 0.);
            }
            else{
                cultures[i].push_back(cur_max + 1);
                for (auto &el : people[i])
                    el.set_val(cur_max + 1, 0.);
            }
            cur_max++;
        }
    }
}

double communications::get_way(int a, int b){
    return gr->get_way(a, b);
}


int image(person el1, person el2, int point){
    if (el1.get_culture() == el2.get_culture())
        return point;
    vector<int> vec1, vec2;
    el1.get_points(vec1);
    el2.get_points(vec2);
    int size = min(vec2.size(), vec1.size());
    for (int i = 0; i < size; i++)
        if (vec1[i] == point)
            return vec2[i];
    return vec2[size - 1];
}


