//
// Created by Andrew Berlin on 5/7/18.
//

#include "include/encoder.h"
#include <utility>

Encoder::Encoder(Frequency const &frequency) : huffman(frequency) {};

Code Encoder::encode_segment(std::vector<char> const &data) {
    auto codes = huffman.get_codes();
    Code res;

    for (char c : data) {
        res.add(codes[c]);
    }
    return res;
}
Code Encoder::encode_segment(std::string const &data) {
    std::vector<char> tmp(data.begin(), data.end());
    return encode_segment(tmp);
}
