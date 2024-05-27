#include <iostream>
#include <list>
#include <random>


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

std::vector<char> solve_simulation_annealing(G &graph, int iterations);

std::vector<char> solve_tabu(const G &g, int iterations, int tabu_size);

int main() {
    std::cout << "\nConnections in first graph \n";
    G g1(5);
    g1.addEd(1, 2);
    g1.addEd(3, 2);
    g1.addEd(2, 1);
    g1.addEd(0, 1);
    g1.addEd(1, 4);
    g1.Con();


    std::cout << "\n Generate random solution \n";
    auto random = generate_random_solution(g1, 0.5);


    for (auto e: random) {
        std::cout << (int) e << " ";
    }

    std::cout << "\n";

    std::cout << "\n Generate neighbour solution \n";
    auto neighbour = generate_neighbours_solution(random);

    for (auto &e: neighbour) {
        for (auto f: e) {
            std::cout << (int) f << " ";
        }
    }

    std::cout << "\n";

    std::cout << "\n Generate hill-climbing solution \n";
    auto hc = hill_climbing_solution(g1, 20);


    for (auto e: hc) {
        std::cout << (int) e << " ";
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
                                             std::function<double(int)> T = [](auto i) { return 1.0 / (i + 1); }) {
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

std::vector<char> solve_tabu(const G &g, int iterations, int tabu_size) {
    
}