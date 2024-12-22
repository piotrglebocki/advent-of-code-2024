#include <iostream>
#include <fstream>

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
    number =((number * 64) ^ number) % 16777216;
    number =(((long long)(number / 32)) ^ number) % 16777216;
    number =((number * 2048) ^ number) % 16777216;
    return number;
}

long long after_2000(long long number) {
    for (int i = 0; i < 2000; i++) {
        number = next(number);
    }
    return number;
}

int main() {
    auto data = read_data("22/data");
    long long sum = 0;
    for (auto it : data) {
        sum += after_2000(it);
    }
    cout<<sum<<endl;
    return 0;
}