#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>

using namespace std;

string read_data(const string &filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open file " << filename << endl;
    }
    string line;
    getline(inputFile, line);
    inputFile.close();
    return line;
}

vector<int> generate_disk_map(const string& word) {
    vector<int> disk;
    for (int i = 0; i < word.length(); i++) {
        int n = word[i] - '0';
        while(n--) {
            disk.push_back(i % 2 == 0 ? (i / 2) : -1);
        }
    }
    return disk;
}

void print(const vector<int>& sequence) {
    for (auto c: sequence) {
        if (c == -1) {
            cout<<".";
        } else {
            cout << c << "";
        }
    }
    cout<< endl;
}

vector<vector<int>> group(const vector<int>& sequence) {
    vector<vector<int>> groups;
    vector<int> group;
    int prev = sequence.front();

    for (auto it : sequence) {
        if (it == prev) {
            group.push_back(it);
        } else {
            groups.push_back(group);
            group.clear();
            prev = it;
            group.push_back(it);
        }
    }
    if (!group.empty()) {
        groups.push_back(group);
    }
    return groups;
}

int next_window(const vector<vector<int>>& groups, unordered_set<int>& processed_files) {
    int i = groups.size() - 1;
    while (i >= 0 && (groups[i][0] == -1 || processed_files.find(groups[i][0]) != processed_files.end())) {
        i--;
    }
    processed_files.insert(groups[i][0]);
    return i;
}

int find_first_matching_whole(const vector<vector<int>>& groups, int min_size) {
    for (int i = 0; i < groups.size(); i++) {
        const auto& group = groups[i];
        if (group.size() >= min_size && group[0] == -1) {
            return i;
        }
    }
    return -1;
}

vector<int> to_sequence(const vector<vector<int>>& groups) {
    vector<int> out;
    for (const auto& group: groups) {
        out.insert(out.end(), group.begin(), group.end());
    }
    return out;
}

vector<int> compress(const vector<int>& sequence) {
    auto groups = group(sequence);
    unordered_set<int> processed_files;
    int file_index = INT_MAX;

    while (file_index > 0) {
        file_index = next_window(groups, processed_files);
        int file_len = groups[file_index].size();

        int whole_index = find_first_matching_whole(groups, file_len);
        if (whole_index != -1) {
            if (whole_index < file_index) {
                int whole_len = groups[whole_index].size();
                auto file = groups[file_index];

                groups.erase(groups.begin() + file_index);
                groups.insert(groups.begin() + file_index, vector<int>(file_len, -1));

                groups.erase(groups.begin() + whole_index);
                groups.insert(groups.begin() + whole_index, file);

                // handle rest;
                if (whole_len > file_len) {
                    int rest = whole_len - file_len;
                    vector<int> whole(rest, -1);
                    groups.insert(groups.begin() + whole_index + 1, whole);
                }

                // re-group
                auto seq = to_sequence(groups);
                groups = group(seq);
            }
        }
    }

    return to_sequence(groups);
}

long long checksum(const vector<int>& sequence) {
    long long sum = 0;
    for (long long i = 0; i < sequence.size(); i++) {
        if (sequence[i] == -1) {
            continue;
        }
        long long val = sequence[i];
        sum += i * val;
    }
    return sum;
}

int main() {
    auto data = read_data("9/data2.txt");
    auto res = generate_disk_map(data);
    res = compress(res);
    cout << checksum(res);
    return 0;
}