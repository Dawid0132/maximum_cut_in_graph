#include <iostream>
#include <list>
#include <random>
#include <set>
#include <algorithm>
#include "memory"


#define N - 1;
std::random_device rd;
std::mt19937 rdgen(rd());


class G {
    int n;
    std::vector<std::vector<int>> adj;

    void connections(int n, bool visited[], int disc[], int low[], int par[]);

public:
    G(int n);

    void addEd(int w, int x);

    void Con();

    const std::vector<std::vector<int>> &getAdj() const;

    int getNumVertices() const;

};

G::G(int n) : n(n), adj(n) {}

void G::addEd(int w, int x) {
    adj[x].push_back(w);
    adj[w].push_back(x);
}

void G::connections(int n, bool *visited, int *disc, int *low, int *par) {
    static int t = 0;
    visited[n] = true;
    disc[n] = low[n] = ++t;
    std::vector<int>::iterator i;
    for (i = adj[n].begin(); i != adj[n].end(); ++i) {
        int x = *i;
        if (!visited[x]) {
            par[x] = n;
            connections(x, visited, disc, low, par);
            low[n] = std::min(low[n], low[x]);
            if (low[x] > disc[n]) {
                std::cout << n << " " << x << "\n";
            }
        } else if (x != par[n]) {
            low[n] = std::min(low[n], disc[x]);
        }
    }
}

void G::Con() {
    bool *visited = new bool[n];
    int *dis = new int[n];
    int *low = new int[n];
    int *par = new int[n];
    for (int i = 0; i < n; i++) {
        par[i] = N;
        visited[i] = false;
    }

    for (int i = 0; i < n; i++) {
        if (visited[i] == false) {
            connections(i, visited, dis, low, par);
        }
    }

    delete[] visited;
    delete[] dis;
    delete[] low;
    delete[] par;
}

const std::vector<std::vector<int>> &G::getAdj() const {
    return adj;
}

int G::getNumVertices() const {
    return n;
}

std::function<int(const std::vector<char> &)> goal_factory(G &graph) {
    return [=](const std::vector<char> &partition) {

        if (partition.size() != graph.getNumVertices()) {
            throw std::invalid_argument("Bad partition size!");
        }

        double cut_value = 0;

        for (int i = 0; i < graph.getNumVertices(); i++) {
            for (int j: graph.getAdj()[i]) {
                if (partition[i] != partition[j]) {
                    cut_value += 1;
                }
            }
        }

        return cut_value / 2;
    };
}

std::vector<char> generate_random_solution(G &graph, double p_1);

std::vector<std::vector<char>> generate_neighbours_solution(std::vector<char> &partition);

std::vector<char> hill_climbing_solution(G &graph, int iterations);

std::vector<char> generate_random_neighbour_norm(std::vector<char> partition);

std::vector<char> solve_simulation_annealing(G &graph, int iterations,
                                             std::function<double(int)> T = [](auto i) { return 1.0 / (i + 1); });

std::vector<char> solve_tabu(G &g, int iterations, int tabu_size);

std::vector<char> solve_tabu_list(G &g, int iterations, int tabu_size);

char mutated_genes();

std::vector<char> create_gnome() {
    std::vector<char> target = {'1', '1', '1', '1', '1'};
    int len = target.size();
    std::vector<char> gnome;
    for (int i = 0; i < len; i++) {
        gnome.push_back(mutated_genes());
    }
    return gnome;
}

class Individual {
public:
    std::vector<char> chromosome;
    int fitness;

    Individual(std::vector<char> chromosome);

    Individual mate(Individual parent2);

    int cal_fitness();
};

Individual::Individual(std::vector<char> chromosome) {
    this->chromosome = chromosome;
    fitness = cal_fitness();
}

Individual Individual::mate(Individual parent2) {
    std::vector<char> child_chromosome = {};
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    int len = chromosome.size();

    for (int i = 0; i < len; i++) {
        float p = (dist(rdgen));
        if (p < 0.45) {
            child_chromosome.push_back(chromosome.at(i));
        } else if (p < 0.90) {
            child_chromosome.push_back(parent2.chromosome.at(i));
        } else {
            child_chromosome.push_back(mutated_genes());
        }
    }
    return Individual(child_chromosome);
}

int Individual::cal_fitness() {
    std::vector<char> target = {'1', '1', '1', '1', '1'};
    int len = target.size();
    int fitness = 0;

    for (int i = 0; i < len; i++) {
        if (chromosome.at(i) != target[i]) {
            fitness++;
        }
    }
    return fitness;
}

bool operator<(const Individual &ind1, const Individual &ind2) {
    return ind1.fitness < ind2.fitness;
}

void genetic_algorithm();

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Musisz podać conajmniej jeden argument\n";
        return 1;
    }

    std::vector<std::string> methods;
    for (int i = 1; i < argc; ++i) {
        methods.push_back(argv[i]);
    }

    G g1(5);
    g1.addEd(1, 2);
    g1.addEd(3, 2);
    g1.addEd(2, 1);
    g1.addEd(0, 1);
    g1.addEd(1, 4);

    for (auto &method: methods) {
        if (method == "Con") {
            std::cout << "\nConnections in first graph\n";
            g1.Con();
        } else if (method == "random") {
            std::cout << "\n Generate random solution \n";
            auto random = generate_random_solution(g1, 0.5);
            for (auto e: random) {
                std::cout << (int) e << " ";
            }
            std::cout << "\n";
        } else if (method == "neighbour") {
            std::cout << "\n Generate neighbour solution \n";
            auto random = generate_random_solution(g1, 0.5);
            auto neighbour = generate_neighbours_solution(random);
            for (auto &e: neighbour) {
                for (auto f: e) {
                    std::cout << (int) f << " ";
                }
            }
            std::cout << "\n";
        } else if (method == "tabu") {
            std::cout << "\n Generate tabu solution \n";
            auto t = solve_tabu(g1, 20, 1000);
            for (auto e: t) {
                std::cout << (int) e << " ";
            }
            std::cout << "\n";
        } else if (method == "tabu_list") {
            std::cout << "\n Generate tabu_list solution \n";
            auto ts = solve_tabu_list(g1, 20, 1000);
            for (auto e: ts) {
                std::cout << (int) e << " ";
            }
            std::cout << "\n";
        } else if (method == "hill_climbing") {
            std::cout << "\n Generate hill-climbing solution \n";
            auto hc = hill_climbing_solution(g1, 20);
            for (auto e: hc) {
                std::cout << (int) e << " ";
            }
            std::cout << "\n";
        } else if (method == "genetic") {
            std::cout << "\n Generate genetic solution \n";
            genetic_algorithm();
        } else {
            std::cerr << "Unknown method: " << method << "\n";
        }
    }
    


    return 0;
}

std::vector<char> generate_random_solution(G &graph, double p_1 = 0.5) {
    std::vector<char> random(graph.getNumVertices());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (auto &e: random) {
        e = (dist(rdgen) < p_1) ? 1 : 0;
    }
    return random;
}

std::vector<std::vector<char>> generate_neighbours_solution(std::vector<char> &partition) {
    std::vector<std::vector<char>> result;

    for (int i = 0; i < partition.size(); i++) {
        auto modified = partition;
        modified[i] = 1 - modified[i];
        result.push_back(modified);
    }
    return result;
}

std::vector<char> hill_climbing_solution(G &graph, int iterations) {
    auto random = generate_random_solution(graph, 0.5);
    auto goal = goal_factory(graph);

    for (int i = 0; i < iterations; ++i) {
        auto neighbour = generate_neighbours_solution(random);
        auto max_elements = *std::max_element(neighbour.begin(), neighbour.end(), [=](auto a, auto b) {
            return goal(a) < goal(b);
        });


        if (goal(max_elements) >= goal(random)) {
            random = max_elements;
        } else {
            break;
        }
    }
    return random;
}

std::vector<char> generate_random_neighbour_norm(std::vector<char> partition) {
    std::normal_distribution<int> distr;

    int count = distr(rdgen) + 1;

    if (count >= (partition.size()) * 2) {
        count = partition.size();
    }

    for (int i = 0; i < count; i++) {
        std::uniform_int_distribution<int> selbit(0, partition.size() - 1);
        int sel = selbit(rdgen);
        partition[sel] = 1 - partition[sel];
    }
    return partition;
}

std::vector<char> solve_simulation_annealing(G &graph, int iterations = 20,
                                             std::function<double(int)> T) {
    auto random = generate_random_solution(graph);
    auto best_solution = random;
    auto goal = goal_factory(graph);


    for (int i = 0; i < iterations; i++) {
        auto t = generate_random_neighbour_norm(random);
        if (goal(t) >= goal(random)) {
            random = t;
            if (goal(random) >= goal(best_solution)) {
                best_solution = random;
            }
        } else {
            std::uniform_real_distribution<double> u(0.0, 1.0);
            if (u(rdgen) < exp((abs(goal(t) - goal(random)) / T(i)))) {
                random = t;
            }
        }
    }

    return best_solution;
}

std::vector<char> solve_tabu(G &g, int iterations = 20, int tabu_size = 1000) {
    auto random = generate_random_solution(g);
    auto best_solution = random;
    auto goal = goal_factory(g);


    std::list<std::vector<char>> tabu_list = {random};
    std::set<std::vector<char>> tabu = {random};

    auto is_in_tabu = [&](std::vector<char> &elem) -> bool {
        return tabu.count(elem) > 0;
    };

    for (int i = 0; i < iterations; i++) {
        std::vector<std::vector<char>> partitions;
        for (auto p: generate_neighbours_solution(random)) {
            if (!is_in_tabu(p)) {
                partitions.push_back(p);
            }
        }

        auto random_next = *std::max_element(partitions.begin(), partitions.end(), [=](auto a, auto b) {
            return goal(a) < goal(b);
        });

        random = random_next;

        if (goal(random) >= goal(best_solution)) {
            best_solution = random;
        }
        tabu_list.push_back(random);
        tabu.insert(random);
        if (tabu.size() > tabu_size) {
            tabu.erase(tabu_list.front());
            tabu_list.pop_front();
        }

    }
    return best_solution;
}

std::vector<char> solve_tabu_list(G &g, int iterations, int tabu_size) {
    auto random = generate_random_solution(g);
    auto best_solution = random;
    auto goal = goal_factory(g);


    std::list<std::vector<char>> tabu = {random};

    auto is_in_tabu = [&](std::vector<char> &e) -> bool {
        return std::find(tabu.begin(), tabu.end(), e) != tabu.end();
    };

    for (int i = 0; i < iterations; i++) {
        std::vector<std::vector<char>> partitions;
        for (auto p: generate_neighbours_solution(random)) {
            if (!is_in_tabu(p)) {
                partitions.push_back(p);
            }
        }

        auto random_next = *std::max_element(partitions.begin(), partitions.end(), [=](auto a, auto b) {
            return goal(a) < goal(b);
        });

        random = random_next;

        if (goal(random) >= goal(best_solution)) {
            best_solution = random;
        }
        tabu.push_back(random);

        if (tabu.size() > tabu_size) {
            tabu.pop_front();
        }

    }
    return best_solution;
}

char mutated_genes() {
    std::uniform_int_distribution<int> dist(0, 1);
    return dist(rdgen) == 0 ? '0' : '1';
}

void genetic_algorithm() {
    int generation = 0;
    const int POPULATION_SIZE = 100;

    std::vector<Individual> population;
    bool found = false;

    std::uniform_int_distribution<int> dist(0, 50);

    for (int i = 0; i < POPULATION_SIZE; i++) {
        std::vector<char> gnome = create_gnome();
        population.push_back(Individual(gnome));
    }

    while (!found) {
        std::sort(population.begin(), population.end());

        if (population[0].fitness <= 0) {
            found = true;
            break;
        }

        std::vector<Individual> new_generation;

        int s = (10 * POPULATION_SIZE) / 100;

        for (int i = 0; i < s; i++) {
            new_generation.push_back(population[i]);
        }

        s = (90 * POPULATION_SIZE) / 100;

        for (int i = 0; i < s; i++) {
            int len = population.size();
            int r = dist(rdgen);
            Individual parent1 = population[r];
            r = dist(rdgen);
            Individual parent2 = population[r];
            Individual offspring = parent1.mate(parent2);
            new_generation.push_back(offspring);
        }
        population = new_generation;
        std::cout << "Generation: " << generation << "\t";
        std::cout << "Best chromosome : " << "\t";
        for (auto e: population[0].chromosome) {
            std::cout << e << " ";
        }
        std::cout << "Fitness: " << population[0].fitness << "\n";

        generation++;


    }

    std::cout << "Generation: " << generation << "\t";
    std::cout << "Target found: " << "\t";
    for (auto e: population[0].chromosome) {
        std::cout << e << " ";
    }
    std::cout << "Fitness: " << population[0].fitness << "\n";

}