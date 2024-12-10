#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_set>

using namespace std;

struct Pos {
    int r;
    int c;
};


vector<vector<int>> read_data(const string &filename) {
    vector<vector<int>> matrix;
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open file " << filename << endl;
    }
    string line;
    while (getline(inputFile, line)) {
        vector<int> row;
        for (auto c: line) {
            row.push_back(c - '0');
        }
        matrix.emplace_back(row);
    }
    inputFile.close();
    return matrix;
}

int get(const vector<vector<int>> &matrix, const Pos& pos) {
    const int rows = matrix.size();
    const int cols = matrix[0].size();

    if (0 <= pos.r && pos.r < rows && 0 <= pos.c && pos.c < cols) {
        return matrix[pos.r][pos.c];
    }
    return -1;
}

void dfs(const vector<vector<int>> &matrix, const Pos &start, int& result) {
    if (get(matrix, start) == 9) {
        result++;
        return;
    }

    int x = get(matrix, start);
    Pos right_pos{start.r, start.c + 1};
    Pos left_pos{start.r, start.c - 1};
    Pos up_pos{start.r - 1, start.c};
    Pos down_pos{start.r + 1, start.c};

    // right
    if (get(matrix, right_pos) == x + 1) {
        dfs(matrix, right_pos, result);
    }
    // left
    if (get(matrix, left_pos) == x + 1) {
        dfs(matrix, left_pos, result);
    }
    // up
    if (get(matrix, up_pos) == x + 1) {
        dfs(matrix, up_pos, result);
    }
    // down
    if (get(matrix, down_pos) == x + 1) {
        dfs(matrix, down_pos, result);
    }
}

int main() {
    auto matrix = read_data("10/data2.txt");

    int sum;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            if (matrix[i][j] == 0) {
                int res = 0;
                dfs(matrix, Pos { i, j}, res);
                sum += res;
            }
        }
    }
    cout << sum;
    return 0;
}