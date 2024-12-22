#include <iostream>
#include <fstream>
#include <deque>
#include <sstream>
#include <unordered_set>
#include <thread>

using namespace std;

vector<int> read_data(const string &filename) {
    vector<int> out;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
    }

    string line;
    while (getline(file, line)) {
        out.push_back(stoi(line));
    }

    file.close();
    return out;
}

long long next(long long number) {
    number = ((number * 64) ^ number) % 16777216;
    number = (((long long) (number / 32)) ^ number) % 16777216;
    number = ((number * 2048) ^ number) % 16777216;
    return number;
}

long long after_2000(long long number) {
    for (int i = 0; i < 2000; i++) {
        number = next(number);
    }
    return number;
}

void get_all_sequences(int number, unordered_set<string> &sequences) {
    deque<int> sequence;
    int prev_digit = number % 10;
    for (int i = 0; i < 2000 - 1; i++) {
        number = next(number);
        int current_digit = number % 10;
        int diff = current_digit - prev_digit;
        prev_digit = current_digit;
        sequence.push_back(diff);
        if (sequence.size() == 4) {
            stringstream ss;
            for (auto it: sequence) {
                ss << it << ",";
            }
            sequences.insert(ss.str());
            sequence.pop_front();
        }
    }
}

int get_number_of_bananas(int number, const vector<int> &target) {
    deque<int> sequence;
    int prev_digit = number % 10;
    for (int i = 0; i < 2000 - 1; i++) {
        number = next(number);
        int current_digit = number % 10;
        int diff = current_digit - prev_digit;
        prev_digit = current_digit;
        sequence.push_back(diff);
        if (sequence.size() == 4) {
            if (std::equal(target.begin(), target.end(), sequence.begin())) {
                return current_digit;
            }
            sequence.pop_front();
        }
    }
    return 0;
}

std::vector<int> split_to_numbers(const std::string &sentence) {
    std::vector<int> numbers;
    std::string token;
    std::istringstream tokenStream(sentence);

    while (std::getline(tokenStream, token, ',')) {
        numbers.push_back(stoi(token));
    }
    return numbers;
}

void part_1() {
    auto data = read_data("22/data");
    long long sum = 0;
    for (auto it: data) {
        sum += after_2000(it);
    }
    cout << sum << endl;
}

long long process_chunk(
    const vector<string> &sequences,
    const vector<int> &data,
    int start_row,
    int end_row,
    int thread_index
) {
    long long max_number_of_bananas = INT_MIN;
    int index = 0;
    int size = end_row - start_row;
    for (int r = start_row; r < end_row; r++) {
        cout << "thread (" << thread_index << ") - processing sequence : " << ++index << "/" << size << endl;
        const auto &seq = sequences[r];
        auto vec = split_to_numbers(seq);
        long long bananas = 0;
        for (auto number: data) {
            bananas += get_number_of_bananas(number, vec);
        }
        max_number_of_bananas = max(max_number_of_bananas, bananas);
    }
    return max_number_of_bananas;
}

void part_2() {
    auto data = read_data("22/data");
    cout << "number size : " << data.size() << endl;

    // collect all sequences
    unordered_set<string> sequences_set;
    for (size_t i = 0; i < data.size(); i++) {
        cout << "init number " << i << "/" << data.size() << endl;
        get_all_sequences(data[i], sequences_set);
    }

    // convert set to vec
    vector<string> sequences;
    sequences.reserve(sequences_set.size());
    for (const auto &seq: sequences_set) {
        sequences.emplace_back(seq);
    }

    cout << "sequences size : " << sequences.size() << endl;

    const int cores = 10;
    int chunk_size = sequences.size() / cores;
    vector<thread> threads;
    vector<long long> thread_result(cores, 0);

    for (int i = 0; i < cores; i++) {
        int start_row = chunk_size * i;
        int end_row = (i == cores - 1) ? sequences.size() : chunk_size * (i + 1);

        threads.emplace_back([&](int start_row, int end_row, int index) {
            thread_result[index] = process_chunk(sequences, data, start_row, end_row, i);
        }, start_row, end_row, i);
    }

    for (int i = 0; i < cores; i++) {
        threads[i].join();
    }

    long long max_result = INT_MIN;
    for (auto it: thread_result) {
        max_result = max(max_result, it);
    }
    cout << "Max possible bananas : " << max_result << endl;
}

int main() {
    part_2();
    return 0;
}