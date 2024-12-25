#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<int> matrix_to_vec(const vector<vector<char>> &matrix) {
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();

    bool is_lock = matrix[0][0] == '#';
    vector<int> out;
    out.reserve(cols);
    for (int c = 0; c < cols; c++) {
        int count = 0;
        if (is_lock) {
            for (int r = 1; r < rows; r++) {
                if (matrix[r][c] == '#') {
                    count++;
                } else {
                    break;
                }
            }
        } else {
            for (int r = rows - 2; r >= 0; r--) {
                if (matrix[r][c] == '#') {
                    count--;
                } else {
                    break;
                }
            }
        }
        out.push_back(count);
    }
    return out;
}

vector<vector<int>> read_data(const string &filename) {
    vector<vector<int>> out;
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open file " << filename << endl;
    }
    string line;
    vector<vector<char>> matrix;
    while (getline(inputFile, line)) {
        if (line == "") {
            auto vec = matrix_to_vec(matrix);
            out.push_back(vec);
            matrix.clear();
        } else {
            vector<char> row(line.begin(), line.end());
            matrix.push_back(row);
        }
    }
    auto vec = matrix_to_vec(matrix);
    out.push_back(vec);
    matrix.clear();

    inputFile.close();
    return out;
}

bool is_lock(const vector<int>& vec) {
    for (auto it : vec) {
        if (it != 0) {
            return it > 0;
        }
    }
    assert(false);
}

vector<vector<int>> get_locks(const vector<vector<int>>& data) {
    vector<vector<int>> out;
    for (const auto& it: data) {
        if (is_lock(it)) {
            out.push_back(it);
        }
    }
    return out;
}

vector<vector<int>> get_keys(const vector<vector<int>>& data) {
    vector<vector<int>> out;
    for (const auto& it: data) {
        if (!is_lock(it)) {
            out.push_back(it);
        }
    }
    return out;
}

bool match(const vector<int>& key, const vector<int>& lock) {
    for (int i = 0; i < key.size(); i++) {
        if (abs(key[i]) + abs(lock[i]) > 5) {
            return false;
        }
    }
    return true;
}

int main() {

//    cout << match({0,5,3,4,3}, {3,0,2,0,1}) << endl;

    auto data = read_data("25/data");
    auto keys = get_keys(data);
    auto locks = get_locks(data);

    int pairs_count = 0;
    for (const auto& key : keys) {
        for (const auto& lock : locks) {
            if (match(key, lock)) {
                pairs_count++;
            }
        }
    }
    cout << pairs_count<< endl;

    return 0;
}