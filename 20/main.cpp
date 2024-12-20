#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <thread>

using namespace std;

using Pos = std::tuple<int, int>;

const vector<tuple<int, int>> Directions = {
    {0,  1},
    {0,  -1},
    {1,  0},
    {-1, 0}
};

struct QueueElement {
    int dist;
    Pos pos;
};

bool compare(const QueueElement &a, const QueueElement &b) {
    return a.dist > b.dist;
}

vector<vector<char>> read_matrix(const string &filename) {
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

vector<Pos> get_neighbours(vector<vector<char>> &matrix, const Pos &pos) {
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();
    const auto &[pos_r, pos_c] = pos;

    vector<Pos> neighbours;
    for (const auto &[dr, dc]: Directions) {
        int r = pos_r + dr;
        int c = pos_c + dc;

        if (0 <= r && r < rows && 0 <= c && c < cols && matrix[r][c] != '#') {
            neighbours.emplace_back(r, c);
        }
    }
    return neighbours;
}

int find_path_len(vector<vector<char>> &matrix, const Pos &start, const Pos &end) {
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();
    auto [start_r, start_c] = start;

    priority_queue<QueueElement, vector<QueueElement>, decltype(&compare)> pq(compare);
    pq.push(QueueElement{0, start});

    vector<vector<int>> distance = vector(rows, vector(cols, INT_MAX));
    distance[start_r][start_c] = 0;

    while (!pq.empty()) {
        auto element = pq.top();
        pq.pop();

        int current_dist = element.dist;
        Pos current_pos = element.pos;
        const auto &[current_r, current_c] = current_pos;

        if (current_pos == end) {
            return current_dist;
        }

        auto neighbours = get_neighbours(matrix, current_pos);
        for (const auto &neighbour: neighbours) {
            const auto &[r, c] = neighbour;

            int new_dist = current_dist + 1;
            if (new_dist < distance[r][c]) {
                distance[r][c] = new_dist;
                pq.push(QueueElement{new_dist, neighbour});
            }
        }
    }

    return INT_MAX;
}

Pos find_char(const vector<vector<char>> &matrix, const char target) {
    for (int r = 0; r < matrix.size(); r++) {
        for (int c = 0; c < matrix[0].size(); c++) {
            if (matrix[r][c] == target) {
                return make_tuple(r, c);
            }
        }
    }
    assert(false); // not found
}

void print(const vector<vector<char>> &matrix) {
    for (const auto &row: matrix) {
        for (auto it: row) {
            cout << it;
        }
        cout << endl;
    }
}

void benchmark(std::function<void()> operation) {
    auto start_time = std::chrono::high_resolution_clock::now();
    operation();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Elapsed time: " << duration.count() << " ms" << std::endl;
}

int process_rows(
    vector<vector<char>> matrix, // make copy of matrix. Important for threading !
    const Pos &start,
    const Pos &end,
    int initial_path_len,
    int start_row,
    int end_row
) {
    const auto cols = matrix[0].size();
    int count = 0;

    for (int r = start_row; r < end_row; r++) {
        for (int c = 0; c < cols; c++) {
            if (matrix[r][c] == '#') {
                matrix[r][c] = '.';
                auto path_len = find_path_len(matrix, start, end);
                if (path_len < INT_MAX && (initial_path_len - path_len >= 100)) {
                    count++;
                }
                matrix[r][c] = '#';
            }
        }
    }
    return count;
}

int main() {
    benchmark([]() {
        auto matrix = read_matrix("20/data.txt");

        auto start = find_char(matrix, 'S');
        auto end = find_char(matrix, 'E');

        auto initial_path_len = find_path_len(matrix, start, end);
        const auto rows = matrix.size();

        const int cores = 10;
        int chunk_size = rows / cores;
        vector<thread> threads;
        vector<int> thread_result(cores, 0);

        for (int i = 0; i < cores; i++) {
            int start_row = chunk_size * i;
            int end_row = (i == cores - 1) ? rows : chunk_size * (i + 1);

            threads.emplace_back([&](int start_row, int end_row, int index) {
                thread_result[index] = process_rows(matrix, start, end, initial_path_len, start_row, end_row);
            }, start_row, end_row, i);
        }

        for (int i = 0; i < cores; i++) {
            threads[i].join();
        }

        int count = 0;
        for (auto it: thread_result) count += it;

        cout << count << endl;
    });

    return 0;
}