#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

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
        cout <<c<<" ";
    }
    cout<< endl;
}

void compress(vector<int>& sequence) {
    int l = 0;
    int r = sequence.size() - 1;

    while (l < r) {
        if (sequence[l] != -1) {
            l++;
        } else {
            while (r > l && sequence[r] == -1) r--;
            sequence[l] = sequence[r];
            sequence[r] = -1;
            r--;
            l++;
        }
    }
}

long long checksum(const vector<int>& sequence) {
    long long sum = 0;
    for (long long i = 0; i < sequence.size(); i++) {
        if (sequence[i] == -1) {
            break;
        }
        long long val = sequence[i];
        sum += i * val;
    }
    return sum;
}

int main() {
    auto data = read_data("9/data.txt");
    auto res = generate_disk_map(data);
//    print(res);
    compress(res);
//    print(res);
    cout << checksum(res);
    return 0;
}