//
// Created by Andrew Berlin on 5/6/18.
//

#ifndef HUFFMAN_LIB_FREQUENCY_H
#define HUFFMAN_LIB_FREQUENCY_H

#include <vector>
#include <string>
#include <unordered_map>

typedef std::pair<char, int> occur;

struct Frequency {
    Frequency() = default;
    Frequency(std::string s);
    void add_char(char c, int cnt);
    void add_char(char c);
    std::vector<occur> get_data()const;
    size_t size();
private:
    std::unordered_map<char, int> data;
};

#endif //HUFFMAN_LIB_FREQUENCY_H
