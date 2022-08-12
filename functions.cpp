#include "functions.h"





void get_real_data(vector<vector<person>> &people, vector<vector<int>> &cultures){
    int n = 100;
    vector<person> vec(n);
    vector<int> cult;
    vector<vector<int>> values;
    ifstream f;
    
    for (int i = 0; i < 8; ++i){
        f.open("data" + to_string(i) + ".csv");
        getline(f, tmp, ',');
        while (f){
            getline(f, tmp, ',');
            stringstream ss(tmp);
            vector<int> val;
            for (int j = 0; j < 1 + (i == 0); ++j)
                getline(ss, tmp2, ',');
            while(ss){
                tmp2.clear();
                getline(ss, tmp2, ',');
                tmp2 = tmp2.substr(1, tmp2.length() - 2);
                if (tmp2 == '-')
                    tmp2 = '0';
                val.push_back(stoi(tmp2));
            }

            values.push_back(val);
        }
        size += val.size();
    }
    
    for (int i = 0; i < size; ++i)
        cult.push_back(i);

    for (int i = 0; i < n; ++i){
        person * chel = new person(rand() % 40 + 10, 0, 0, cult);
        vec[i] = *chel;
        for (auto &el : cult)
            vec[i].set_val(el, values[0][el])
    }
    
    
    
    people.push_back(vec);
}








void check(vector<vector<person>> &people){
    vector<int> vec;
    for (auto &el : people){
        el[0].get_points(vec);
        for (auto &el : vec)
            cout << el << " ";
        cout << endl;
        vec.clear();
    }
    cout << endl;
}

void check2(vector<vector<person>> &people){
    vector<double> vec;
    for (auto &el : people){
        for (auto &el2 : el){
            el2.get_all_values(vec);
            for (auto &el : vec)
                cout << el << " ";
            cout << endl;
            vec.clear();
        }
        cout << endl;

        system("pause");
    }
}


int elite_educate(vector<vector<person>>& people, vector<vector<pair<int, double>>> &res){
    int size = people.size();
    srand(time(NULL));
    for (int i = 0; i < size; i++)
        for (auto &el : people[i])
            if (el.get_age() <= _MAX_EDUC && el.get_age() >= _MIN_EDUC && el.get_type() == 2)
                for (int j = 0; j < size; j++)
                    el.set_val(image(people[j][0], el, res[0][j].first),
                            res[0][j].second + (rand() % 10) / 100.);
    return 0;
}

int common_educate(vector<vector<person>>& people, vector<vector<pair<int, double>>> &res){
    int size = people.size();
    srand(time(NULL));
    for (int i = 0; i < size; i++)
        for (auto &el : people[i])
            if (el.get_age() <= _MAX_EDUC && el.get_age() >= _MIN_EDUC && el.get_type() == 1)
                for (int j = 0; j < size; j++){
                    if (rand() % 2 == 0 && j != i)
                        continue;
                    el.set_val(image(people[j][0], el, res[0][j].first),
                            res[0][j].second + (rand() % 10) / 100.);
                }

    return 0;
}

/*int marginal_educate(vector<vector<person>>& people){
    int size = people.size();
    for (int i = 0; i < size; i++){
        if (rand() % 2 == 0)
            continue;
        for (auto &el : people[i])
            if (el.get_age() <= _MAX_EDUC && el.get_age() >= _MIN_EDUC)
                for (int j = 0; j < size; j++)
                    el.set_val(image(people[j][0], el, res[0][j].first),
                            res[0][j].second + (rand() % 10) / 100.);
    }
    return 0;
}*/

int educate(vector<vector<person>>& people, vector<vector<pair<int, double>>> &res){
    elite_educate(people, res);
    common_educate(people, res);
    return 0;
}



pair<int, double> soc_influence(vector<person>& people, set<int> &prev){
    map<double, int> val;
    int max = -1, max_point = 0, tmp, size = people[0].get_size();
    double tmp_val, mem = 0;
    auto end = prev.end();
    vector<int> vec;
    people[0].get_points(vec);
    for (int i = 0; i < size; i++){
        if (prev.find(vec[i]) == end)
            continue;
        for (auto &p : people){
            tmp_val = p.get_val(vec[i]);
            if (tmp_val >= 0){
                val.insert(make_pair(tmp_val, 0));
                tmp = val[tmp_val];
                ++tmp;
                if (max < tmp){
                    max =  tmp;
                    mem = tmp_val;
                    max_point = i;
                }
            }
            else cout << "Bad influence" << endl;
        }
    }

    for (auto &el : people)
        el.set_val(max_point, mem);

    prev.insert(max_point);
    return make_pair(max_point, mem);

}

int outer_influence(vector<vector<person>> &people, vector<vector<pair<int, double>>> &res,
                    communications * comm, int k){
    int size = people.size(), size_val;
    int tmp_point, r;
    double tmp_val;
    srand(time(NULL));
    for (int i = 0; i < size; i++){
        size_val = people[i][0].get_size();
        for (int j = 0; j < size; j++){
            if (i == j)
                continue;
            r = rand() % k;
            tmp_point = image(people[j][0], people[i][0], res[j][r].first);
            tmp_val = res[i][r].second * comm->get_way(j, i);
            try{
                for (auto &el : people[i])
                    el.set_val(tmp_point, tmp_val + (rand() % 25) / 100.);
            }
            catch(const char *){
                throw "Outer";
            }
        }
    }
    return 0;
}

int flow_influence(vector<vector<person>> &people, communications * comm){
    int k = 10, size = people.size();
    vector<vector<pair<int, double>>> res;
    vector<pair<int, double>> tmp(k);
    for (int i = 0; i < k; i++)
        res.push_back(tmp);
    set<int> prev;

    try{
        for (int j = 0; j < size; j++){
            for (int i = 0; i < k; i++)
                res[j][i] = soc_influence(people[j], prev);
            educate(people, res);
            outer_influence(people, res, comm, k);
        }
    }
    catch(const char * val){
        cout << "Can`t set value in " << val << endl;
        return -1;
    }

    return 0;
}


int count_fixed_points(vector<vector<person>>& people_vec, vector<int>& cultures,
                       communications * comm){
    if (flow_influence(people_vec, comm) < 0)
        return -1;
    for (auto &people : people_vec)
        for (auto &el : people){
            if (el.get_age() < _AGE_EDGE)
                el.grow(people);
            el.money(people);
        }   
    return 1;
}


//culture_comm(people_vec, comm);

int culture_comm(vector<vector<person>>& people_vec, communications * comm){
    return 0;
}

