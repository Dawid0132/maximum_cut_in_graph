#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <fstream>

using namespace std;

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

int goal_factory(vector<pair<int, int>> e, vector<int> V1, vector<int> V2) {
    int cut_value = 0;

    for (auto first: V1) {
        for (auto second: V2) {
            for (auto edges: e) {
                if (first == edges.first && second == edges.second ||
                    first == edges.second && second == edges.first) {
                    cut_value++;
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

    int maximumCut() {

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
    }
};

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

            cout << u << "\n";
            cout << v << "\n";
            G.addEdge(u, v);
        }
    }

    /*Graph G(6);
    G.addEdge(0, 1);
    G.addEdge(0, 2);
    G.addEdge(1, 3);
    G.addEdge(2, 4);
    G.addEdge(3, 4);
    G.addEdge(3, 5);
    G.addEdge(4, 5);
*/

    int max_cut = G.maximumCut();

    cout << "Waga maksymalnego przecięcia: " << max_cut << endl;

    return 0;
}
