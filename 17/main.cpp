#include <vector>
#include <iostream>

using namespace std;

long long A = 236581108670061;
long long B = 0;
long long C = 0;

vector<long long> program = {2,4,1,3,7,5,0,3,4,1,1,5,5,5,3,0};

long long program_ptr = 0;

vector<long long> output;

long long read(long long arg) {
    if (arg == 4) return A;
    if (arg == 5) return B;
    if (arg == 6) return C;
    return arg;
}

void adv(long long arg) {
    long long val = read(arg);
    A = A >> val;
}

void bxl(long long arg) {
    B = B ^ arg;
}

void bst(long long arg) {
    long long val = read(arg);
    B = (val % 8);
}

void jnz(long long arg) {
    if (A != 0) {
        program_ptr = program[program_ptr];
    }
}

void bxc(long long arg) {
    B = B ^ C;
}

void out(long long arg) {
    long long val = read(arg) % 8;
    output.push_back(val);
    if (output.size() > program.size() || output[output.size()-1] != program[output.size() - 1]) {
        throw std::exception();
    }
}

void bdv(long long arg) {
    long long val = read(arg);
    B = A >> val;
}

void cdv(long long arg) {
    long long val = read(arg);
    C = A >> val;
}

void (*opcodes[])(long long) = {adv, bxl, bst, jnz, bxc, out, bdv, cdv};

void run() {
    while (program_ptr < program.size()) {
        long long opcode = program[program_ptr];
        long long operand = program[program_ptr + 1];

        if (opcode == 3) {
            if (A != 0) {
                program_ptr = operand * 2;
            } else {
                program_ptr += 2;
            }
        } else {
            opcodes[opcode](operand);
            program_ptr += 2;
        }
    }
}

bool equal(const vector<long long>& a, const vector<long long>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

int main() {

    long long min_A = 0;

    while (true) {
        // reset
        A = min_A;
        B = 0;
        C = 0;
        program_ptr = 0;
        output.clear();

        try {
            run();
            if (equal(output, program)) {
                cout << min_A;
                return 0;
            }
        } catch(std::exception e) {

        }

        min_A++;
    }

    return 0;
}