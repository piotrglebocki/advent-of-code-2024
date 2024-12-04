#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

const vector<char> XMAS = {'X', 'M', 'A', 'S'};

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

bool is_xmas(int r, int c, const vector<vector<char>>& m) {
    const int rows = m.size();
    const int cols = m[0].size();

    if (r >= 1 && r + 1 < rows && c >= 1 && c + 1 < cols) {
        bool diagonal1 = (m[r-1][c-1] == 'M' && m[r+1][c+1] == 'S') || (m[r-1][c-1] == 'S' && m[r+1][c+1] == 'M');
        bool diagonal2 = (m[r+1][c-1] == 'M' && m[r-1][c+1] == 'S') || (m[r+1][c-1] == 'S' && m[r-1][c+1] == 'M');
        return diagonal1 && diagonal2;
    }
    return false;
}

int main() {
    auto matrix = read_data("4/data.txt");
    int count = 0;

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            const auto c = matrix[i][j];
            if (c == 'A' && is_xmas(i, j, matrix)) {
                count++;
            }
        }
    }

    cout<<count;
    return 0;
}