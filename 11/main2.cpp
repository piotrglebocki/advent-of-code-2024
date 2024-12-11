#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <chrono>


using namespace std;

long long digit_count(long long number) {
    return to_string(number).length();
}

vector<long long> split_number(long long  number) {
    auto str = to_string(number);
    auto len = str.length();
    std::string first = str.substr(0, len / 2);
    std::string second = str.substr(len / 2);
    return {stoll(first), stoll(second)};
}

vector<long long> convert_stone(long long number) {
    if (number == 0) {
        return {1};
    } else if (digit_count(number) % 2 == 0) {
        return split_number(number);
    } else {
        return {number * 2024};
    }
}

long long count_new_stones_after_n_blinking(long long stone, long long n, unordered_map<long long, vector<long long>>& cache) {
    if (n == 0) {
        return 1;
    }
    auto iter = cache.find(stone);
    if (iter != cache.end() && iter->second[n] != -1) {
        return iter->second[n];
    }

    long long count = 0;
    auto new_stones = convert_stone(stone);
    for (auto it: new_stones) {
        count += count_new_stones_after_n_blinking(it, n - 1, cache);
    }
    
    if (iter == cache.end()) {
        vector<long long> vec(76, -1);
        vec[n] = count;
        cache.insert({stone, vec});
    } else {
        iter->second[n] = count;
    }
    return count;
}

int main() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    unordered_map<long long, vector<long long >> cache;
    vector<long long> stones = {4189, 413, 82070, 61, 655813, 7478611, 0, 8};
    long long count = 0;

    for (auto it: stones) {
        count += count_new_stones_after_n_blinking(it, 75, cache);
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    cout << count;

    return 0;
}