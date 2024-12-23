#include <iostream>
#include <fstream>
#include <deque>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <thread>

using namespace std;

std::vector<std::string> split(const std::string &sentence, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(sentence);

    while (std::getline(tokenStream, token, delimiter)) {
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

//unordered_set<string> find_all_connected_vertices(
//    const string &start,
//    const unordered_map<string, unordered_set<string>> &nmap
//) {
//    unordered_set<string> visited;
//    visited.insert(start);
//
//    queue<string> q;
//    q.push(start);
//
//    while (!q.empty()) {
//        auto current = q.front();
//        q.pop();
//
//        auto it = nmap.find(current);
//        if (it == nmap.end()) {
//            continue;
//        }
//        const auto& neighbours = it->second;
//        for (const auto& neighbour : neighbours) {
//            if (visited.find(neighbour) == visited.end()) {
//                visited.insert(neighbour);
//                q.push(neighbour);
//            }
//        }
//    }
//
//    return visited;
//}

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
    for (const auto& it : tmp) {
        ss << it <<",";
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

void part_1() {
    auto all_connections = read_connections("23/data");
    auto all_vertices = get_all_vertices(all_connections);
    auto neighbours_map = create_neighbours_map(all_connections);
    auto t_vertices = get_t_started_vertices(all_vertices);

    unordered_set<string> results;
    for (const auto &vertex: t_vertices) {
//        cout << "vertex : "<< vertex << endl;
        auto graphs = get_all_three_inner_connect_graphs_from(vertex, neighbours_map);
        for (const auto& graph: graphs) {
            results.insert(graph);
        }
    }

    cout << results.size() << endl;
}

int main() {
    part_1();
    return 0;
}