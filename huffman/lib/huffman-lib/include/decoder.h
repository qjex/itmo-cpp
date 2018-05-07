//
// Created by Andrew Berlin on 5/6/18.
//

#ifndef HUFFMAN_LIB_DECODERR_H
#define HUFFMAN_LIB_DECODERR_H

#include <vector>

#include "huffman_code.h"
#include "tree.h"
#include "huffman.h"

class Decoder {
public:
    Decoder() = delete;
    Decoder(Frequency const &frequency);
    std::vector<char> decode(Code const& code);
    void go(bool b);
    bool is_ready();
    char get_char();
private:
    Huffman huffman;
    ptr cur_node;
};

#endif //HUFFMAN_LIB_DECODERR_H
