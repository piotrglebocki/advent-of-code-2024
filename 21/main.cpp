#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

char NUMERIC_CONTROLLER[4][3]{
    {
        '7', '8', '9'},
    {
        '4', '5', '6'},
    {
        '1', '2', '3'},
    {
        ' ', '0', 'A'}
};

char KEYPAD_CONTROLLER[2][3]{
    {' ', '^', 'A'},
    {'<', 'v', '>'},
};

struct Position {
    int r, c;

    tuple<int, int> distance(const Position &other) const {
        return make_tuple(
            other.r - r,
            other.c - c
        );
    }
};

unordered_map<char, Position> numeric_map;
unordered_map<char, Position> kaypad_map;

void init_numeric_controller_map() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            numeric_map.insert({NUMERIC_CONTROLLER[i][j], Position{i, j}});
        }
    }
}

void init_keypad_controller_map() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            kaypad_map.insert({KEYPAD_CONTROLLER[i][j], Position{i, j}});
        }
    }
}

bool check_if_panic_when_vert_move_first(const Position& from, const Position& to, const Position& panic_pos) {
    auto [r, c] = from;
    const auto [dr, dc] = from.distance(to);
    const auto [pr, pc] = panic_pos;

    for (int i = 0; i < abs(dr); i++) {
        r = (dr < 0) ? r - 1 : r + 1;
        if (r == pr && c == pc) {
            return true;
        }
    }
    for (int i = 0; i < abs(dc); i++) {
        c = (dc < 0) ? c - 1 : c + 1;
        if (r == pr && c == pc) {
            return true;
        }
    }
    return false;
}

string translate_code_to_keypad_moves(const string &code) {
    stringstream ss;
    auto current_pos = numeric_map.find('A')->second;
    static const Position panic_pos {0, 0};

    for (auto c: code) {
        const auto next_pos = numeric_map.find(c)->second;
        const auto& [dr, dc] = current_pos.distance(next_pos);

        bool vert_first = !check_if_panic_when_vert_move_first(current_pos, next_pos, panic_pos);

        if (vert_first) {
            for (int i = 0; i < abs(dr); i++) {
                if (dr < 0) ss << '^';
                else ss << 'v';
            }

            for (int i = 0; i < abs(dc); i++) {
                if (dc < 0) ss << '<';
                else ss << '>';
            }
        } else {
            for (int i = 0; i < abs(dc); i++) {
                if (dc < 0) ss << '<';
                else ss << '>';
            }

            for (int i = 0; i < abs(dr); i++) {
                if (dr < 0) ss << '^';
                else ss << 'v';
            }
        }

        ss<<'A';
        current_pos = next_pos;
    }
    return ss.str();
}

string translate_keypad_sequence_to_another_keypad_moves(const string &sequence) {
    stringstream ss;
    auto current_pos = kaypad_map.find('A')->second;
    static const Position panic_pos {3, 0};

    for (auto c: sequence) {
        const auto next_pos = kaypad_map.find(c)->second;
        const auto& [dr, dc] = current_pos.distance(next_pos);

        bool vert_first = !check_if_panic_when_vert_move_first(current_pos, next_pos, panic_pos);

        if (vert_first) {
            for (int i = 0; i < abs(dr); i++) {
                if (dr < 0) ss << '^';
                else ss << 'v';
            }

            for (int i = 0; i < abs(dc); i++) {
                if (dc < 0) ss << '<';
                else ss << '>';
            }
        } else {
            for (int i = 0; i < abs(dc); i++) {
                if (dc < 0) ss << '<';
                else ss << '>';
            }

            for (int i = 0; i < abs(dr); i++) {
                if (dr < 0) ss << '^';
                else ss << 'v';
            }
        }

        ss<<'A';
        current_pos = next_pos;
    }
    return ss.str();
}

int code_to_number(const string& code) {
    stringstream ss;
    for (auto& c : code) {
        if (isdigit(c)) ss<<c;
    }
    return stoi(ss.str());
}


// TEST data
//vector<string> INPUT_DATA = {
//    "029A",
//    "980A",
//    "179A",
//    "456A",
//    "379A"
//};

vector<string> INPUT_DATA = {
    "129A",
    "540A",
    "789A",
    "596A",
    "582A"
};

int main() {
    init_numeric_controller_map();
    init_keypad_controller_map();

    long long sum = 0;
    for (const auto& code: INPUT_DATA) {
        auto numeric_seq = translate_code_to_keypad_moves(code);
        auto keypad_1_seq = translate_keypad_sequence_to_another_keypad_moves(numeric_seq);
        auto keypad_2_seq = translate_keypad_sequence_to_another_keypad_moves(keypad_1_seq);

        cout << "(" << keypad_2_seq.size() << " * " <<code_to_number(code)<<")" << endl;
        sum += keypad_2_seq.size() * code_to_number(code);
    }

    cout <<sum;

    return 0;
}