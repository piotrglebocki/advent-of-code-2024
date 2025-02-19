#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

using Position = std::tuple<int, int>;
using Cost = vector<vector<vector<int>>>;
using Previous = vector<vector<Position>>;

enum class Direction {
    RIGHT = 0,
    LEFT = 1,
    UP = 2,
    DOWN = 3
};

struct CellData {
    int cost;
    int row;
    int col;
    Direction dir;
    vector<Position> path;
};

std::tuple<int, int> get_step(Direction dir) {
    switch (dir) {
        case Direction::LEFT:
            return make_tuple(0, -1);
        case Direction::RIGHT:
            return make_tuple(0, 1);
        case Direction::UP:
            return make_tuple(-1, 0);
        case Direction::DOWN:
            return make_tuple(1, 0);
    }
}

Direction rotate_cw(Direction dir) {
    switch (dir) {
        case Direction::UP:
            return Direction::RIGHT;
        case Direction::RIGHT:
            return Direction::DOWN;
        case Direction::DOWN:
            return Direction::LEFT;
        case Direction::LEFT:
            return Direction::UP;
    }
}

Direction rotate_ccw(Direction dir) {
    switch (dir) {
        case Direction::UP:
            return Direction::LEFT;
        case Direction::RIGHT:
            return Direction::UP;
        case Direction::DOWN:
            return Direction::RIGHT;
        case Direction::LEFT:
            return Direction::DOWN;
    }
}

vector<vector<char>> read_field(const string &filename) {
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

std::tuple<int, int> find_start(const vector<vector<char>> &matrix) {
    for (int r = 0; r < matrix.size(); r++) {
        for (int c = 0; c < matrix[0].size(); c++) {
            if (matrix[r][c] == 'S') {
                return make_tuple(r, c);
            }
        }
    }
    assert(false); // not found
}

bool compare(const CellData &a, const CellData &b) {
    return a.cost > b.cost;
}

vector<vector<Position>> find(vector<vector<char>> &matrix) {
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();

    auto start_dir = Direction::RIGHT;
    auto start_pos = find_start(matrix);
    auto [start_r, start_c] = start_pos;

    priority_queue<CellData, vector<CellData>, decltype(&compare)> pq(compare);
    pq.push(CellData{0, start_r, start_c, start_dir, {start_pos}});

    Cost min_cost = vector(rows, vector(cols, vector(4, INT_MAX)));
    min_cost[start_r][start_c][(int) start_dir] = 0;

    vector<vector<Position>> shortest_paths;

    while (!pq.empty()) {
        auto cell_data = pq.top();
        pq.pop();

        int current_cost = cell_data.cost;
        int r = cell_data.row;
        int c = cell_data.col;
        Direction dir = cell_data.dir;
        auto current_path = cell_data.path;

        if (current_cost > min_cost[r][c][(int) dir]) {
            continue;
        }

        if (matrix[r][c] == 'E') {
            int minimum_cost = min_cost[r][c][(int) dir];
            if (current_cost < minimum_cost) {
                min_cost[r][c][(int) dir] = current_cost;
                shortest_paths.clear();
                shortest_paths.push_back(current_path);
            } else if (current_cost == minimum_cost) {
                shortest_paths.push_back(current_path);
            }
            continue;
        }

        // move
        {
            const auto &[r_step, c_step] = get_step(dir);
            auto new_r = r + r_step;
            auto new_c = c + c_step;

            if (matrix[new_r][new_c] != '#') {
                int new_cost = current_cost + 1;
                if (new_cost <= min_cost[new_r][new_c][(int) dir]) {
                    min_cost[new_r][new_c][(int) dir] = new_cost;
                    auto new_path = current_path;
                    new_path.emplace_back(new_r, new_c);
                    pq.push(CellData{new_cost, new_r, new_c, dir, new_path});
                }
            }
        }

        // rotate cw
        {
            auto new_dir = rotate_cw(dir);
            int new_cost = current_cost + 1000;
            if (new_cost <= min_cost[r][c][(int) new_dir]) {
                min_cost[r][c][(int) new_dir] = new_cost;
                pq.push(CellData{new_cost, r, c, new_dir, current_path});
            }

        }

        // rotate ccw
        {
            auto new_dir = rotate_ccw(dir);
            int new_cost = current_cost + 1000;
            if (new_cost <= min_cost[r][c][(int) new_dir]) {
                min_cost[r][c][(int) new_dir] = new_cost;
                pq.push(CellData{new_cost, r, c, new_dir, current_path});
            }

        }
    }

    return shortest_paths;
}

void print(const vector<vector<char>> &matrix) {
    for (const auto &row: matrix) {
        for (auto it: row) {
            cout << it;
        }
        cout << endl;
    }
}

int count(const vector<vector<char>> &matrix) {
    int count = 0;
    for (const auto &row: matrix) {
        for (auto it: row) {
            if (it == 'O' || it == 'E' || it == 'S') {
                count++;
            }
        }
    }
    return count;
}

int main() {
    auto matrix = read_field("16/data.txt");
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();

    unordered_set<string> visited;

    auto paths = find(matrix);

    for (const auto &path: paths) {
        for (const auto &[r, c]: path) {
            matrix[r][c] = 'O';
        }
    }
    print(matrix);
    cout << count(matrix);
    return 0;
}