#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

std::vector<std::string> split(const std::string &sentence, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(sentence);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<pair<int, int>> read_rules_data(const string &filename) {
    vector<pair<int, int>> result;
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open file " << filename << endl;
    }
    string line;
    while (getline(inputFile, line)) {
        vector<string> words = split(line, '|');
        result.emplace_back(stoi(words[0]), stoi(words[1]));
    }
    inputFile.close();
    return result;
}

vector<vector<int>> read_lines_data(const string &filename) {
    vector<vector<int>> result;
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open file " << filename << endl;
    }
    string line;
    while (getline(inputFile, line)) {
        vector<string> words = split(line, ',');
        vector<int> line_numbers;
        line_numbers.reserve(words.size());
        for (const auto &it: words) {
            line_numbers.emplace_back(stoi(it));
        }
        result.emplace_back(line_numbers);
    }
    inputFile.close();
    return result;
}

bool is_valid(const vector<int> &line, const unordered_map<int, vector<int>>& rules_map) {
    unordered_map<int, int> index_of;
    for (int i = 0; i < line.size(); i++) {
        index_of[line[i]] = i;
    }

    for (int index = 0; index < line.size(); index++) {
        int num = line[index];
        auto it = rules_map.find(num);
        if (it != rules_map.end()) {
            for (auto number_must_be_after: it->second) {
                auto it = index_of.find(number_must_be_after);
                if (it != index_of.end()) {
                    int index_of_number_must_be_after = it->second;
                    if (index > index_of_number_must_be_after) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

void sort_line(vector<int> &line, const unordered_map<int, vector<int>>& rules_map) {
    sort(line.begin(), line.end(), [&](int a, int b) {
        auto it = rules_map.find(b);
        if (it != rules_map.end()) {
            for (auto num : it->second) {
                if (num == a) {
                    return false;
                }
            }
        }
        return true;
    });
}

int main() {
    auto rules_pairs = read_rules_data("5/rules_data.txt");
    auto lines = read_lines_data("5/lines_data.txt");

    unordered_map<int, vector<int>> rules_map;
    for (const auto &it: rules_pairs) {
        auto iter = rules_map.find(it.first);
        if (iter == rules_map.end()) {
            rules_map.insert({it.first, {it.second}});
        } else {
            iter->second.push_back(it.second);
        }
    }

    int sum = 0;
    for (auto &line: lines) {
        if (!is_valid(line, rules_map)) {
            sort_line(line, rules_map);
            int mid = line[line.size() / 2];
            sum += mid;
        }
    }

    cout << sum;
    return 0;
}