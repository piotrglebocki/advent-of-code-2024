#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Pos {
    int i;
    int j;
};

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

void print_matrix(const vector<vector<char>> &matrix) {
    for (const auto &row: matrix) {
        for (auto c: row) {
            cout<<c;
        }
        cout<<endl;
    }
}

unordered_map<char, vector<Pos>> group_all_antennas(const vector<vector<char>> &matrix) {
    unordered_map<char, vector<Pos>> groups;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            auto c = matrix[i][j];
            if (c != '.') {
                Pos coords{i, j};
                auto it = groups.find(c);
                if (it == groups.end()) {
                    groups.insert({c, {coords}});
                } else {
                    it->second.push_back(coords);
                }
            }
        }
    }
    return groups;
}

bool is_in_bounds(const Pos &coord, const vector<vector<char>> &matrix) {
    const int rows = matrix.size();
    const int cols = matrix[0].size();
    return 0 <= coord.i && coord.i < rows && 0 <= coord.j && coord.j < cols;
}

int solution_1() {
    auto matrix = read_data("8/data.txt");
    auto groups = group_all_antennas(matrix);

    int count = 0;

    for (const auto &it: groups) {
        auto antennas = it.second;
        const auto size = antennas.size();

        for (int i = 0; i < size - 1; i++) {
            const Pos antenna1 = antennas[i];
            for (int j = i + 1; j < size; j++) {
                const Pos antenna2 = antennas[j];
                const int di = antenna2.i - antenna1.i;
                const int dj = antenna2.j - antenna1.j;
                const Pos antinode1{antenna2.i + di, antenna2.j + dj};
                const Pos antinode2{antenna1.i - di, antenna1.j - dj};

                if (is_in_bounds(antinode1, matrix)) {
                    if (matrix[antinode1.i][antinode1.j] != 'X') {
                        matrix[antinode1.i][antinode1.j] = 'X';
                        count++;
                    }
                }
                if (is_in_bounds(antinode2, matrix)) {
                    if (matrix[antinode2.i][antinode2.j] != 'X') {
                        matrix[antinode2.i][antinode2.j] = 'X';
                        count++;
                    }
                }
            }
        }
    }

//    print_matrix(matrix);
    return count;
}

int main() {
    cout << solution_1();
    return 0;
}