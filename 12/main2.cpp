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

struct VerticalEdge {
    int j;
    int start_i;
    int end_i;
    bool is_left;
};

struct HorizontalEdge {
    int i;
    int start_j;
    int end_j;
    bool is_top;
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

vector<VerticalEdge> get_all_vertical_edges(const vector<Pos> nodes, const vector<vector<char>> &matrix) {
    vector<VerticalEdge> edges;
    for (const auto &it: nodes) {
        int val = matrix[it.i][it.j];
        Pos left{it.i, it.j - 1};
        Pos right{it.i, it.j + 1};

        if (get(matrix, left) != val) {
            edges.emplace_back(VerticalEdge{it.j, it.i, it.i + 1, true});
        }
        if (get(matrix, right) != val) {
            edges.emplace_back(VerticalEdge{it.j + 1, it.i, it.i + 1, false});
        }
    }
    return edges;
}

vector<HorizontalEdge> get_all_horizontal_edges(const vector<Pos> nodes, const vector<vector<char>> &matrix) {
    vector<HorizontalEdge> edges;
    for (const auto &it: nodes) {
        int val = matrix[it.i][it.j];
        Pos up{it.i - 1, it.j};
        Pos down{it.i + 1, it.j};

        if (get(matrix, up) != val) {
            edges.emplace_back(HorizontalEdge{it.i, it.j, it.j + 1, true});
        }
        if (get(matrix, down) != val) {
            edges.emplace_back(HorizontalEdge{it.i + 1, it.j, it.j + 1, false});
        }
    }
    return edges;
}

int count_groups(const std::vector<bool>& vec) {
    int count = 0;
    bool in_group = false;

    for (bool value : vec) {
        if (value) {
            if (!in_group) {
                count++;
                in_group = true;
            }
        } else {
            in_group = false;
        }
    }

    return count;
}

int count_vertical_sides(vector<VerticalEdge> edges, int j, bool left_side) {
    vector<VerticalEdge> edges_in_line;
    int min_i = INT_MAX;
    int max_i = INT_MIN;
    for (const auto& it: edges) {
        if (it.j == j && it.is_left == left_side) {
            edges_in_line.push_back(it);
            min_i = min(min_i, it.start_i);
            max_i = max(max_i, it.start_i);
        }
    }

    int size = max_i - min_i + 1;
    if (size == 0) {
        return 0;
    }

    vector<bool> buffer(size, false);
    for (const auto& it: edges_in_line) {
        buffer[it.start_i - min_i] = true;
    }

    return count_groups(buffer);
}


int count_horizontal_sides(vector<HorizontalEdge> edges, int i, bool top_side) {
    vector<HorizontalEdge> edges_in_line;
    int min_j = INT_MAX;
    int max_j = INT_MIN;
    for (const auto& it: edges) {
        if (it.i == i && it.is_top == top_side) {
            edges_in_line.push_back(it);
            min_j = min(min_j, it.start_j);
            max_j = max(max_j, it.start_j);
        }
    }

    int size = max_j - min_j + 1;
    if (size == 0) {
        return 0;
    }

    vector<bool> buffer(size, false);
    for (const auto& it: edges_in_line) {
        buffer[it.start_j - min_j] = true;
    }

    return count_groups(buffer);
}

int count_sides(const vector<Pos> nodes, const vector<vector<char>> &matrix) {
    int min_i = INT_MAX;
    int max_i = INT_MIN;
    int min_j = INT_MAX;
    int max_j = INT_MIN;

    for (const auto &it: nodes) {
        min_i = min(it.i, min_i);
        max_i = max(it.i + 1, max_i);
        min_j = min(it.j, min_j);
        max_j = max(it.j + 1, max_j);
    }

    int sides = 0;

    // vertical sides
    for (int j = min_j; j <= max_j; j++) {
        auto edges = get_all_vertical_edges(nodes, matrix);
        sides += count_vertical_sides(edges, j, false);
        sides += count_vertical_sides(edges, j, true);
    }

    // horizontal sides
    for (int i = min_i; i <= max_i; i++) {
        auto edges = get_all_horizontal_edges(nodes, matrix);
        sides += count_horizontal_sides(edges, i, false);
        sides += count_horizontal_sides(edges, i, true);
    }

    return sides;
}

int count_price(const Pos &pos, const vector<vector<char>> &matrix, vector<vector<bool>> &visited) {
    if (visited[pos.i][pos.j]) {
        return 0;
    }

    vector<Pos> nodes;
    visited[pos.i][pos.j] = true;

    queue<Pos> q;
    q.push(pos);

    while (!q.empty()) {
        Pos current_pos = q.front();
        q.pop();
        nodes.push_back(current_pos);

        auto neighbours = get_same_neighbours(matrix, current_pos);
        for (auto neighbour: neighbours) {
            if (!visited[neighbour.i][neighbour.j]) {
                visited[neighbour.i][neighbour.j] = true;
                q.push(neighbour);
            }
        }
    }

    return nodes.size() * count_sides(nodes, matrix);
}

int main() {
    auto matrix = read_data("12/data2.txt");
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