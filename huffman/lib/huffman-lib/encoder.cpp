//
// Created by Andrew Berlin on 5/7/18.
//

#include "include/encoder.h"
#include <utility>

Encoder::Encoder(Frequency const &frequency) : Huffman(frequency) {};

Code Encoder::encode_segment(std::vector<uint8_t> const &data) {
    Code res;

    for (char c : data) {
        res.add(codes[c]);
    }
    return res;
}

Code Encoder::encode_segment(std::string const &data) {
    std::vector<uint8_t> tmp(data.begin(), data.end());
    return encode_segment(tmp);
}

Code const &Encoder::encode_char(uint8_t c) {
    return codes[c];
}


