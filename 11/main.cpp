#include <vector>
#include <iostream>
#include <fstream>
#include <queue>

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

long long count_new_stones_after_25_blinking(long long stone) {
    queue<long long> q;
    q.push(stone);

    long long n = 25;
    while(n--) {
        long long size = q.size();
        while (size--) {
            long long number = q.front();
            q.pop();
            auto new_numbers = convert_stone(number);
            for (auto it: new_numbers) {
                q.push(it);
            }
        }
    }
    return q.size();
}

int main() {
    vector<long long> stones = {4189, 413, 82070, 61, 655813, 7478611, 0, 8};
    long long count = 0;

    for (auto it: stones) {
        count += count_new_stones_after_25_blinking(it);
    }

    cout << count;

    return 0;
}