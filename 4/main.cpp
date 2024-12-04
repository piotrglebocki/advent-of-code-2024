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

vector<vector<char>> get_all_words_starting_from(int r, int c, const vector<vector<char>>& m) {
    vector<vector<char>> result;

    const int rows = m.size();
    const int cols = m[0].size();

    static const int SHIFT = XMAS.size() - 1;

    // right
    if (r + SHIFT < rows) {
        vector<char> word = { m[r][c], m[r+1][c], m[r+2][c], m[r+3][c] };
        result.push_back(word);
    }

    // left
    if (r - SHIFT >= 0) {
        vector<char> word = { m[r][c], m[r-1][c], m[r-2][c], m[r-3][c] };
        result.push_back(word);
    }

    // up
    if (c - SHIFT >= 0) {
        vector<char> word = { m[r][c], m[r][c-1], m[r][c-2], m[r][c-3] };
        result.push_back(word);
    }

    // down
    if (c + SHIFT < cols) {
        vector<char> word = { m[r][c], m[r][c+1], m[r][c+2], m[r][c+3] };
        result.push_back(word);
    }

    // right - up
    if (r + SHIFT < rows && c - SHIFT >= 0) {
        vector<char> word = { m[r][c], m[r+1][c-1], m[r+2][c-2], m[r+3][c-3] };
        result.push_back(word);
    }

    // right - down
    if (r + SHIFT < rows && c + SHIFT < cols) {
        vector<char> word = { m[r][c], m[r+1][c+1], m[r+2][c+2], m[r+3][c+3] };
        result.push_back(word);
    }

    // left - up
    if (r - SHIFT >= 0 && c - SHIFT >= 0) {
        vector<char> word = { m[r][c], m[r-1][c-1], m[r-2][c-2], m[r-3][c-3] };
        result.push_back(word);
    }

    // left - down
    if (r - SHIFT >= 0 && c + SHIFT < cols) {
        vector<char> word = { m[r][c], m[r-1][c+1], m[r-2][c+2], m[r-3][c+3] };
        result.push_back(word);
    }

    return result;
}

bool is_xmas(const vector<char>& word) {
    assert(word.size() == 4);
    for (int i = 0; i < 4; i++) {
        if (XMAS[i] != word[i]) return false;
    }
    return true;
}

int main() {
    auto matrix = read_data("4/data.txt");
    int count = 0;

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            const auto c = matrix[i][j];
            if (c == 'X') {
                auto words = get_all_words_starting_from(i, j, matrix);
                for (const auto& word: words) {
                    if (is_xmas(word)) {
                        count++;
                    }
                }
            }
        }
    }

    cout<<count;
    return 0;
}