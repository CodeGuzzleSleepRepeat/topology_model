#include "gen_alg.h"

gen_alg::gen_alg(unsigned int num_of_cult){
    sets = new vector<vector<vector<int>>>[num_of_cult];
}

int create_cur_set(vector<int> &vec, vector<vector<int>> &cur_set){
    int r, r2 = 0, size = vec.size();
    vector<int> tmp;
    srand(time(NULL));
    if (size == 1){
        tmp.push_back(vec[0]);
        cur_set.push_back(tmp);
        return -1;
    }

    while (r2 == 0)
        r2 = rand() % size;
    auto beg = vec.begin();
    for (int i = 0; i < r2; i++){
        r = rand() % vec.size();
        tmp.push_back(*(beg + r));
        vec.erase(beg + r);
    }

    cur_set.push_back(tmp);
    return 1;
}

void gen_alg::create_new_sets(vector<int>& vec, int cult){
    int num = 1000;
    vector<vector<int>> cur_set;
    for (int i = 0; i < num; i++){
        vector<int> tmp(vec);
        while (create_cur_set(tmp, cur_set) == 1);
        sets[cult].push_back(cur_set);
        cur_set.clear();
    }
}


void gen_alg::build_functions(map<double, int> &best, int cult, person el){
    int size, cur_size, outer_size = sets[cult].size();
    double mistake;
    double point;
    for (int j = 0; j < outer_size; j++){
        size = sets[cult][j].size();
        if (size < 2)
            continue;

        mistake = 0.;
        for (auto &cur_set : sets[cult][j]){
            cur_size = cur_set.size();
            for (int i = 0; i < cur_size; i++){
                point = el.get_val(cur_set[i]);
                if (point >= 0)
                    mistake += fabs(point - (double)i / (size - 2));
                else cout << "Bad build" << endl;
            }
        }
        best.insert(make_pair(mistake, j));
    }
}

void gen_alg::count(vector<person> &people, int cult, vector<int> &points){
    int num_of_best = 10, size, size_tmp;
    vector<vector<vector<int>>> sets_tmp;   
    map<double, int> best;

    //create_new_sets(points, cult);
    for (auto &el : people){
        build_functions(best, cult, el);
        size = best.size();
        size_tmp = sets_tmp.size();
        if (num_of_best > size)
            num_of_best = size;

        auto iter = best.begin();
        //cout << "Least mistake " << iter->first << endl;
        el.set_values(sets[cult][iter->second]);
        el.clear_best();
        for (int i = 0; i < num_of_best; i++){
            sets_tmp.push_back(sets[cult][iter->second]);
            el.set_best(size_tmp + i);
            ++iter;
        }
        best.clear();
    }

    sets[cult] = move(sets_tmp);
}

int gen_alg::mix(int num1, int num2, int cult, person el){
    int cur_size, size1, size2, cur_mistake, points_num = el.max_point();
    vector<int> mist(points_num), set_numbers(points_num), * res;
    vector<vector<int>> new_set;
    int j = 0;
    double point;

    for (auto &el : set_numbers)
        el = -1;

    size1 = sets[cult][num1].size();
    size2 = sets[cult][num2].size();
    int s = max(size1, size2);
    if (size1 < 2 || size2 < 2){
        //cout << "Bad sets" << endl;
        return -1;
    }

    for (auto &cur_set : sets[cult][num1]){
        cur_size = cur_set.size();
        for (int i = 0; i < cur_size; i++){
            point = el.get_val(cur_set[i]);
            if (point >= 0){
                mist[cur_set[i]] = fabs(point - (double)i / (size1 - 1));
                set_numbers[cur_set[i]] = j;
            }
            else cout << "Bad mix" << endl;
        }
        j++;
    }

    j = 0;
    for (auto &cur_set : sets[cult][num2]){
        cur_size = cur_set.size();
        for (int i = 0; i < cur_size; i++){
            point = el.get_val(cur_set[i]);
            if (point >= 0){
                cur_mistake = fabs(point - (double)i / (size2 - 1));
                if (cur_mistake < mist[cur_set[i]])
                    set_numbers[cur_set[i]] = j;
            }
            else cout << "Bad mix" << endl;
        }
        j++;
    }

    res = new vector<int>[s];
    new_set.reserve(s);

    vector<int> vec;
    el.get_points(vec);
    points_num = vec.size();
    for (int i = 0; i < points_num; i++)
        if (set_numbers[i] >= 0)
            res[set_numbers[i]].push_back(vec[i]);


    for (int i = 0; i < s; i++){
        if (res[i].size() == 0)
            break;
        new_set.push_back(res[i]);
    }

    sets[cult].push_back(new_set);
    return sets[cult].size() - 1;
}



void gen_alg::breed(vector<person> &people, int cult){
    vector<int> best;
    vector<int>::iterator beg, end;
    int res;
    for (auto &p : people){
        p.get_best(best);
        beg = best.begin();
        end = best.end();
        for (auto iter = beg; iter != end; ++iter)
            for (auto iter2 = iter + 1; iter2 != end; ++iter2){
                res = mix(*iter, *iter2, cult, p);
                if (res >= 0)
                    p.set_best(res);
            }
        best.clear();
    }
}




