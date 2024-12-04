#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int similarity(vector<int>& a, vector<int>& b) {
    map<int, int> count;
    for (auto it : b) {
        count[it]++;
    }
    int sum = 0;
    for (auto it : a) {
        sum += it * count[it];
    }
    return sum;
}

void read_data(std::vector<int>& column1, std::vector<int>& column2) {
    std::ifstream file("1/data.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << std::endl;
        return;
    }

    int col1, col2;
    while (file >> col1 >> col2) {
        column1.push_back(col1);
        column2.push_back(col2);
    }

    file.close();
}

int main() {
    vector<int> a;
    vector<int> b;
    read_data(a, b);
    cout << similarity(a, b);
    return 0;
}