
#include <climits>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Vec {
    int x, y;
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
        int a_x = 0, a_y = 0, b_x = 0, b_y = 0, dest_x = 0, dest_y = 0;

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

            result.emplace_back(Data{Vec{a_x, a_y}, Vec{b_x, b_y}, Vec{dest_x, dest_y}});
        }
    }

    file.close();
    return result;
}

string gen_key(const Vec &pos, const int cost) {
    return to_string(pos.x) + "," + to_string(pos.y) + "," + to_string(cost);
}

int calculate_cost(
    const Data &data,
    const Vec &pos,
    const int cost,
    unordered_map<string, int> &cache
) {
//    cout << pos.x << ":" <<pos.y << ", " <<cost<<endl;
    const auto cache_key = gen_key(pos, cost);
    auto it = cache.find(cache_key);

    int result;
    if (it != cache.end()) {
        return it->second;
    }

    if (pos == data.dest) {
        result = cost;
    } else if (pos > data.dest) {
        result = INT_MAX;
    } else {
        result = min(
            calculate_cost(data, pos + data.step_a, cost + 3, cache),
            calculate_cost(data, pos + data.step_b, cost + 1, cache)
        );
    }
    cache.insert({cache_key, result});
    return result;
}

int main() {
    auto data = read_data("13/data.txt");
    unsigned int cost_sum = 0;
    for (const auto &it: data) {
        unordered_map<string, int> cache;
        auto cost = calculate_cost(it, {0, 0}, 0, cache);
        if (cost != INT_MAX) {
            cost_sum +=cost;
        }
    }

    cout<<cost_sum;

    return 0;
}



