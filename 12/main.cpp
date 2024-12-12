#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <queue>

using namespace std;

struct Pos {
    int i = -1;
    int j = -1;

    bool is_valid() {
        return i != -1 && j != -1;
    }
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

char get(const vector<vector<char>> &matrix, const Pos &pos) {
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();
    if (0 <= pos.i && pos.i < rows && 0 <= pos.j && pos.j < cols) {
        return matrix[pos.i][pos.j];
    }
    return '.';
}

vector<Pos> get_same_neighbours(const vector<vector<char>> &matrix, const Pos &pos) {
    vector<Pos> neighbours;

    int val = matrix[pos.i][pos.j];

    Pos left{pos.i, pos.j - 1};
    Pos right{pos.i, pos.j + 1};
    Pos up{pos.i - 1, pos.j};
    Pos down{pos.i + 1, pos.j};

    if (get(matrix, left) == val) neighbours.emplace_back(left);
    if (get(matrix, right) == val) neighbours.emplace_back(right);
    if (get(matrix, up) == val) neighbours.emplace_back(up);
    if (get(matrix, down) == val) neighbours.emplace_back(down);

    return neighbours;
}

int count_price(const Pos &pos, const vector<vector<char>> &matrix, vector<vector<bool>> &visited) {
    if (visited[pos.i][pos.j]) {
        return 0;
    }

    visited[pos.i][pos.j] = true;
    int areas_count = 0;
    int perimeter = 0;

    queue<Pos> q;
    q.push(pos);

    while (!q.empty()) {
        Pos current_pos = q.front();
        q.pop();

        auto neighbours = get_same_neighbours(matrix, current_pos);
        for (auto neighbour: neighbours) {
            if (!visited[neighbour.i][neighbour.j]) {
                visited[neighbour.i][neighbour.j] = true;
                q.push(neighbour);
            }
        }

        areas_count++;
        perimeter += (4 - neighbours.size());
    }

    return areas_count * perimeter;
}

int main() {
    auto matrix = read_data("12/data.txt");
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    int price_sum = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!visited[i][j]) {
                price_sum += count_price(Pos{i, j}, matrix, visited);
            }
        }
    }

    cout << price_sum << endl;

    return 0;
}