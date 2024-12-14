#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

using namespace std;

const int WIDTH = 101;
const int HEIGHT = 103;

using DataTuple = std::tuple<int, int, int, int>;

std::vector<std::string> split(const std::string &sentence, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(sentence);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


std::vector<DataTuple> read_data(const std::string &filename) {
    std::vector<DataTuple> data;
    std::ifstream infile(filename);

    if (!infile) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(infile, line)) {
        line.replace(0, 2, "");
        auto index = line.find(" v=");
        line.replace(index, 3, ",");
        auto numbers = split(line, ',');
        data.emplace_back(make_tuple(stoi(numbers[0]), stoi(numbers[1]), stoi(numbers[2]), stoi(numbers[3])));
    }

    infile.close();
    return data;
}

void print(const vector<vector<int>> &matrix) {
    for (const auto &row: matrix) {
        for (auto it: row) {
            if (it == 0)
                cout << '.';
            else
                cout << it;
        }
        cout << endl;
    }
}

std::tuple<int, int> move_robot_x_times(const DataTuple& robot, int times) {
    const auto &[x, y, dx, dy] = robot;

    int new_x = x + (dx * times);
    int new_y = y + (dy * times);

    new_x = (new_x % WIDTH + WIDTH) % WIDTH;
    new_y = (new_y % HEIGHT + HEIGHT) % HEIGHT;

    return std::make_tuple(new_x, new_y);
}

// Example usage
int main() {
    const std::string filename = "14/data.txt";
    std::vector<DataTuple> data = read_data(filename);

//    std::vector<DataTuple> data = {std::make_tuple(2, 4, 2, -3)};
//    vector<vector<int>> matrix(HEIGHT, vector<int>(WIDTH, 0));

    int tl = 0;
    int tr = 0;
    int bl = 0;
    int br = 0;

    for (const auto &it: data) {
        const auto& [x, y] = move_robot_x_times(it, 100);
        if (x < WIDTH / 2) {
            if (y < HEIGHT / 2) tl++;
            else if (y > HEIGHT / 2) bl++;
        } else if (x > WIDTH / 2) {
            if (y < HEIGHT / 2) tr++;
            else if (y > HEIGHT / 2) br++;
        }
//        matrix[y][x]++;
    }
//    print(matrix);

    cout << tl * tr * bl * br;
    return 0;
}
