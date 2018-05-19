//
// Created by Andrew Berlin on 5/6/18.
//

#ifndef HUFFMAN_LIB_FREQUENCY_H
#define HUFFMAN_LIB_FREQUENCY_H

#include <vector>
#include <string>
#include <array>

typedef std::pair<char, unsigned long long> occur;

struct Frequency {
    Frequency() = default;
    explicit Frequency(std::string s);
    void add_char(uint8_t c, unsigned long long cnt);
    void add_char(uint8_t c);
    std::array<unsigned long long, 256> const &get_data()const;
private:
    std::array<unsigned long long, 256> data;
};

#endif //HUFFMAN_LIB_FREQUENCY_H
