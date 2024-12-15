#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

const char WALL = '#';
const char BOX = 'O';
const char ROBOT = '@';
const char EMPTY = '.';

enum class Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN
};

struct Position {
    explicit Position(int row, int col) : row(row), col(col) {}
    int row, col;
};

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

vector<Direction> read_moves(const string &filename) {
    vector<Direction> output;
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open file " << filename << endl;
    }
    string line;
    while (getline(inputFile, line)) {
        for (auto c: line) {
            Direction dir;
            switch (c) {
                case '<':
                    dir = Direction::LEFT;
                    break;
                case '>':
                    dir = Direction::RIGHT;
                    break;
                case '^':
                    dir = Direction::UP;
                    break;
                case 'v':
                    dir = Direction::DOWN;
                    break;
            }
            output.push_back(dir);
        }
    }
    inputFile.close();
    return output;
}

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

void print(const vector<vector<char>> &matrix) {
    for (const auto &row: matrix) {
        for (auto it: row) {
            cout << it;
        }
        cout << endl;
    }
}

Position find_robot(const vector<vector<char>> &matrix) {
    for (int r = 0; r < matrix.size(); r++) {
        for (int c = 0; c < matrix[0].size(); c++) {
            if (matrix[r][c] == ROBOT) {
                return Position(r, c);
            }
        }
    }
    assert(false); // not found
}

Position move(vector<vector<char>> &matrix, const Position &robot, Direction dir) {
    const auto &[row_step, col_step] = get_step(dir);
    const auto new_row = robot.row + row_step;
    const auto new_col = robot.col + col_step;

    char val = matrix[new_row][new_col];
    if (val == WALL) {
        return robot; // no move
    } else if (val == EMPTY) {
        matrix[robot.row][robot.col] = EMPTY;
        matrix[new_row][new_col] = ROBOT;
        return Position(new_row, new_col);
    } else if (val == BOX) {
        bool can_move = false;
        int r = new_row;
        int c = new_col;
        while (matrix[r][c] != '#') {
            if (matrix[r][c] == EMPTY) {
                can_move = true;
                break;
            }
            r += row_step;
            c += col_step;
        }
        if (can_move) {
            while (r != new_row || c != new_col) {
                matrix[r][c] = matrix[r - row_step][c - col_step];
                r -= row_step;
                c -= col_step;
            }
            matrix[new_row][new_col] = ROBOT;
            matrix[robot.row][robot.col] = EMPTY;
            return Position(new_row, new_col);
        } else {
            return robot; // no move
        }
    } else {
        assert(false); // should never be here
    }
}

int cout_box_gps_pos_sum(const vector<vector<char>> &matrix) {
    int sum = 0;
    for (int r = 0; r < matrix.size(); r++) {
        for (int c = 0; c < matrix[0].size(); c++) {
            if (matrix[r][c] == BOX) {
               int gps_pos = r * 100 + c;
               sum += gps_pos;
            }
        }
    }
    return sum;
}

int main() {
    auto matrix = read_field("15/field.txt");
    auto moves = read_moves("15/moves.txt");
    auto robot = find_robot(matrix);

    for (auto &dir: moves) {
        robot = move(matrix, robot, dir);
    }

    print(matrix);
    cout<< cout_box_gps_pos_sum(matrix) << endl;
    return 0;
}