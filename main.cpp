#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <fstream>
#include <random>

using namespace std;

random_device rd;
mt19937 rdgen(rd());

void generatePartitions(int V, int index, vector<int> &currentPartition, vector<vector<int>> &partitions) {
    if (index == V) {
        partitions.push_back(currentPartition);
        return;
    }


    currentPartition[index] = 0;
    generatePartitions(V, index + 1, currentPartition, partitions);


    currentPartition[index] = 1;
    generatePartitions(V, index + 1, currentPartition, partitions);
}

int goal_factory(vector<pair<int, int>> e, vector<int> partition) {
    int cut_value = 0;

    for (int i = 0; i < partition.size(); ++i) {
        for (int j = 0; j < partition.size(); ++j) {
            if (partition[i] == 0 && partition[j] == 1) {
                for (auto edges: e) {
                    if (i == edges.first && j == edges.second || i == edges.second && j == edges.first) {
                        cut_value++;
                    }
                }
            }
        }
    }

    return cut_value;
}

class Graph {
private:
    int V;
    vector<pair<int, int>> edges;
    vector<vector<int>> max_cuts;

public:

    Graph(int V) {
        this->V = V;
    }

    void addEdge(int u, int v) {
        edges.push_back({u, v});
    }

    const vector<pair<int, int>> getEdges() const {
        return edges;
    }

    vector<int> maximumCut() {

        vector<int> currentPartition(V);
        vector<vector<int>> partitions;
        generatePartitions(V, 0, currentPartition, partitions);


        vector<int> result;

        int max_cut_weight = 0;

        for (auto partition: partitions) {
            int cut = goal_factory(edges, partition);

            if (max_cut_weight < cut) {
                max_cut_weight = cut;
                result = partition;
            }
        }

        return result;
    }

    /*int maximumCut() {

        vector<int> currentPartition(V);
        vector<vector<int>> partitions;
        generatePartitions(V, 0, currentPartition, partitions);


        int max_cut_weight = 0;
        int index = 0;
        int temp = 0;

        for (const auto &partition: partitions) {
            vector<int> V1, V2;

            for (size_t i = 0; i < partitions.size(); i++) {
                if (partition[i] == 0) {
                    V1.push_back(i);
                } else {
                    V2.push_back(i);
                }
            }

            int cut = goal_factory(edges, V1, V2);

            if (max_cut_weight < cut) {
                max_cut_weight = cut;
                temp = index;
            }

            index++;
        }

        vector<int> V1, V2;

        for (size_t i = 0; i < partitions[temp].size(); ++i) {
            if (partitions[temp][i] == 0) {
                V1.push_back(i);
            } else {
                V2.push_back(i);
            }
        }

        cout << "V1: ";
        for (int v: V1) {
            cout << v << " ";
        }

        cout << " | V2: ";
        for (int v: V2) {
            cout << v << " ";
        }

        cout << "\n";

        return max_cut_weight;
    }*/

    int getVertices() {
        return V;
    }
};

void printDetailsAboutCut(Graph g, vector<int> partition) {

    auto max_cut = goal_factory(g.getEdges(), partition);

    vector<vector<int>> group_of_cut(2);

    for (int i = 0; i < g.getVertices(); ++i) {
        if (partition[i] > 0) {
            group_of_cut[0].push_back(i);
        } else {
            group_of_cut[1].push_back(i);
        }
    }

    cout << "V1: [ ";
    for (auto v1: group_of_cut[0]) {
        cout << v1 << " ";
    }
    cout << "]\n";

    cout << "V2: [ ";
    for (auto v2: group_of_cut[1]) {
        cout << v2 << " ";
    }
    cout << "]\n";

    cout << "Waga maksymalnego przecięcia: " << max_cut << "\n";

}

vector<int> generate_random_cut(Graph g, double p_1 = 0.5) {

    vector<int> group_of_cuts;

    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < g.getVertices(); ++i) {
        auto e = (dist(rdgen) < p_1) ? 1 : 0;

        if (e > 0) {
            group_of_cuts.push_back(0);
        } else {
            group_of_cuts.push_back(1);
        }
    }
    return group_of_cuts;
}

vector<int> generate_neighbours_cut(Graph g, vector<int> partition) {

    vector<int> neighbour_partition;

    for (auto p: partition) {
        if (p > 0) {
            neighbour_partition.push_back(0);
        } else {
            neighbour_partition.push_back(1);
        }
    }

    /*vector<vector<int>> neighbour_generate = fill_each_group(g, neighbour_partition);*/

    /*int max_cut = goal_factory(g.getEdges(), neighbour_generate[0], neighbour_generate[1]);

    printDetailsAboutCut(neighbour_generate, max_cut);*/

    return neighbour_partition;
}

vector<int> solve_random(Graph g, int iterations = 20, double p_1 = 0.5) {
    auto random_cut_partition = generate_random_cut(g, p_1);
    /*auto random_cut = fill_each_group(g, random_cut_partition);*/
    int cut_value_temp = 0;
    int cut_value = goal_factory(g.getEdges(), random_cut_partition);

    for (int i = 0; i < iterations; i++) {

        auto random_cuts_partiton = generate_random_cut(g, p_1);
        /*auto random_cuts = fill_each_group(g, random_cuts_partiton);*/

        cut_value_temp = goal_factory(g.getEdges(), random_cuts_partiton);

        if (cut_value_temp >= cut_value) {
            cut_value = cut_value_temp;
            random_cut_partition = random_cuts_partiton;
        }
    }


    /*printDetailsAboutCut(random_cut, cut_value);*/

    return random_cut_partition;
}

int main() {

    std::fstream file("/Users/dawid/ClionProjects/maxcut/edges.txt");

    string line;
    int count = 0;
    int foo = 0;
    int v = 0;

    vector<vector<int>> temp(1);

    if (file.is_open()) {
        while (getline(file, line)) {
            if (count % 2 == 0 && count != 0) {
                foo++;
                temp.push_back(vector<int>());
            }
            temp[foo].push_back(atoi(line.c_str()));
            if (v < atoi(line.c_str())) {
                v = atoi(line.c_str());
            }
            count++;
        }
        file.close();
    } else {
        cerr << "Unable to open file!\n";
        return 1;
    }

    Graph G(v + 1);

    count = 0;

    for (const auto &first: temp) {
        int u = 0;
        int v = 0;

        if (first.size() >= 2) {
            u = first[0];
            v = first[1];

            G.addEdge(u, v);
        }
    }

    vector<int> bar = solve_random(G, 100);

    printDetailsAboutCut(G, bar);

    bar = generate_neighbours_cut(G, bar);

    printDetailsAboutCut(G, bar);


    return 0;
}
