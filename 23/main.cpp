#include <iostream>
#include <fstream>
#include <deque>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <thread>

using namespace std;

vector<string> split(const string &sentence, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(sentence);

    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<pair<string, string>> read_connections(const string &filename) {
    vector<pair<string, string>> output;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
    }

    string line;
    while (getline(file, line)) {
        auto codes = split(line, '-');
        output.emplace_back(codes[0], codes[1]);
    }

    file.close();
    return output;
}

unordered_set<string> get_all_vertices(const vector<pair<string, string>> &connections) {
    unordered_set<string> vertices;
    for (const auto &connection: connections) {
        vertices.insert(connection.first);
        vertices.insert(connection.second);
    }
    return vertices;
}

unordered_map<string, unordered_set<string>> create_neighbours_map(
    const vector<pair<string, string>> &connections
) {
    unordered_map<string, unordered_set<string>> nmap;
    for (const auto &connection: connections) {
        const auto &[v1, v2] = connection;

        auto it1 = nmap.find(v1);
        if (it1 == nmap.end()) {
            nmap.insert({v1, {v2}});
        } else {
            it1->second.insert(v2);
        }

        auto it2 = nmap.find(v2);
        if (it2 == nmap.end()) {
            nmap.insert({v2, {v1}});
        } else {
            it2->second.insert(v1);
        }
    }
    return nmap;
}

vector<string> get_t_started_vertices(const unordered_set<string> &vertices) {
    vector<string> t_vertices;
    for (const auto &it: vertices) {
        if (it[0] == 't') {
            t_vertices.push_back(it);
        }
    }
    return t_vertices;
}

bool are_connected(
    const string &v1,
    const string &v2,
    const unordered_map<string, unordered_set<string>> &nmap
) {
    auto it = nmap.find(v1);
    if (it == nmap.end()) return false;
    return it->second.find(v2) != it->second.end();
}

string create_sorted(
    const string &v1,
    const string &v2,
    const string &v3
) {
    vector tmp = {v1, v2, v3};
    sort(tmp.begin(), tmp.end());
    stringstream ss;
    for (const auto &it: tmp) {
        ss << it << ",";
    }
    return ss.str();
}

string create_sorted(
    unordered_set<string> vertices
) {
    vector tmp(vertices.begin(), vertices.end());
    sort(tmp.begin(), tmp.end());
    stringstream ss;
    for (const auto &it: tmp) {
        ss << it << ",";
    }
    return ss.str();
}

vector<string> get_all_three_inner_connect_graphs_from(
    const string &vertex,
    const unordered_map<string, unordered_set<string>> &nmap
) {
    auto it = nmap.find(vertex);
    if (it == nmap.end()) {
        return {};
    }

    vector<string> output;

    const auto &neighbours = it->second;
    vector<string> neighbours_list(neighbours.begin(), neighbours.end());
    const auto size = neighbours.size();

    for (int i = 0; i < size - 1; i++) {
        const auto v1 = neighbours_list[i];
        for (int j = i; j < size; j++) {
            const auto v2 = neighbours_list[j];
            if (are_connected(v1, v2, nmap)) {
                output.emplace_back(create_sorted(vertex, v1, v2));
            }
        }
    }
    return output;
}

vector<vector<string>> generate_power_set(const vector<string> &input) {
    vector<vector<string>> power_set;
    int n = input.size();
    int subset_count = (1 << n);

    for (int mask = 0; mask < subset_count; ++mask) {
        vector<string> subset;
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                subset.push_back(input[i]);
            }
        }
        power_set.push_back(subset);
    }

    reverse(power_set.begin(), power_set.end());
    return power_set;
}

bool is_complete_graph(
    const vector<string> &vertices,
    const unordered_map<string, unordered_set<string>> &nmap
) {
    const auto size = vertices.size();
    for (int i = 0; i < size - 1; i++) {
        const auto v1 = vertices[i];
        for (int j = i + 1; j < size; j++) {
            const auto v2 = vertices[j];
            if (!are_connected(v1, v2, nmap)) {
                return false;
            }
        }
    }
    return true;
}

string get_biggest_complete_graph_from(
    const string &vertex,
    const unordered_map<string, unordered_set<string>> &nmap
) {
    auto it = nmap.find(vertex);
    if (it == nmap.end()) {
        return {};
    }

    const auto &neighbours = it->second;
    vector<string> neighbours_list(neighbours.begin(), neighbours.end());

    auto all_possible_sets = generate_power_set(neighbours_list);
    for (const auto &set: all_possible_sets) {
        if (is_complete_graph(set, nmap)) {
            unordered_set<string> res;
            res.insert(vertex);
            res.insert(set.begin(), set.end());
            return create_sorted(res);
        }
    }

    return "";
}

void part_1() {
    auto all_connections = read_connections("23/data");
    auto all_vertices = get_all_vertices(all_connections);
    auto neighbours_map = create_neighbours_map(all_connections);
    auto t_vertices = get_t_started_vertices(all_vertices);

    unordered_set<string> results;
    for (const auto &vertex: t_vertices) {
//        cout << "vertex : "<< vertex << endl;
//        auto graphs = get_bigger_inner_connect_graph_from(vertex, neighbours_map);
//        cout << graphs << endl;
    }

    cout << results.size() << endl;
}

void part_2() {
    auto all_connections = read_connections("23/data");
    auto all_vertices = get_all_vertices(all_connections);
    auto neighbours_map = create_neighbours_map(all_connections);

    vector<string> results;
    const auto size = all_vertices.size();
    auto index = 0;
    for (const auto &vertex: all_vertices) {
        cout << "processing vertex : " << ++index << "/" << size << endl;
        auto graph = get_biggest_complete_graph_from(vertex, neighbours_map);
        results.push_back(graph);
    }

    sort(results.begin(), results.end(), [](const string &a, const string &b) {
        return b.length() < a.length();
    });

    cout << results.front() << endl;
}

int main() {
    part_2();
    return 0;
}