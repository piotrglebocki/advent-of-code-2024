#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<vector<char>> NUMERIC_CONTROLLER = {
    {
        '7', '8', '9'},
    {
        '4', '5', '6'},
    {
        '1', '2', '3'},
    {
        '#', '0', 'A'}
};

vector<vector<char>> KEYPAD_CONTROLLER = {
    {'#', '^', 'A'},
    {'<', 'v', '>'},
};

const vector<tuple<int, int>> DIRS = {
    {0,  1},
    {0,  -1},
    {1,  0},
    {-1, 0}
};

struct Position {
    int r, c;

    char get_move_char_to(const Position &to) const {
        if (r == to.r) {
            return (to.c - c < 0) ? '<' : '>';
        } else {
            return (to.r - r < 0) ? '^' : 'v';
        }
    }

    bool operator==(const Position &other) const {
        return r == other.r && c == other.c;
    }
};

unordered_map<string, vector<string>> numeric_cache;
unordered_map<string, vector<string>> keypad_cache;

Position find_char(const vector<vector<char>> &matrix, char target) {
    for (int r = 0; r < matrix.size(); r++) {
        for (int c = 0; c < matrix[0].size(); c++) {
            if (matrix[r][c] == target) {
                return Position{r, c};
            }
        }
    }
    assert(false); // not found
}

vector<Position> get_neighbours(const vector<vector<char>> &matrix, const Position &pos) {
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();
    const auto &[pos_r, pos_c] = pos;

    vector<Position> neighbours;
    for (const auto &[dr, dc]: DIRS) {
        int r = pos_r + dr;
        int c = pos_c + dc;

        if (0 <= r && r < rows && 0 <= c && c < cols && matrix[r][c] != '#') {
            neighbours.push_back(Position{r, c});
        }
    }
    return neighbours;
}

void dfs(
    const vector<vector<char>> &matrix,
    int depth,
    const Position &pos,
    const Position &target,
    vector<Position> current_path,
    vector<vector<Position>> &paths
) {
    if (depth > 5) return;

    if (pos == target) {
        paths.push_back(current_path);
        return;
    }

    auto neighbours = get_neighbours(matrix, pos);
    for (const auto &neighbour: neighbours) {
        if (std::find(current_path.begin(), current_path.end(), neighbour) == current_path.end()) {
            auto new_path = current_path;
            new_path.push_back(neighbour);
            dfs(matrix, depth + 1, neighbour, target, new_path, paths);
        }
    }
}

bool compare_paths_len(const vector<char> &a, const vector<char> &b) {
    return a.size() < b.size();
}

vector<vector<char>> find_shortest_paths(vector<vector<char>> &paths) {
    vector<vector<char>> out;
    sort(paths.begin(), paths.end(), compare_paths_len);

    if (!paths.empty()) {
        size_t max_size = paths.front().size();
        for (const auto &path: paths) {
            if (max_size == path.size()) {
                out.push_back(path);
            } else {
                break;
            }
        }
    }
    return out;
}

vector<vector<char>> find_simplest_paths(vector<vector<char>> &paths) {
    assert(!paths.empty());
    vector<vector<char>> out;
    int min_number_of_changes = INT_MAX;

    for (int p_index = 0; p_index < paths.size(); p_index++) {
        const auto &path = paths[p_index];
        int number_of_changes = 0;

        for (int i = 1; i < path.size(); i++) {
            if (path[i] != path[i - 1]) {
                number_of_changes++;
            }
        }

        if (number_of_changes < min_number_of_changes) {
            out.clear();
            out.push_back(path);
            min_number_of_changes = number_of_changes;
        } else if (number_of_changes == min_number_of_changes) {
            out.push_back(path);
        }
    }

    return out;
}

vector<vector<char>> find_all_move_sequences(const vector<vector<char>> &matrix, char from, char to) {
    const auto rows = matrix.size();
    const auto cols = matrix[0].size();

    auto start = find_char(matrix, from);
    auto end = find_char(matrix, to);

    vector<vector<Position>> paths;
    vector<vector<bool>> visited(rows, vector(cols, false));
    dfs(matrix, 0, start, end, {start}, paths);

    vector<vector<char>> sequences;

    for (const auto &path: paths) {
        vector<char> sequence;
        auto prev = path.front();
        for (int i = 1; i < path.size(); i++) {
            auto current = path[i];
            auto sign = prev.get_move_char_to(current);
            sequence.push_back(sign);
            prev = current;
        }
        sequences.push_back(sequence);
    }

    return sequences;
}

vector<string> get_path_sequences(const vector<vector<char>> &matrix, char from, char to) {
    auto paths = find_all_move_sequences(matrix, from, to);
    paths = find_shortest_paths(paths);
    paths = find_simplest_paths(paths);
    vector<string> out;
    for (const auto &path: paths) {
        out.emplace_back(path.begin(), path.end());
    }
    return out;
}

void
combine(const vector<vector<string>> &input, const string &current_string, vector<string> &results, int index = 0) {
    if (index == input.size()) {
        results.push_back(current_string);
    } else {
        auto sequences = input[index];
        for (auto seq: sequences) {
            combine(input, current_string + seq, results, index + 1);
        }
    }
}

vector<string> translate_code_to_keypad_moves(string code) {
    auto iter = numeric_cache.find(code);
    if (iter != numeric_cache.end()) {
        return iter->second;
    }

    vector<vector<string>> all_sequences(code.length());
    auto current = 'A';
    for (int i = 0; i < code.length(); i++) {
        auto sequences = get_path_sequences(NUMERIC_CONTROLLER, current, code[i]);
        for (const auto &seq: sequences) {
            all_sequences[i].push_back(seq + "A");
        }
        current = code[i];
    }

    vector<string> combinations;
    combine(all_sequences, "", combinations);
    numeric_cache.insert({code, combinations});
    return combinations;
}

vector<string> translate_keypad_sequence_to_another_keypad_moves(const string &sequence) {
    auto iter = keypad_cache.find(sequence);
    if (iter != keypad_cache.end()) {
        return iter->second;
    }
    vector<vector<string>> all_sequences(sequence.length());

    auto current = 'A';
    for (int i = 0; i < sequence.length(); i++) {
        auto sequences = get_path_sequences(KEYPAD_CONTROLLER, current, sequence[i]);
        for (const auto &seq: sequences) {
            all_sequences[i].push_back(seq + "A");
        }
        current = sequence[i];
    }

    vector<string> combinations;
    combine(all_sequences, "", combinations);
    keypad_cache.insert({sequence, combinations});
    return combinations;
}

int code_to_number(const string &code) {
    stringstream ss;
    for (auto &c: code) {
        if (isdigit(c)) ss << c;
    }
    return stoi(ss.str());
}


// TEST data
//vector<string> INPUT_DATA = {
//    "029A",
//    "980A",
//    "179A",
//    "456A",
//    "379A"
//};

vector<string> INPUT_DATA = {
    "129A",
    "540A",
    "789A",
    "596A",
    "582A"
};

void benchmark(std::function<void()> operation) {
    auto start_time = std::chrono::high_resolution_clock::now();
    operation();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Elapsed time: " << duration.count() << " ms" << std::endl;
}

int main() {
    benchmark([]() {
        long long sum = 0;
        for (const auto &code: INPUT_DATA) {
            int min_len = INT_MAX;
            auto numeric_sequences = translate_code_to_keypad_moves(code);
            for (const auto &numeric: numeric_sequences) {
                auto keypad_sequences_1 = translate_keypad_sequence_to_another_keypad_moves(numeric);
                for (const auto &keypad_1: keypad_sequences_1) {
                    auto keypad_sequences_2 = translate_keypad_sequence_to_another_keypad_moves(keypad_1);
                    for (const auto ks_2: keypad_sequences_2) {
                        min_len = min(min_len, (int) ks_2.length());
                    }

                }
            }
            sum += min_len * code_to_number(code);
        }
        cout << sum <<endl;
    });

    return 0;
}