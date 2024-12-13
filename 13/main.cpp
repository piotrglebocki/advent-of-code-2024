
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

long long SHIFT = 10000000000000;

struct Vec {
    long long x, y;
};

struct Data {
    const Vec step_a;
    const Vec step_b;
    const Vec dest;
};

Vec operator+(const Vec &a, const Vec &b) {
    return {a.x + b.x, a.y + b.y};
}

bool operator==(const Vec &a, const Vec &b) {
    return a.x == b.x && a.y == b.y;
}

bool operator>(const Vec &a, const Vec &b) {
    return a.x > b.x || a.y > b.y;
}

std::vector<Data> read_data(const std::string& filename) {
    std::vector<Data> result;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file.");
    }

    std::string line;
    while (std::getline(file, line)) {
        long long a_x = 0, a_y = 0, b_x = 0, b_y = 0, dest_x = 0, dest_y = 0;

        // Parse Button A
        if (line.find("Button A:") != std::string::npos) {
            std::sscanf(line.c_str(), "Button A: X+%d, Y+%d", &a_x, &a_y);

            // Parse Button B
            if (std::getline(file, line) && line.find("Button B:") != std::string::npos) {
                std::sscanf(line.c_str(), "Button B: X+%d, Y+%d", &b_x, &b_y);
            }

            // Parse Prize
            if (std::getline(file, line) && line.find("Prize:") != std::string::npos) {
                std::sscanf(line.c_str(), "Prize: X=%d, Y=%d", &dest_x, &dest_y);
            }

            result.emplace_back(Data{Vec{a_x, a_y}, Vec{b_x, b_y}, Vec{dest_x + SHIFT, dest_y + SHIFT}});
        }
    }

    file.close();
    return result;
}

long long calculate_cost(
    const Data &data
) {
    long long a_dx = data.step_a.x;
    long long a_dy = data.step_a.y;
    long long b_dx = data.step_b.x;
    long long b_dy = data.step_b.y;
    long long target_x = data.dest.x;
    long long target_y = data.dest.y;

    double denominator = a_dx * b_dy - a_dy * b_dx;
    if (denominator == 0) return 0;

    double pb = (a_dx * target_y - a_dy * target_x) / static_cast<double>(denominator);

    if (static_cast<long long>(pb) != pb) return 0;

    double pa = (target_x - b_dx * pb) / static_cast<double>(a_dx);
    if (static_cast<long long>(pa) != pa) return 0;

    return static_cast<long long>(pa * 3 + pb);
}

int main() {
    auto data = read_data("13/data.txt");
    unsigned long long cost_sum = 0;
    for (const auto &it: data) {
        cost_sum += calculate_cost(it);
    }
    cout<<cost_sum;
    return 0;
}



