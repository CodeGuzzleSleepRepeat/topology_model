#include "gen_alg.h"
#include <fstream>
using namespace std;

#define NUM_OF_STEPS 20


void init_values(ifstream &f, vector<vector<double>> &values){
    unsigned int n = 0;
    f >> n;
    vector<int> sizes(n);
    for (int i = 0; i < n; i++){
        f >> sizes[i];
        vector<double> tmp(sizes[i]);
        for (int j = 0; j < sizes[i]; j++)
            f >> tmp[j];
        values.push_back(tmp);
    }
}

void init_cultures(ifstream &f, vector<vector<int>> &cultures){
    unsigned int n = 0;
    f >> n;
    vector<int> sizes(n);
    for (int i = 0; i < n; i++)
        f >> sizes[i];
    for (int i = 0; i < n; i++){
        vector<int> tmp(sizes[i]);
        for (int j = 0; j < sizes[i]; j++)
            f >> tmp[j];
        cultures.push_back(tmp);
    }
}

void init_people(vector<vector<person>> &people, vector<vector<int>> &cultures){
    int size = cultures.size(), num_of_people = 50, size_val;
    person * tmp;
    vector<person> tmp_vec;

    for (int i = 0; i < size; i++){
        size_val = cultures[i].size();
        for (int j = 0; j < num_of_people; j++){
            tmp = new person(rand() % 60, i, rand() % 2 + 1, cultures[i]);
            for (int k = 0; k < size_val; k++)
                tmp->set_val(rand() % size_val + cultures[i][0], (rand() % 100) / 100.);

            tmp_vec.push_back(*tmp);
        }
        people.push_back(tmp_vec);
        tmp_vec.clear();
    }
}


graph * init_graph(ifstream &f, vector<vector<int>> &cultures){
    unsigned int n;
    vector<vector<double>> vec;
    f >> n;
    if (n != cultures.size()){
        cout << "Bad graph file" << endl;
        return nullptr;
    }

    for (int i = 0; i < n; i++){
        vector<double> tmp(n);
        for (int j = 0; j < n; j++)
            f >> tmp[j];
        vec.push_back(tmp);
    }
    return new graph(vec);
}

void demography(vector<vector<person>> &people, vector<vector<int>> &cultures){
    int size = cultures.size(), num_of_people = 20, size_val;
    double coeff = 0.15;
    person * tmp;
    srand(1);
    for (int i = 0; i < size; i++){
        size_val = cultures[i].size();
        for (int j = 0; j < num_of_people * coeff; j++){
            tmp = new person(0, i, rand() % 2 + 1, cultures[i]);
            people[i].push_back(*tmp);
        }
        for (auto iter = people[i].begin(); iter != people[i].end(); ++iter){
            iter->get_older();
            if (iter->get_age() - 50 + rand() % 10 > 8)
                people[i].erase(iter);
        }
    }
}

void print(vector<vector<person>> &people){
    for (auto &el : people){
        for (auto &el2 : el){
            int s = el2.get_size();
            for (int i = 0; i < s; i++)
                cout << el2.get_val(i) << " ";
            cout << endl;
        }
        cout << endl << endl;
    }
}



void step(vector<vector<person>> &people, vector<vector<int>> &cultures,
          gen_alg * alg, communications * comm){
    int size = cultures.size(), num_of_alg_steps = 1;
    demography(people, cultures);
    for (int s = 0; s < 2; s++){
    for (int i = 0; i < size; i++){
        count_fixed_points(people, cultures[i], comm);
        for (int j = 0; j < num_of_alg_steps; j++){
            alg->create_new_sets(cultures[i], i);
            for (int k = 0; k < 4; k++){
                alg->count(people[i], i, cultures[i]);
                alg->breed(people[i], i);
            }
        }
    }
    }
    comm->exec(cultures, people);
}

void analyze(vector<vector<person>> &people, vector<vector<int>> &cultures){
    int size = cultures.size(), num_of_people;
    double sum = 0, sum2 = 0, sum3 = 0, sum4 = 0, tmp, res;
    vector<int> counter(size);
    for (auto &el : counter)
        el = 0;

    for (int i = 0; i < size; i++){
        for (auto &el : cultures[i]){
            num_of_people = people[i].size();
            for (auto &p : people[i]){
                tmp = p.get_val(el);
                sum += tmp;
                sum2 += pow(tmp, 2);
                sum3 += pow(tmp, 3);
                sum4 += pow(tmp, 4);
            }
            sum /= num_of_people;
            sum2 /= num_of_people;
            sum3 /= num_of_people;
            sum4 /= num_of_people;
            if (sum2 - pow(sum, 2) == 0.)
                res = 0.;
            else
                res = (sum4 - 4 * sum3 * sum + 6 * sum2 * pow(sum, 2) - 3 * pow(sum, 4)) /
                    pow(sum2 - pow(sum, 2), 2) - 3;
            cout << res << " ";
            if (res < -0.5)
                counter[i]++;
            sum = 0;
            sum2 = 0;
            sum3 = 0;
            sum4 = 0;
        }
        cout << endl;
    }

    cout << "Results" << endl;
    for (auto &el : counter)
        cout << el << endl;
}

void output(ofstream &f, vector<person> &people){
    vector<double> vec;
    vector<vector<double>> values;
    vector<int> points;
    people[0].get_points(points);

    for (auto &el2 : people){
        el2.get_all_values(vec);
        values.push_back(vec);
        vec.clear();
    }
    int size = values[0].size();
    int outer_size = values.size();

    for (auto &el : values)
        for (int i = 0; i < size; i++)
            f << points[i] << " " << el[i] << endl;
}

void new_output(ofstream &f, vector<person> &people, int step, int num){
    vector<map<double, int>> out;
    map<double, int> tmp_map;
    vector<int> points;
    int size;
    double tmp;
    if (people.size() == 0)
        return;

    people[0].get_points(points);
    size = points.size();
    //for (int i = 0; i < size; i++){
        for (auto &el : people){
            tmp = el.get_val(points[num]);
            tmp_map.insert(make_pair(tmp, 0));
            tmp_map[tmp]++;
        }
        out.push_back(tmp_map);
        tmp_map.clear();
    //}

    /*for (auto &el : out){
        for (auto &el2 : el)
            cout << el.first << " ";
        cout << endl;
    }*/

    //for (int i = 0; i < size; i++)
        for (auto &el : out[0])
           f << step << " " << el.first << endl;// " " << el.second << endl;
    /*for (auto &el : points)
        f << el << " ";
    f << endl;

    for (int i = 0; i < size; i++){

    }*/
}

void get_data(vector<vector<person>> &people, vector<vector<int>> &cultures){
    int tmp;
    double val;
    ifstream f;
    vector<person> vec1, vec2, vec3;
    for (int i = 0; i < 2128 / 16; i++){
        person * p1 = new person(40, 0, 0, cultures[0]);

        vec1.push_back(*p1);

    }

    for (int i = 0; i < 2112 / 16; i++){
        person * p2 = new person(40, 0, 0, cultures[0]);

        vec2.push_back(*p2);

    }

    for (int i = 0; i < 2064 / 16; i++){
        person * p3 = new person(40, 0, 0, cultures[0]);
        vec3.push_back(*p3);
    }

    f.open("result2_2.txt");
    for (int i = 0; i < 2064; i++){
        f >> tmp;
        f >> val;
        vec3[i / 16].set_val(tmp, val);

    }
    ifstream g;
    g.open("result2_1.txt");
    for (int i = 0; i < 2112; i++){
        g >> tmp;
        g >> val;
        vec2[i / 16].set_val(tmp, val);
    }

    ifstream h;
    h.open("result2_0.txt");
    for (int i = 0; i < 2128; i++){
        h >> tmp;
        h >> val;
        vec1[i / 16].set_val(tmp, val);
    }

    people.push_back(vec1);
    people.push_back(vec2);
    people.push_back(vec3);
    check2(people);
    cout << endl << endl;
}

int main(){
    srand(time(NULL));
    vector<vector<int>> cultures;
    vector<vector<person>> people;
    ifstream f("points.txt"), g("graph.txt");
    ofstream * out;
    if (!f.is_open() || !g.is_open()){
        cout << "Can`t open file" << endl;
        return -1;
    }

    for (int count = 0; count < 5; ++count){
    init_cultures(f, cultures);
    f.close();
    init_people(people, cultures);
    gen_alg * alg = new gen_alg(cultures.size());

    graph * gr = init_graph(g, cultures);
    g.close();
    if (!gr)
        return -1;
    communications * comm = new communications(gr, cultures);

    out = new ofstream[6];
    /*for (int i = 0; i < 12; i++){
        out[i].open("govno_third_" + to_string(i) + ".txt");
        if (!out[i].is_open()){
            cout << "Bad govno" << endl;
            return 0;
        }

    }

    for (int i = 0; i < 3; i++)
        new_output(out[i], people[i]);

    for (int j = 0; j <= NUM_OF_STEPS; j++){
        step(people, cultures, alg, comm);

        if (j % 10 == 0 && j > 0)
            for (int i = 0; i < 3; i++)
                new_output(out[j / 10 * 3 + i], people[i]);
        cout << j << endl;
    }*/



    for (int i = 0; i < 6; i++)
        out[i].open("graph_t_" + to_string(i) + "_" + to_string(count) + ".txt");

    for (int i = 0; i < 3; i++){
            new_output(out[i], people[i], 0, 5);
            new_output(out[i + 3], people[i], 0, 9);
    }

    for (int j = 0; j <= NUM_OF_STEPS; j++){
        step(people, cultures, alg, comm);

        if (j % 5 != 0) continue;
        for (int i = 0; i < 3; i++){
            new_output(out[i], people[i], j + 1, 5);
            new_output(out[i + 3], people[i], j + 1, 9);
        }

        cout << j << endl;
    }
    people.clear();
    cultures.clear();
    }

    system("pause");
    return 0;
}


/*if (j % 25 == 0){
    //analyze(people, cultures);
    //system("pause");
    //for (int i = 0; i < size; i++)
      //  new_output(out[j * NUM_OF_STEPS / 100 + i], people[i]);
    cout << j << endl;
    flag = false;
}*/

/*out1 = new ofstream[3];
out2 = new ofstream[3];
out3 = new ofstream[3];
for (int i = 0; i < 3; i++){
    out1[i].open("result_fin1_" + to_string(i) + ".txt");
    out2[i].open("result_fin2_" + to_string(i) + ".txt");
    out3[i].open("result_fin3_" + to_string(i) + ".txt");
}

for (int i = 0; i < 3; i++)
    new_output(out1[i], people[i]);*/

