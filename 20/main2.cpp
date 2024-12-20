#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <thread>

using namespace std;

struct Pos {
    int r, c;

    bool operator==(const Pos& other) const {
        return r == other.r && c == other.c;
    }

    bool operator!=(const Pos& other) const {
        return !(*this == other);
    }

    Pos operator+(const Pos& other) const {
        return Pos{r + other.r, c + other.c};
    }

    Pos operator*(int scalar) const {
        return Pos{r * scalar, c * scalar};
    }
};

namespace std {
template <>
struct hash<Pos> {
    size_t operator()(const Pos& p) const {
        return hash<int>()(p.r) ^ (hash<int>()(p.c) << 1);
    }
};
}

int manhattan_distance(const Pos& a, const Pos& b) {
    return abs(a.r - b.r) + abs(a.c - b.c);
}

const vector<tuple<int, int>> Directions = {
    {0,  1},
    {0,  -1},
    {1,  0},
    {-1, 0}
};

struct QueueElement {
    int dist;
    Pos pos;
};

bool compare(const QueueElement &a, const QueueElement &b) {
    return a.dist > b.dist;
}

vector<vector<char>> read_matrix(const string &filename) {
    vector<vector<char>> matrix;
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open file " << filename << endl;
    }
    string line;
    while (getline(inputFile, line)) {
        vector<char> row(line.begin(), line.end());
        matrix.emplace_back(row);
    }
    inputFile.close();
    return matrix;
}

vector<Pos> get_neighbours(vector<vector<char>> &matrix, const Pos &pos) {
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();
    const auto &[pos_r, pos_c] = pos;

    vector<Pos> neighbours;
    for (const auto &[dr, dc]: Directions) {
        int r = pos_r + dr;
        int c = pos_c + dc;

        if (0 <= r && r < rows && 0 <= c && c < cols && matrix[r][c] != '#') {
            neighbours.push_back(Pos{r, c});
        }
    }
    return neighbours;
}

vector<Pos> find_path(vector<vector<char>> &matrix, const Pos &start, const Pos &end) {
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();
    auto [start_r, start_c] = start;
    auto [end_r, end_c] = end;

    priority_queue<QueueElement, vector<QueueElement>, decltype(&compare)> pq(compare);
    pq.push(QueueElement{0, start});

    vector<vector<int>> distance = vector(rows, vector(cols, INT_MAX));
    vector<vector<Pos>> previous = vector(rows, vector(cols, Pos{-1, -1}));
    distance[start_r][start_c] = 0;

    while (!pq.empty()) {
        auto element = pq.top();
        pq.pop();

        int current_dist = element.dist;
        Pos current_pos = element.pos;
        const auto &[current_r, current_c] = current_pos;

        if (current_pos == end) {
            break;
        }

        auto neighbours = get_neighbours(matrix, current_pos);
        for (const auto &neighbour: neighbours) {
            const auto &[r, c] = neighbour;

            int new_dist = current_dist + 1;
            if (new_dist < distance[r][c]) {
                distance[r][c] = new_dist;
                previous[r][c] = current_pos;
                pq.push(QueueElement{new_dist, neighbour});
            }
        }
    }

    if (distance[end_r][end_c] == INT_MAX) {
        return {};
    }

    vector<Pos> path;
    Pos current = end;
    while (current != Pos{-1, -1}) {
        path.push_back(current);
        current = previous[current.r][current.c];
    }
    reverse(path.begin(), path.end());
    return path;
}

Pos find_char(const vector<vector<char>> &matrix, const char target) {
    for (int r = 0; r < matrix.size(); r++) {
        for (int c = 0; c < matrix[0].size(); c++) {
            if (matrix[r][c] == target) {
                return Pos{r, c};
            }
        }
    }
    assert(false); // not found
}

void print(const vector<vector<char>> &matrix) {
    for (const auto &row: matrix) {
        for (auto it: row) {
            cout << it;
        }
        cout << endl;
    }
}

void benchmark(std::function<void()> operation) {
    auto start_time = std::chrono::high_resolution_clock::now();
    operation();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Elapsed time: " << duration.count() << " ms" << std::endl;
}

int main() {
    benchmark([]() {
        auto matrix = read_matrix("20/data.txt");
        auto start = find_char(matrix, 'S');
        auto end = find_char(matrix, 'E');

        auto path = find_path(matrix, start, end);
        unordered_map<Pos, int> pos_to_steps;
        int steps = 0;
        for (auto it : path) {
            pos_to_steps.insert({it, ++steps});
        }

        int count = 0;
        for (const auto& [p1,step1] : pos_to_steps) {
            for (const auto& [p2, steps2] : pos_to_steps) {
                int dist = manhattan_distance(p1, p2);
                if (dist <= 20 && step1 - steps2 - dist >= 100) {
                    count++;
                }
            }
        }
        cout << count << endl;

    });

    return 0;
}