#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

using Position = std::tuple<int, int>;
using Visited = vector<vector<vector<bool>>>;

enum class Direction {
    RIGHT = 0,
    LEFT = 1,
    UP = 2,
    DOWN = 3
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

int dfs(const vector<vector<char>> &matrix, Visited &visited, const Position &pos, const Direction dir, int current_cost) {
    const auto &[r, c] = pos;
    visited[r][c][(int) dir] = true;

    if (matrix[r][c] == 'E') {
        return current_cost;
    }

    int min_cost = INT_MAX;

    // move
    {
        const auto &[r_step, c_step] = get_step(dir);
        auto new_r = r + r_step;
        auto new_c = c + c_step;
        if (!visited[new_r][new_c][(int) dir]) {

            if (matrix[new_r][new_c] != '#') {
                int cost = dfs(matrix, visited, make_tuple(new_r, new_c), dir, current_cost + 1);
                min_cost = min(min_cost, cost);
            }
        }
    }

    // rotate cw
    {
        auto new_dir = rotate_cw(dir);
        if (!visited[r][c][(int) new_dir]) {
            int cost = dfs(matrix, visited, pos, new_dir, current_cost + 1000);
            min_cost = min(min_cost, cost);
        }
    }

    // rotate ccw
    {
        auto new_dir = rotate_ccw(dir);
        if (!visited[r][c][(int) new_dir]) {
            int cost = dfs(matrix, visited, pos, new_dir, current_cost + 1000);
            min_cost = min(min_cost, cost);
        }
    }
    return min_cost;
}

int main() {
    auto matrix = read_field("16/data.txt");
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();
    Visited visited = vector(rows, vector(cols, vector(4, false)));

    auto start = find_start(matrix);
    auto [r, c] = start;
    visited[r][c][(int)Direction::RIGHT] = true;

    int cost = dfs(matrix, visited, start, Direction::RIGHT, 0);
    cout << cost << endl;
    return 0;
}