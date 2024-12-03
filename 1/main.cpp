#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

int distance(vector<int>& a, vector<int>& b) {
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    int sum = 0;
    for (size_t i = 0; i < a.size(); i++) {
        sum += abs(a[i] - b[i]);
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
    cout << distance(a, b);
    return 0;
}