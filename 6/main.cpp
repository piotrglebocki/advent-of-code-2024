#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>

using namespace std;

enum class Dir {
    RIGHT = 1,
    LEFT = 2,
    TOP = 3,
    DOWN = 4
};

struct Guard {
    int row;
    int col;
    Dir dir = Dir::TOP;

    string hash() {
        return to_string(row) + "/" + to_string(col) + "/" + to_string((int) dir);
    }
};

bool*** visited;

void reset_visited() {
    for (int i = 0; i < 130; i++) {
        for (int j = 0; j < 130; j++) {
            for (int k = 0; k < 4; k++) {
                visited[i][j][k] = false;
            }
        }
    }
}

vector<vector<char>> read_data(const string &filename) {
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

Guard find_start_position(const vector<vector<char>> &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == '^') {
                return Guard{i, j};
            }
        }
    }
    return Guard{-1, -1};
}

bool is_game_over(const vector<vector<char>> &matrix, const Guard &guard) {
    const int rows = matrix.size();
    const int cols = matrix.front().size();
    return
        (guard.row == 0 && guard.dir == Dir::TOP) ||
        (guard.row == rows - 1 && guard.dir == Dir::DOWN) ||
        (guard.col == 0 && guard.dir == Dir::LEFT) ||
        (guard.col == cols - 1 && guard.dir == Dir::RIGHT);
}

char next_char(const vector<vector<char>> &matrix, const Guard &guard) {
    if (guard.dir == Dir::TOP) {
        return matrix[guard.row - 1][guard.col];
    }
    if (guard.dir == Dir::DOWN) {
        return matrix[guard.row + 1][guard.col];
    }
    if (guard.dir == Dir::RIGHT) {
        return matrix[guard.row][guard.col + 1];
    }
    if (guard.dir == Dir::LEFT) {
        return matrix[guard.row][guard.col - 1];
    }
    return '?';
}

void move(Guard &guard) {
    if (guard.dir == Dir::TOP) guard.row--;
    if (guard.dir == Dir::RIGHT) guard.col++;
    if (guard.dir == Dir::DOWN) guard.row++;
    if (guard.dir == Dir::LEFT) guard.col--;
}

void rotate(Guard &guard) {
    if (guard.dir == Dir::TOP) {
        guard.dir = Dir::RIGHT;
    } else if (guard.dir == Dir::RIGHT) {
        guard.dir = Dir::DOWN;
    } else if (guard.dir == Dir::DOWN) {
        guard.dir = Dir::LEFT;
    } else if (guard.dir == Dir::LEFT) {
        guard.dir = Dir::TOP;
    }
}

bool check_and_mark_visited(vector<vector<char>> &matrix, const Guard &guard) {
    if (guard.row < 0 || guard.row >= matrix.size() || guard.col < 0 || guard.col >= matrix[0].size())
        return false;

    if (matrix[guard.row][guard.col] != 'X') {
        matrix[guard.row][guard.col] = 'X';
        return true;
    }
    return false;
}

int solution_1() {
    auto matrix = read_data("6/data.txt");
    auto guard = find_start_position(matrix);

    int visited_count = 1;
    matrix[guard.row][guard.col] = 'X';

    while (!is_game_over(matrix, guard)) {
        auto c = next_char(matrix, guard);
        if (c == '#') {
            rotate(guard);
        }
        move(guard);
        if (check_and_mark_visited(matrix, guard)) {
            visited_count++;
        }
    }

    return visited_count;
}

bool is_running_in_loop(const vector<vector<char>> &matrix) {
    auto guard = find_start_position(matrix);
    reset_visited();

    while (!is_game_over(matrix, guard)) {
        auto c = next_char(matrix, guard);
        if (c == '#') {
            rotate(guard);
            visited[guard.row][guard.col][(int) guard.dir - 1] = true;
        } else {
            move(guard);
            if (visited[guard.row][guard.col][(int) guard.dir - 1]) {
                return true;
            }
            visited[guard.row][guard.col][(int) guard.dir - 1] = true;
        }
    }

    return false;
}

int solution_2() {
    auto matrix = read_data("6/data2.txt");
    int count = 0;

    visited = new bool**[matrix.size()];
    for (int i = 0; i < matrix.size(); i++) {
        visited[i] = new bool*[matrix[0].size()];
        for (int j = 0; j < matrix[0].size(); j++) {
            visited[i][j] = new bool[4];
        }
    }

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            auto c = matrix[i][j];
            if (c == '.') {
                matrix[i][j] = '#';
                if (is_running_in_loop(matrix)) {
                    count++;
                }
                matrix[i][j] = '.';
            }
        }
    }

    return count;
}


int main() {
//    cout << solution_1();
    cout << solution_2();
    return 0;
}