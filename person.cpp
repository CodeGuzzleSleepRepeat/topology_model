#include "person.h"

bool functor::operator()(double a, double b){
    return fabs(a - b) < 0.01;
}

person::person(int a, int c, int type, vector<int> &points){
    age = a;
    culture = c;
    educ_type = type;  
    int size = points.size();
    for (int i = 0; i < size; i++)
        values.insert(make_pair(points[i], 0.));
    max = points[size - 1];
}
person::person(int a, int c, int type, vector<double>& v, vector<int> &points){
    age = a;
    culture = c;
    educ_type = type;
    int size = v.size();
    if (points.size() != size){
        cout << "Bad arrays in init person" << endl;
        return;
    }
    for (int i = 0; i < size; i++)
        values.insert(make_pair(points[i], v[i]));
    max = points[size - 1];
}

void person::get_older(){
    age++;
}

double person::get_val(int i){
    auto iter = values.find(i);
    if (iter == values.end())
        return -1;

    return iter->second;
}

double person::get_main_val(int i){
    if (main_values.find(i) == main_values.end())
        return -1;
    return main_values[i];
}

void person::get_all_values(vector<double> &vec){
    for (auto &el : values)
        vec.push_back(el.second);
}

void person::get_points(vector<int> & vec){
    for (auto &el : values)
        vec.push_back(el.first);
}

int person::get_size(){
    return values.size();
}

int person::get_age(){
    return age;
}

void person::set_val(int pos, double val){
    if (values.find(pos) == values.end()){
        values.insert(make_pair(pos, val));
        if (pos > max)
            max = pos;
    }
    else
        values[pos] = val;
}

void person::set_main_val(int pos, double val){
    if (main_values.find(pos) == main_values.end())
        main_values.insert(make_pair(pos, val));
    else
        main_values[pos] = val;
}

void person::money(vector<person>& people){

}

void person::grow(vector<person> &people){
    int k = 10, point, size_val = values.size(), size_p = people.size();
    srand(time(NULL));
    double sum;
    for (int i = 0; i < k; i++){
        sum = 0.;
        point = rand() % size_val;
        auto iter = values.begin();
        for (int i = 0; i < point; i++)
            ++iter;
        for (int j = 0; j < size_p; j++)
            sum += people[j].get_val(iter->first);
        iter->second = sum / size_p + (rand() % 20) / 100.;
    }
}

int person::get_type(){
    return educ_type;
}


void person::set_best(int val){
    sets.push_back(val);
}

int person::set_values(vector<vector<int>> &vec){
    double size = vec.size();
    for (int i = 0; i < size; i++){
        for (auto &el : vec[i]){
            auto iter = values.find(el);
            if (iter == values.end())
                return -1;
            iter->second = i / (size - 1.);
        }
    }
    return 1;
}

void person::get_best(vector<int> &vec){
    for (auto &el : sets)
        vec.push_back(el);
}

void person::clear_best(){
    sets.clear();
}


int person::max_point(){
    return max;
}

int person::get_culture(){
    return culture;
}



