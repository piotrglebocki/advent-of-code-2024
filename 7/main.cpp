#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Element {
    long long number;
    char operation;
    bool is_number = true;
};

vector<vector<long long>> read_data(const string &filename) {
    vector<vector<long long>> matrix;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
    }

    string line;
    while (getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), ':'), line.end());
        vector<long long> row;
        istringstream lineStream(line);
        long long value;
        while (lineStream >> value) {
            row.push_back(value);
        }
        matrix.push_back(row);
    }

    file.close();
    return matrix;
}

void generate_sequences(string current, long long length, vector<string> &results) {
    if (current.size() == length) {
        results.push_back(current);
        return;
    }
    generate_sequences(current + '*', length, results);
    generate_sequences(current + '+', length, results);
}

vector<string> generate_all_combinations(long long length) {
    vector<string> results;
    generate_sequences("", length, results);
    return results;
}

void generate_sequences2(string current, long long length, vector<string> &results) {
    if (current.size() == length) {
        results.push_back(current);
        return;
    }
    generate_sequences2(current + '*', length, results);
    generate_sequences2(current + '+', length, results);
    generate_sequences2(current + '|', length, results);
}

vector<string> generate_all_combinations2(long long length) {
    vector<string> results;
    generate_sequences2("", length, results);
    return results;
}

vector<Element> build_equation(const vector<long long> &factors, const string &operations) {
    assert(operations.length() + 1 == factors.size());
    vector<Element> result;
    for (long long i = 0; i < operations.length(); i++) {
        result.push_back({factors[i], ' ', true});
        result.push_back({-1, operations[i], false});
    }
    result.push_back({factors[factors.size() - 1], ' ', true});
    return result;
}

long long solve_equation(vector<Element> &equation) {
    if (equation.size() == 1) return equation.front().number;

    vector<Element> tmp;
    long long res = equation[0].number;
    for (long long i = 2; i < equation.size(); i += 2) {
        if (equation[i - 1].operation == '*') {
            res *= equation[i].number;
        } else {
            res += equation[i].number;
        }
    }
    return res;
}

bool can_be_valid_equation(long long result, const vector<long long> &factors) {
    const long long factors_size = factors.size();
    if (factors_size == 1) {
        return result == factors.front();
    }
    long long all_operations_size = factors_size - 1;
    vector<string> sequences = generate_all_combinations(all_operations_size);

    for (const auto& seq : sequences) {
        auto equation = build_equation(factors, seq);
        auto res = solve_equation(equation);
        if (res == result) {
            return true;
        }
    }
    return false;
}

long long solution_1() {
    auto lines = read_data("7/data.txt");
    long long sum = 0;

    for (auto &line: lines) {
        if (line.empty()) {
            continue;
        }
        long long result = line.front();
        line.erase(line.begin());
        if (can_be_valid_equation(result, line)) {
            sum += result;
        }
    }

    return sum;
}

long long solution_2() {
    auto lines = read_data("7/data.txt");
    long long sum = 0;

    for (auto &line: lines) {
        if (line.empty()) {
            continue;
        }
        long long result = line.front();
        line.erase(line.begin());
        if (can_be_valid_equation(result, line)) {
            sum += result;
        } else {

        }
    }

    return sum;
}

int main() {
    cout << solution_1();
    return 0;
}