#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <cassert>

using namespace std;

const char WALL = '#';
const char BOX_L = '[';
const char BOX_R = ']';
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

    bool operator!=(const Position &other) const {
        return row != other.row || col != other.col;
    }
};

vector<vector<char>> read_field(const string &filename) {
    vector<vector<char>> matrix;
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open file " << filename << endl;
    }
    string line;
    while (getline(inputFile, line)) {
        vector<char> row;
        for (auto c: line) {
            if (c == '@') {
                row.push_back('@');
                row.push_back('.');
            } else if (c == 'O') {
                row.push_back('[');
                row.push_back(']');
            } else {
                row.push_back(c);
                row.push_back(c);
            }
        }
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
                default:
                    continue; // Ignore invalid characters
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
    return make_tuple(0, 0); // Default case
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
    assert(false);
}

bool move_horozontally(vector<vector<char>> &matrix, const Position &pos, Direction dir) {
    const auto &[row_step, col_step] = get_step(dir);
    const auto new_row = pos.row + row_step;
    const auto new_col = pos.col + col_step;

    if (matrix[new_row][new_col] == WALL) {
        return false;
    } else if (matrix[new_row][new_col] == EMPTY) {
        matrix[new_row][new_col] = matrix[pos.row][pos.col];
        matrix[pos.row][pos.col] = EMPTY;
        return true;
    } else {
        if (move_horozontally(matrix, Position{new_row, new_col}, dir)) {
            matrix[new_row][new_col] = matrix[pos.row][pos.col];
            matrix[pos.row][pos.col] = EMPTY;
            return true;
        }
    }

    return false;
}

bool can_move_vertically(const vector<vector<char>> &matrix, const Position &pos, Direction dir, vector<tuple<Position, char>>& to_be_moved) {
    const auto &[row_step, col_step] = get_step(dir);
    const auto new_row = pos.row + row_step;
    const auto new_col = pos.col + col_step;

    if (matrix[pos.row][pos.col] != WALL)
        to_be_moved.push_back(make_tuple(pos, matrix[pos.row][pos.col]));

    char val = matrix[new_row][new_col];
    if (val == WALL) {
        return false;
    } else if (val == EMPTY) {
        return true;
    } else if (val == BOX_L) {
        return can_move_vertically(matrix, Position(new_row, new_col), dir, to_be_moved) &&
               can_move_vertically(matrix, Position(new_row, new_col + 1), dir, to_be_moved);
    } else if (val == BOX_R) {
        return can_move_vertically(matrix, Position(new_row, new_col), dir, to_be_moved) &&
               can_move_vertically(matrix, Position(new_row, new_col - 1), dir, to_be_moved);
    }
    return false;
}

bool move_vertically(vector<vector<char>> &matrix, vector<tuple<Position, char>> &to_be_moved, Direction dir) {
    for (const auto& [pos, _] : to_be_moved) {
        matrix[pos.row][pos.col] = EMPTY;
    }

     const auto &[row_step, col_step] = get_step(dir);
    for (const auto& [pos, c] : to_be_moved) {
         const auto new_row = pos.row + row_step;
         const auto new_col = pos.col + col_step;
         matrix[new_row][new_col] = c;
     }
}

Position move(vector<vector<char>> &matrix, const Position &robot, Direction dir) {
    const auto &[row_step, col_step] = get_step(dir);
    const auto new_row = robot.row + row_step;
    const auto new_col = robot.col + col_step;

    if (matrix[new_row][new_col] == WALL) {
        return robot; // No movement
    } else {
        if (dir == Direction::LEFT || dir == Direction::RIGHT) {
            if (move_horozontally(matrix, robot, dir)) {
                return Position(new_row, new_col);
            }
        } else {
            vector<tuple<Position, char>> to_be_moved;
            if (can_move_vertically(matrix, robot, dir, to_be_moved)) {
                move_vertically(matrix, to_be_moved, dir);
                return Position(new_row, new_col);
            }
        }
    }
    return robot;
}

int count_box_gps_pos_sum(const vector<vector<char>> &matrix) {
    int sum = 0;
    for (int r = 0; r < matrix.size(); r++) {
        for (int c = 0; c < matrix[0].size(); c++) {
            if (matrix[r][c] == BOX_L) {
                sum += r * 100 + c;
            }
        }
    }
    return sum;
}

int main() {
    auto matrix = read_field("15/field.txt");
    auto moves = read_moves("15/moves.txt");
    auto robot = find_robot(matrix);

    for (const auto &dir: moves) {
        robot = move(matrix, robot, dir);
    }

    print(matrix);
    cout << count_box_gps_pos_sum(matrix) << endl;
    return 0;
}
