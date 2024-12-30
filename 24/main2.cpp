#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <set>

using namespace std;

enum class Operation {
    AND, OR, XOR
};

int calculate(int a, int b, Operation operation) {
    switch (operation) {
        case Operation::AND:
            return a && b;
        case Operation::OR:
            return a || b;
        case Operation::XOR:
            return a ^ b;
    }
}

vector<string> split(const string &sentence, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(sentence);

    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

map<string, int> read_wires(const string &filename) {
    map<string, int> output;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
    }

    string line;
    while (getline(file, line)) {
        auto signal = line.back() - '0';
        auto name = line.substr(0, 3);
        output.insert({name, signal});
    }

    file.close();
    return output;
}

vector<tuple<string, string, Operation, string>> read_gates(const string &filename, map<string, int> &wires) {
    vector<tuple<string, string, Operation, string>> output;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
    }

    string line;
    while (getline(file, line)) {
        auto pos = line.find("-> ");
        line.erase(pos, 3);
        auto array = split(line, ' ');
        auto in1 = array[0];
        auto in2 = array[2];
        auto out = array[3];
        auto type = array[1];

        if (wires.find(in1) == wires.end()) wires.insert({in1, -1});
        if (wires.find(in2) == wires.end()) wires.insert({in2, -1});
        if (wires.find(out) == wires.end()) wires.insert({out, -1});

        Operation op;
        if (type == "AND") {
            op = Operation::AND;
        } else if (type == "OR") {
            op = Operation::OR;
        } else if (type == "XOR") {
            op = Operation::XOR;
        } else {
            assert(false);
        }
        output.push_back(make_tuple(in1, in2, op, out));
    }

    file.close();
    return output;
}

int count_inactive_wires(const map<string, int> &wires) {
    int count = 0;
    for (const auto [_, signal]: wires) {
        if (signal == -1) {
            count++;
        }
    }
    return count;
}

long long get_number_on(const map<string, int> &wires, char c) {
    vector<pair<string, int>> z_wires;
    for (const auto &[name, signal]: wires) {
        if (name[0] == c) {
            z_wires.push_back({name, signal});
        }
    }
    sort(z_wires.begin(), z_wires.end());
    reverse(z_wires.begin(), z_wires.end());
    long long out = 0;
    for (const auto &[name, signal]: z_wires) {
        auto wire = wires.find(name)->second;

        if (signal == 1) {
            out = (out << 1) + 1;
        } else {
            out <<= 1;
        }
    }
    return out;
}

void part_1() {
    auto wires = read_wires("24/wires");
    auto gates = read_gates("24/gates", wires);

    while (count_inactive_wires(wires) > 0) {
        for (auto &[in1, in2, op, out]: gates) {
            if (wires[in1] != -1 && wires[in2] != -1) {
                wires[out] = calculate(wires[in1], wires[in2], op);
            }
        }
    }
    cout << get_number_on(wires, 'z');
}

long long wire_list_to_number(vector<string> list, map<string, int> &wires) {
    reverse(list.begin(), list.end());
    long long out = 0;
    for (const auto &name: list) {
        if (wires[name] == 1) {
            out = (out << 1) + 1;
        } else {
            out <<= 1;
        }
    }
    return out;
}
map<char, long long> group_wires_to_numbers(map<string, int> &wires) {
    map<char, vector<string>> groups;
    for (const auto &[name, signal]: wires) {
        auto c = name[0];
        auto iter = groups.find(c);
        if (iter == groups.end()) {
            groups.insert({c, {name}});
        } else {
            iter->second.push_back(name);
        }
    }

    map<char, long long> wires_to_number;
    for (auto &[c, list]: groups) {
        sort(list.begin(), list.end());
        wires_to_number.insert({c, wire_list_to_number(list, wires)});
    }
    return wires_to_number;
}

vector<pair<int, int>> create_all_indices_pairs(int N) {
    vector<pair<int, int>> out;
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            out.emplace_back(i, j);
        }
    }
    return out;
}

void swap_outs(vector<tuple<string, string, Operation, string>> &gates, pair<int, int> indices) {
    const auto &[i, j] = indices;
    string &out1 = get<3>(gates[i]);
    string &out2 = get<3>(gates[j]);

    auto entry1 = make_tuple(get<0>(gates[i]), get<1>(gates[i]), get<2>(gates[i]), out2);
    auto entry2 = make_tuple(get<0>(gates[j]), get<1>(gates[j]), get<2>(gates[j]), out1);

    gates[i] = entry1;
    gates[j] = entry2;
}

bool test(map<string, int> wires, const vector<tuple<string, string, Operation, string>> gates) {
    auto inactive = count_inactive_wires(wires);
    while (inactive > 0) {
        for (auto &[in1, in2, op, out]: gates) {
            if (wires[in1] != -1 && wires[in2] != -1) {
                wires[out] = calculate(wires[in1], wires[in2], op);
            }
        }
        auto tmp = count_inactive_wires(wires);
        if (tmp == inactive) {
            return false;
        }
        inactive = tmp;
    }

    long long x = get_number_on(wires, 'x');
    long long y = get_number_on(wires, 'y');
    long long z = get_number_on(wires, 'z');
//    cout << "x = " << x << endl;
//    cout << "y = " << y << endl;
//    cout << "z = " << z << endl;

    if (x + y == z) {
        return true;
    }
    return false;
}

void part_2() {
    auto wires = read_wires("24/wires");
    const auto gates = read_gates("24/gates", wires);

    const int N = gates.size();
    auto pairs = create_all_indices_pairs(N);

    for (int i = 0; i < pairs.size() - 3; i++) {
        for (int j = i + 1; j < pairs.size() - 2; j++) {
            for (int k = j + 1; k < pairs.size() - 1; k++) {
                for (int l = k + 1; l < pairs.size(); l++) {
                    cout << i << " " << j << " " << k << " " << l << endl;
                    auto mixed_gates = gates;
                    swap_outs(mixed_gates, pairs[i]);
                    swap_outs(mixed_gates, pairs[j]);
                    swap_outs(mixed_gates, pairs[k]);
                    swap_outs(mixed_gates, pairs[l]);

                    if (test(wires, mixed_gates)) {
                        cout << "Success" << endl;
                        return;
                    }
                }
            }
        }
    }
}

int main() {
//    part_1();
    part_2();
    return 0;
}