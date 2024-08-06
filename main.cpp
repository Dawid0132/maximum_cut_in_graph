#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <fstream>
#include <random>
#include <list>
#include <set>

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

function<int(const vector<int> &)> goal_factory(const vector<pair<int, int>> &edges) {
    return [=](const vector<int> &cut) {
        int cut_value = 0;

        for (int i = 0; i < cut.size(); ++i) {
            for (int j = 0; j < cut.size(); ++j) {
                if (cut[i] == 0 && cut[j] == 1) {
                    for (const auto &e: edges) {
                        if (i == e.first && j == e.second || i == e.second && j == e.first) {
                            cut_value++;
                        }
                    }
                }
            }
        }


        return cut_value;
    };
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
        auto goal = goal_factory(edges);

        vector<int> result;

        int max_cut_weight = 0;

        for (auto partition: partitions) {
            int cut = goal(partition);

            if (max_cut_weight < cut) {
                max_cut_weight = cut;
                result = partition;
            }
        }

        return result;
    }

    int getVertices() {
        return V;
    }
};

void printDetailsAboutCut(Graph g, vector<int> partition) {

    auto goal = goal_factory(g.getEdges());

    auto max_cut = goal(partition);

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

vector<vector<int>> generate_neighbours_cut(Graph g, vector<int> partition) {

    vector<vector<int>> neighbour_partition;


    for (int i = 0; i < partition.size(); ++i) {
        auto modified = partition;
        modified[i] = 1 - modified[i];
        neighbour_partition.push_back(modified);
    }

    return neighbour_partition;
}

vector<int> solve_random(Graph g, int iterations = 20, double p_1 = 0.5) {

    auto goal = goal_factory(g.getEdges());
    auto random_cut_partition = generate_random_cut(g, p_1);
    int cut_value_temp = 0;
    int cut_value = goal(random_cut_partition);

    for (int i = 0; i < iterations; i++) {

        auto random_cuts_partition = generate_random_cut(g, p_1);

        cut_value_temp = goal(random_cuts_partition);

        if (cut_value_temp >= cut_value) {
            cut_value = cut_value_temp;
            random_cut_partition = random_cuts_partition;
        }
    }

    return random_cut_partition;
}

vector<int> generate_next_solution(vector<int> partition) {
    for (int i = 0; i < partition.size(); ++i) {
        if (partition[i] == 0) {
            partition[i] = 1;
            return partition;
        }
        partition[i] = 0;
    }
    return partition;
}

vector<int> generate_first_cut(Graph g) {
    vector<int> first(g.getVertices());
    return first;
}

vector<int> solve(Graph g) {

    auto goal = goal_factory(g.getEdges());
    auto cut = generate_first_cut(g.getVertices());

    auto best_goal_value = goal(cut);

    auto best_solution = cut;

    while (true) {
        cut = generate_next_solution(cut);

        int next_goal_value = goal(cut);

        if (next_goal_value > best_goal_value) {
            best_goal_value = next_goal_value;
            best_solution = cut;
        }
        int s = 0;

        for (auto e: cut) {
            s += e;
        }

        if (s == 0) break;
    }
    return best_solution;
}

vector<int> solve_hill_climbing(Graph g, int iterations = 20) {
    auto cut = generate_random_cut(g);
    auto goal = goal_factory(g.getEdges());

    for (int i = 0; i < iterations; ++i) {
        auto cuts = generate_neighbours_cut(g, cut);
        auto next_cut = *max_element(cuts.begin(), cuts.end(), [=](auto a, auto b) {
            return goal(a) < goal(b);
        });

        if (goal(next_cut) > goal(cut)) {
            cut = next_cut;
        } else {
            break;
        }

    }

    return cut;
}

vector<int> solve_tabu(const Graph &g, int iterations = 20, int tabu_size = 1000) {
    auto cut = generate_random_cut(g);
    auto best_cut = cut;
    auto goal = goal_factory(g.getEdges());

    std::list<vector<int>> tabu_list = {cut};
    std::set<vector<int>> tabu = {cut};


    auto is_in_tabu = [&](vector<int> &e) -> bool {
        return tabu.count(e) > 0;
    };

    for (int i = 0; i < iterations; ++i) {

        vector<vector<int>> cuts;

        for (auto c: generate_neighbours_cut(g, cut)) {
            if (!is_in_tabu(c))
                cuts.push_back(c);
        }

        auto cut_next = *std::max_element(cuts.begin(), cuts.end(), [=](auto a, auto b) {
            return goal(a) < goal(b);
        });

        cut = cut_next;

        if (goal(cut) >= goal(best_cut)) {
            best_cut = cut;
        }

        tabu_list.push_back(cut);
        tabu.insert(cut);
        if (tabu.size() > tabu_size) {
            tabu.erase(tabu_list.front());
            tabu_list.pop_front();
        }

    }
    return best_cut;
}

vector<int> generate_random_neighbour_norm(vector<int> partition) {
    /*std::normal_distribution<double> distr;*/
    uniform_real_distribution<double> dist(0.0, 1.0);

    int count = dist(rdgen) + 1;
    /*int count = distr(rdgen) + 1;*/

    if (count >= (partition.size() * 2))
        count = partition.size();

    for (int i = 0; i < count; ++i) {
        std::uniform_int_distribution<int> selbit(0, partition.size() - 1);
        int sel = selbit(rdgen);
        partition[sel] = 1 - partition[sel];
    }

    return partition;
}

vector<int> solve_sim_annealing(const Graph g, int iterations = 20,
                                std::function<double(int)> T = [](auto i) { return 1 / (i + 1); }) {

    auto cut_partition = generate_random_cut(g);
    auto best_cut = cut_partition;
    auto goal = goal_factory(g.getEdges());

    for (int i = 0; i < iterations; ++i) {
        auto t = generate_random_neighbour_norm(cut_partition);

        if (goal(t) >= goal(cut_partition)) {
            cut_partition = t;
            if (goal(cut_partition) >= goal(best_cut)) {
                best_cut = cut_partition;
            }
        } else {
            std::uniform_real_distribution<double> u(0.0, 1.0);
            if (u(rdgen) < exp((abs(goal(t) - goal(cut_partition)) / T(i)))) {
                cut_partition = t;
            }
        }
    }
    return best_cut;
}

vector<int> fit(const vector<vector<int>> &population, std::function<int(const vector<int> &)> &fitness) {
    vector<int> ret;
    for (auto &e: population) {
        ret.push_back(fitness(e));
    }
    return ret;
}

vector<int> selection(const vector<int> &fitnesses) {
    std::uniform_int_distribution<int> u(0, fitnesses.size() - 1);
    vector<int> result;

    for (int i = 0; i < fitnesses.size(); ++i) {
        int idx1 = u(rdgen);
        int idx2 = u(rdgen);

        if (fitnesses.at(idx1) > fitnesses.at(idx2))
            result.push_back(idx1);
        else
            result.push_back(idx2);
    }

    return result;
}

vector<vector<int>> crossover_one_point(const vector<vector<int>> &parents) {
    vector<vector<int>> children(2);
    std::uniform_int_distribution<int> u(0, parents[0].size() - 1);

    int cross_point = u(rdgen);

    for (int i = 0; i < parents[0].size(); ++i) {
        if (i < cross_point) {
            children[0].push_back(parents[0][i]);
            children[1].push_back(parents[1][i]);
        } else {
            children[0].push_back(parents[1][i]);
            children[1].push_back(parents[0][i]);
        }
    }
    return children;
}

vector<vector<int>> crossover(const vector<vector<int>> &population, vector<int> parents, double prob_crossover) {
    vector<vector<int>> ret;
    std::uniform_real_distribution<double> u(0.0, 1.0);

    for (int i = 0; i < population.size(); i += 2) {
        vector<int> e = population[parents[i]];
        vector<int> f = population[parents[i + 1]];

        vector<vector<int>> parents = {e, f};

        vector<vector<int>> children;

        if (u(rdgen) < prob_crossover)
            children = crossover_one_point(parents);
        else
            children = parents;

        for (auto &o: children)
            ret.push_back(o);


    }
    return ret;
}

vector<int> bit_flip_mutation(const vector<int> &p, double prob_mutation) {
    std::uniform_real_distribution<double> u(0.0, 1.0);
    auto ret = p;

    for (int i = 0; i < p.size(); ++i) {
        if (u(rdgen) < prob_mutation)
            ret[i] = 1 - ret[i];
    }

    return ret;
}

vector<int> inversion_mutation(const vector<int> &p, double prob_mutation) {

    std::uniform_real_distribution<double> u(0.0, 1.0);

    std::uniform_int_distribution<int> start_subset(0, p.size() - 1);
    int start = start_subset(rdgen);

    std::uniform_int_distribution<int> end_subset(start, p.size() - 1);
    int end = end_subset(rdgen);


    auto ret = p;

    if (u(rdgen) < prob_mutation) {
        for (int i = start; i < end; ++i) {
            int temp = ret[i];
            ret[i] = ret[end];
            ret[end] = temp;

            end--;
        }
    }

    return ret;
}

vector<vector<int>> mutation(const vector<vector<int>> &specimens, double prob_mutation, bool mutation_type) {
    std::uniform_real_distribution<double> u(0.0, 1.0);
    vector<vector<int>> ret;

    for (auto s: specimens)
        if (mutation_type) {
            ret.push_back(bit_flip_mutation(s, prob_mutation));
        } else {
            ret.push_back(inversion_mutation(s, prob_mutation));
        }


    return specimens;
}

vector<int>
solve_genetic_algorithm(const Graph g, int iterations = 20, int pop_size = 100, double prob_crossover = 0.9,
                        double prob_mutation = 0.01, bool conv_log = true, bool mutation_type = true) {

    vector<vector<int>> population;
    vector<int> best_cut;

    for (int i = 0; i < pop_size; ++i)
        population.push_back(generate_random_cut(g));

    auto goal = goal_factory(g.getEdges());

    function<int(const vector<int> &)> fitness = [goal](const vector<int> &specimen) {
        int g = goal(specimen);

        if (g < 0)
            return 0;

        return g;
    };

    auto get_best = [&]() {
        return *std::max_element(population.begin(), population.end(), [&fitness](auto &l, auto &r) {
            return fitness(l) < fitness(r);
        });
    };

    for (int i = 0; i < iterations; ++i) {
        vector<int> fitnesses = fit(population, fitness);
        vector<int> parents = selection(fitnesses);
        vector<vector<int>> offspring = crossover(population, parents, prob_crossover);
        offspring = mutation(offspring, prob_mutation, mutation_type);
        population = offspring;


        if (goal(best_cut) < fitness(get_best())) {
            best_cut = get_best();
        }
    }

    return best_cut;
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

    vector<int> bar = solve_genetic_algorithm(G, 1, 100, 0.9, 0.5);

    printDetailsAboutCut(G, bar);

    return 0;
}
