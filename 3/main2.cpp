#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

const string DO = "do()";
const string DONT = "don't()";
const string pattern = "mul(";
const size_t pattern_size = pattern.size();

vector<size_t> find_indices(const string &sequence, const string &substring) {
    std::vector<size_t> positions;
    size_t position = sequence.find(substring);
    while (position != std::string::npos) {
        positions.push_back(position);
        position = sequence.find(substring, position + 1);
    }
    return positions;
}

bool is_operation_enabled(int index, vector<size_t>& do_indices, vector<size_t>& dont_indices) {
    int last_do_index = -1;
    for (auto it: do_indices) {
        if (it < index) {
            last_do_index = it;
        } else {
            break;
        }
    }

    int last_dont_index = -1;
    for (auto it: dont_indices) {
        if (it < index) {
            last_dont_index = it;
        } else {
            break;
        }
    }

    if (last_dont_index != -1 && last_do_index < last_dont_index) {
        return false;
    }
    return true;
}

// return strings like "123,435"
vector<string> collect_valid_operations(const string &sequence) {
    vector<string> results;
    vector<size_t> indices = find_indices(sequence, pattern);
    vector<size_t> do_indices = find_indices(sequence, DO);
    vector<size_t> dont_indices = find_indices(sequence, DONT);

    for (auto it: indices) {
        if (is_operation_enabled(it, do_indices, dont_indices)) {
            size_t start_index = it + pattern_size;
            auto c = sequence[start_index];
            ostringstream ss;
            while (isdigit(c) || c == ',') {
                ss << c;
                c = sequence[++start_index];
            }
            if (c == ')') {
                results.emplace_back(ss.str());
            }
        }
    }
    return results;
}

std::vector<std::string> split(const std::string &sentence, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(sentence);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string read_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return "";
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    auto data = read_file("3/data.txt");
    auto pairs = collect_valid_operations(data);
    int sum = 0;
    for (const auto &it: pairs) {
        auto factors = split(it, ',');
        int a = stoi(factors[0]);
        int b = stoi(factors[1]);
        sum += a * b;
    }
    cout << sum;

    return 0;
}