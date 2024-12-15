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
                case '<': dir = Direction::LEFT; break;
                case '>': dir = Direction::RIGHT; break;
                case '^': dir = Direction::UP; break;
                case 'v': dir = Direction::DOWN; break;
                default: continue; // Ignore invalid characters
            }
            output.push_back(dir);
        }
    }
    inputFile.close();
    return output;
}

std::tuple<int, int> get_step(Direction dir) {
    switch (dir) {
        case Direction::LEFT:  return make_tuple(0, -1);
        case Direction::RIGHT: return make_tuple(0, 1);
        case Direction::UP:    return make_tuple(-1, 0);
        case Direction::DOWN:  return make_tuple(1, 0);
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

Position move_horozontally(vector<vector<char>> &matrix, const Position &robot, Direction dir) {
    const auto &[row_step, col_step] = get_step(dir);
    const auto new_row = robot.row + row_step;
    const auto new_col = robot.col + col_step;
    int r = new_row, c = new_col;

    while (matrix[r][c] != WALL) {
        if (matrix[r][c] == EMPTY) {
            // Move the robot and clear the previous position
            matrix[robot.row][robot.col] = EMPTY;
            matrix[r][c] = ROBOT;
            return Position(r, c);
        }
        r += row_step;
        c += col_step;
    }
    return robot;
}

bool can_move_vertically(const vector<vector<char>> &matrix, const Position &pos, Direction dir) {
    const auto &[row_step, col_step] = get_step(dir);
    const auto new_row = pos.row + row_step;
    const auto new_col = pos.col + col_step;

    if (new_row < 0 || new_row >= matrix.size() || new_col < 0 || new_col >= matrix[0].size()) {
        return false;
    }

    char val = matrix[new_row][new_col];
    if (val == WALL) {
        return false;
    } else if (val == EMPTY) {
        return true;
    } else if (val == BOX_L) {
        return can_move_vertically(matrix, Position(new_row, new_col), dir) &&
               can_move_vertically(matrix, Position(new_row, new_col + 1), dir);
    } else if (val == BOX_R) {
        return can_move_vertically(matrix, Position(new_row, new_col), dir) &&
               can_move_vertically(matrix, Position(new_row, new_col - 1), dir);
    }
    return false;
}

Position move_vertically(vector<vector<char>> &matrix, const Position &pos, Direction dir) {
    const auto &[row_step, col_step] = get_step(dir);
    const auto new_row = pos.row + row_step;
    const auto new_col = pos.col + col_step;

    if (matrix[new_row][new_col] == EMPTY) {
        matrix[pos.row][pos.col] = EMPTY;
        matrix[new_row][new_col] = ROBOT;
        return Position(new_row, new_col);
    } else if (matrix[new_row][new_col] == BOX_L) {
        const auto box_new_row = new_row + row_step;
        const auto box_new_col_left = new_col;
        const auto box_new_col_right = new_col + 1;

        if (matrix[box_new_row][box_new_col_left] == EMPTY &&
            matrix[box_new_row][box_new_col_right] == EMPTY) {
            matrix[box_new_row][box_new_col_left] = BOX_L;
            matrix[box_new_row][box_new_col_right] = BOX_R;

            matrix[new_row][new_col] = ROBOT;
            matrix[pos.row][pos.col] = EMPTY;

            matrix[new_row][new_col + 1] = EMPTY;
            return Position(new_row, new_col);
        }
    }
    return pos;
}

Position move(vector<vector<char>> &matrix, const Position &robot, Direction dir) {
    const auto &[row_step, col_step] = get_step(dir);
    const auto new_row = robot.row + row_step;
    const auto new_col = robot.col + col_step;

    if (matrix[new_row][new_col] == WALL) {
        return robot; // No movement
    } else if (matrix[new_row][new_col] == EMPTY) {
        return move_vertically(matrix, robot, dir);
    } else if (matrix[new_row][new_col] == BOX_L) {
        if (dir == Direction::LEFT || dir == Direction::RIGHT) {
            return move_horozontally(matrix, robot, dir);
        } else if (can_move_vertically(matrix, Position(new_row, new_col), dir)) {
            return move_vertically(matrix, robot, dir);
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
