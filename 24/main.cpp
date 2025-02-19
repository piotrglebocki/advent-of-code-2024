#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <set>

using namespace std;

class Wire {
public:

    Wire(const string &name) : name_(name), signal_(-1) {}

    Wire(const string &name, int signal) : name_(name), signal_(signal) {}

    bool has_signal() {
        return signal_ != -1;
    }

    int signal() {
        assert(has_signal());
        return signal_;
    }

    void set(int signal) {
        signal_ = signal;
    }

private:
    int signal_;
    string name_;
};

using Wires = map<string, Wire *>;

int AND(int a, int b) {
    return a && b;
}

int OR(int a, int b) {
    return a || b;
}

int XOR(int a, int b) {
    return a ^ b;
}

class Gate {
public:

    Gate(int (*operation)(int, int), Wire *in1, Wire *in2, Wire *out) :
        in1_(in1), in2_(in2), out_(out), operation_(operation) {}

    int out() {
        assert(is_active());
        return operation_(in1_->signal(), in2_->signal());
    }

    void test() {
        if (is_active()) {
            out_->set(out());
        }
    }

    bool is_active() {
        return in1_->has_signal() && in2_->has_signal();
    }

private:
    Wire *in1_;
    Wire *in2_;
    Wire *out_;
    int (*operation_)(int, int);
};

vector<string> split(const string &sentence, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(sentence);

    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

Wire *get_or_create_wire(const string &name, Wires &wires) {
    auto iter = wires.find(name);
    if (iter == wires.end()) {
        Wire *wire = new Wire(name);
        wires.insert({name, wire});
        return wire;
    } else {
        return iter->second;
    }
}

Wires read_wires(const string &filename) {
    Wires output;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
    }

    string line;
    while (getline(file, line)) {
        auto signal = line.back() - '0';
        auto name = line.substr(0, 3);
        output.insert({name, new Wire(name, signal)});
    }

    file.close();
    return output;
}

vector<Gate *> read_gates(const string &filename, Wires &wires) {
    vector<Gate *> output;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
    }

    string line;
    while (getline(file, line)) {
        auto pos = line.find("-> ");
        line.erase(pos, 3);
        auto array = split(line, ' ');
        auto in1 = get_or_create_wire(array[0], wires);
        auto in2 = get_or_create_wire(array[2], wires);
        auto out = get_or_create_wire(array[3], wires);

        auto type = array[1];
        int (*operation)(int, int);
        if (type == "AND") {
            operation = AND;
        } else if (type == "OR") {
            operation = OR;
        } else if (type == "XOR") {
            operation = XOR;
        } else {
            assert(false);
        }

        Gate *gate = new Gate(operation, in1, in2, out);
        output.push_back(gate);
    }

    file.close();
    return output;
}

int count_inactive_wires(const Wires &wires) {
    int count = 0;
    for (const auto [_, wire]: wires) {
        if (!wire->has_signal()) {
            count++;
        }
    }
    return count;
}

long long get_all_z_signals(const Wires &wires) {
    vector<string> z_wires;
    for (const auto &[name, wire]: wires) {
        if (name[0] == 'z') {
            z_wires.push_back(name);
        }
    }
    sort(z_wires.begin(), z_wires.end());
    reverse(z_wires.begin(), z_wires.end());
    long long out = 0;
    for (const auto &name: z_wires) {
        auto wire = wires.find(name)->second;
        auto signal = wire->signal();

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
        for (Gate *gate: gates) {
            gate->test();
        }
    }
    cout << get_all_z_signals(wires);
}

long long wire_list_to_number(vector<Wire *> wires) {
    reverse(wires.begin(), wires.end());
    long long out = 0;
    for (const auto &wire: wires) {
        auto signal = wire->signal();
        if (signal == 1) {
            out = (out << 1) + 1;
        } else {
            out <<= 1;
        }
    }
    return out;
}

map<char, long long> group_wires_to_numbers(const Wires &wires) {
    map<char, vector<Wire *>> groups;
    for (const auto &[name, wire]: wires) {
        auto c = name[0];
        auto iter = groups.find(c);
        if (iter == groups.end()) {
            groups.insert({c, {wire}});
        } else {
            iter->second.push_back(wire);
        }
    }

    map<char, long long> wires_to_number;
    for (auto &[c, list]: groups) {
        sort(list.begin(), list.end());
        wires_to_number.insert({c, wire_list_to_number(list)});
    }
    return wires_to_number;
}

void part_2() {
    auto wires = read_wires("24/wires");
    auto gates = read_gates("24/gates", wires);

    while (count_inactive_wires(wires) > 0) {
        for (Gate *gate: gates) {
            gate->test();
        }
    }

    auto wires_to_number = group_wires_to_numbers(wires);
    for (auto &[c, num]: wires_to_number) {
        cout << c << " = " << num << endl;
    }

    set<char> valid_wires;
    for (auto [c1, n1]: wires_to_number) {
        for (auto [c2, n2]: wires_to_number) {
            for (auto [c3, n3]: wires_to_number) {
                if (c1 != c2 && c1 != c3 && c2 != c3) {
                    if (n1 == n2 + n3) {
                        cout << n1 << " = " << n2 << " + " << n3 << endl;
                        valid_wires.insert(c1);
                        valid_wires.insert(c2);
                        valid_wires.insert(c3);
                    }
                }
            }
        }
    }

    for (auto c : valid_wires) {
        cout << c << endl;
    }
}

int main() {
    part_1();
//    part_2();
    return 0;
}