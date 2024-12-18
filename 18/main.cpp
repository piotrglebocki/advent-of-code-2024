#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

const int AREA_SIZE = 71;

const vector<tuple<int, int>> dirs {
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0}
};

using Pos = tuple<int, int>;

struct CellData {
    Pos pos;
    int dist;
};

std::vector<std::string> split(const std::string &sentence, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(sentence);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<Pos> read_data(const string& filename) {
    vector<Pos> out;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
    }

    string line;
    while (getline(file, line)) {
        istringstream lineStream(line);
        auto numbers = split(line, ',');
        out.emplace_back(stoi(numbers[1]), stoi(numbers[0]));
    }

    file.close();
    return out;
}

void print(const vector<vector<char>> &matrix) {
    for (const auto &row: matrix) {
        for (auto it: row) {
            cout << it;
        }
        cout << endl;
    }
}

bool compare(const CellData &a, const CellData &b) {
    return a.dist > b.dist;
}

vector<Pos> get_neighbours(const vector<vector<char>>& matrix, const Pos& pos) {
    vector<Pos> neighbours;
    const auto& [r,c] = pos;

    for (const auto& [step_r, step_c] : dirs) {
        auto new_r = r + step_r;
        auto new_c = c + step_c;

        if (0 <= new_r && new_r < AREA_SIZE && 0 <= new_c && new_c < AREA_SIZE && matrix[new_r][ new_c] != '#') {
            neighbours.emplace_back(new_r, new_c);
        }
    }
    return neighbours;
}

int find_shortest_path_len(const vector<vector<char>>& matrix, const Pos& start, const Pos& end) {
    vector<vector<int>> distances(AREA_SIZE, vector(AREA_SIZE, INT_MAX));
    vector<vector<Pos>> previous(AREA_SIZE, vector(AREA_SIZE, Pos(-1,-1)));
    priority_queue<CellData, vector<CellData>, decltype(&compare)> pq(compare);

    distances[0][0] = 0;
    pq.push(CellData{start, 0});

    while (!pq.empty()) {
        const auto current = pq.top();
        pq.pop();
        const auto& [current_r, current_c] = current.pos;

        if (current.pos == end) {
            break;
        }

        auto neighbours = get_neighbours(matrix, current.pos);
        for (const auto& neighbour: neighbours) {
            const auto& [n_r, n_c] = neighbour;
            auto new_dist = distances[current_r][current_c] + 1;
            if (new_dist < distances[n_r][n_c]) {
                distances[n_r][n_c] = new_dist;
                previous[n_r][n_c] = current.pos;
                pq.push(CellData{neighbour, new_dist});
            }
        }
    }
    return distances[AREA_SIZE - 1][AREA_SIZE - 1];
}

int main() {
    auto data = read_data("18/data.txt");
    vector<vector<char>> matrix(AREA_SIZE, vector(AREA_SIZE, '.'));
    int b = 0;
    for (const auto& [r, c] : data) {
        if (b++ == 1024) break;
        matrix[r][c] = '#';
    }

//    print(matrix);
    Pos start(0, 0);
    Pos end(AREA_SIZE - 1, AREA_SIZE - 1);
    cout << find_shortest_path_len(matrix, start, end);
    return 0;
}