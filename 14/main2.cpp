#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

using namespace std;

const int WIDTH = 101;
const int HEIGHT = 103;

using DataTuple = std::tuple<int, int, int, int>;

std::vector<std::string> split(const std::string &sentence, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(sentence);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


std::vector<DataTuple> read_data(const std::string &filename) {
    std::vector<DataTuple> data;
    std::ifstream infile(filename);

    if (!infile) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(infile, line)) {
        line.replace(0, 2, "");
        auto index = line.find(" v=");
        line.replace(index, 3, ",");
        auto numbers = split(line, ',');
        data.emplace_back(make_tuple(stoi(numbers[0]), stoi(numbers[1]), stoi(numbers[2]), stoi(numbers[3])));
    }

    infile.close();
    return data;
}

void print(const vector<vector<int>> &matrix) {
    for (const auto &row: matrix) {
        for (auto it: row) {
            if (it == 0)
                cout << '.';
            else
                cout << it;
        }
        cout << endl;
    }
}

void reset(vector<vector<int>> &matrix) {
    for (auto &row: matrix) {
        for (auto& it: row) {
            it = 0;
        }
    }
}

std::tuple<int, int> move_robot_x_times(const DataTuple& robot, int times) {
    const auto &[x, y, dx, dy] = robot;

    int new_x = x + (dx * times);
    int new_y = y + (dy * times);

    new_x = (new_x % WIDTH + WIDTH) % WIDTH;
    new_y = (new_y % HEIGHT + HEIGHT) % HEIGHT;

    return std::make_tuple(new_x, new_y);
}

bool search_line(const vector<vector<int>> &matrix) {
    const int line_size = 10;

    for (int i = 0; i < HEIGHT; i++) {
        int size = 0;
        for (int j = 0; j < WIDTH; j++) {
            if (matrix[i][j] != 0) {
                size++;
                if (size >= line_size) {
                    return true;
                }
            } else {
                size = 0;
            }
        }
    }
    return false;
}

int main() {
    const std::string filename = "14/data.txt";
    std::vector<DataTuple> data = read_data(filename);
    vector<vector<int>> matrix(HEIGHT, vector<int>(WIDTH, 0));

    for (int i = 1; i < 10000; i++) {
        reset(matrix);
        for (const auto &it: data) {
            const auto &[x, y] = move_robot_x_times(it, i);
            matrix[y][x]++;
        }
        if (search_line(matrix)) {
            cout << "After " << i << " sec" << endl;
            print(matrix);
            cout << endl;
        }
    }

    return 0;
}
