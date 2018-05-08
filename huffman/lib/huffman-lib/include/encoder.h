//
// Created by Andrew Berlin on 5/7/18.
//

#ifndef HUFFMAN_LIB_ENCODER_H
#define HUFFMAN_LIB_ENCODER_H

#include <vector>
#include <string>

#include "huffman.h"
#include "frequency.h"
#include "tree.h"
#include "huffman_code.h"

class Encoder : public Huffman {
public:
    Encoder() = delete;
    explicit Encoder(Frequency const &frequency);
    Code encode_segment(std::vector<char> const &data);
    Code encode_segment(std::string const &data);
    Code encode_char(char c);
};

#endif //HUFFMAN_LIB_ENCODER_H
