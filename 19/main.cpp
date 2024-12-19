#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Trie {
public:

    Trie() {
        root_ = new TrieNode();
    }

    void insert(const string &word) {
        TrieNode *node = root_;
        for (const auto c: word) {
            auto it = node->children.find(c);
            if (it == node->children.end()) {
                TrieNode *next = new TrieNode();
                node->children.insert({c, next});
                node = next;
            } else {
                node = it->second;
            }
        }
        node->is_word = true;
    }

    vector<string> find_all_prefixes(const string &word) const {
        vector<string> prefixes;
        TrieNode *node = root_;

        stringstream ss;
        for (const auto c: word) {
            auto it = node->children.find(c);
            if (it == node->children.end()) {
                break;
            } else {
                ss << c;
                if (it->second->is_word) {
                    prefixes.push_back(ss.str());
                }
                node = it->second;
            }
        }
        return prefixes;
    }
private:

    struct TrieNode {
        unordered_map<char, TrieNode *> children;
        bool is_word;
    };

    TrieNode *root_;
};

std::string trim(const std::string &str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> split(const std::string &sentence, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(sentence);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<string> read_patterns(const string &filename) {
    vector<string> out;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
    }

    stringstream ss;
    string line;
    while (getline(file, line)) {
        ss << line;
    }

    auto splitted = split(ss.str(), ',');
    out.reserve(splitted.size());
    for (const auto &word: splitted) {
        out.push_back(trim(word));
    }

    file.close();
    return out;
}

vector<string> read_towels(const string &filename) {
    vector<string> out;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
    }

    string line;
    while (getline(file, line)) {
        out.push_back(line);
    }

    file.close();
    return out;
}

long long ways_to_build_sequence(const string &sequence, const Trie &trie, unordered_map<string, long long> &memo) {
    auto it = memo.find(sequence);
    if (it != memo.end()) {
        return it->second;
    }

    if (sequence.empty()) {
        memo.insert({sequence, 1});
        return 1;
    }

    auto prefixes = trie.find_all_prefixes(sequence);
    if (prefixes.empty()) {
        memo.insert({sequence, 0});
        return 0;
    }

    long long total = 0;
    for (const auto &prefix: prefixes) {
        string new_sequence = sequence.substr(prefix.length());
        long long count = ways_to_build_sequence(new_sequence, trie, memo);
        total += count;
    }

    memo.insert({sequence, total});
    return total;
}

bool has_needed_chars(const string &towel, const unordered_set<char> &available_chars) {
    for (auto c: towel) {
        if (available_chars.find(c) == available_chars.end()) {
            return false;
        }
    }
    return true;
}

int main() {
    // read
    auto patterns = read_patterns("19/patterns.txt");
    auto towels = read_towels("19/towels.txt");

//    vector<string> towels = {"gbbr"};

    // init
    unordered_set<char> available_chars;
    Trie trie;

    for (const auto &pattern: patterns) {
        trie.insert(pattern);
        for (auto c: pattern) {
            available_chars.insert(c);
        }
    }

    // verify
    long long possible_count = 0;
    long long ways_to_build_seq = 0;
    unordered_map<string, long long> memo;
    for (const auto &it: towels) {
        if (has_needed_chars(it, available_chars)) {
            auto count = ways_to_build_sequence(it, trie, memo);
            if (count > 0) {
                cout << it << " : " << count << endl;
                possible_count++;
                ways_to_build_seq += count;
            }
        }
    }

    cout << possible_count << endl;
    cout << ways_to_build_seq;

    return 0;
}
