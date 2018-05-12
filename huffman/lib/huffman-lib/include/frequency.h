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
    explicit Frequency(std::string s);
    void add_char(uint8_t c, unsigned long long cnt);
    void add_char(uint8_t c);
    std::vector<occur> get_data()const;
    size_t size();
private:
    std::unordered_map<uint8_t, unsigned long long> data;
};

#endif //HUFFMAN_LIB_FREQUENCY_H
