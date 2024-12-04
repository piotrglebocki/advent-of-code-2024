#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void read_matrix(const string& filename, vector<vector<int>>& matrix) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<int> row;
        istringstream lineStream(line);
        int value;
        while (lineStream >> value) {
            row.push_back(value);
        }
        matrix.push_back(row);
    }

    file.close();
}

bool is_safe(const vector<int>& report) {
    const size_t size = report.size();
    if (size == 1) return true;

    int initial_diff = report[1] - report[0]; // increasing or decreasing

    for (size_t i = 0; i < size - 1; i++) {
        int diff = report[i + 1] - report[i];
        if (diff == 0) return false; // not increasing or decreasing
        if (diff * initial_diff < 0) return false; // changed growing direction
        if (abs(diff) < 1 || abs(diff) > 3) return false; // invalid jump
    }
    return true;
}

bool is_safe_without_one_level(const vector<int>& report) {
    for (int i = 0; i < report.size(); i++) {
        vector<int> new_report = report;
        new_report.erase(new_report.begin() + i);
        if (is_safe(new_report)) {
            return true;
        }
    }
    return false;
}

int count_safe_reports(const vector<vector<int>>& matrix) {
    int safe = 0;
    for (const auto& row: matrix) {
        if (is_safe(row) || is_safe_without_one_level(row)) {
            safe++;
        }
    }
    return safe;
}

int main() {
    vector<vector<int>> matrix;
    read_matrix("2/data.txt", matrix);
    cout << count_safe_reports(matrix);
    return 0;
}