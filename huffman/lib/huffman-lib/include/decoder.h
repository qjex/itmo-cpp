//
// Created by Andrew Berlin on 5/6/18.
//

#ifndef HUFFMAN_LIB_DECODERR_H
#define HUFFMAN_LIB_DECODERR_H

#include <vector>
#include <unordered_map>
#include <cassert>
#include "huffman_code.h"
#include "tree.h"
#include "huffman.h"

class Decoder {
public:
    Decoder() = delete;
    explicit Decoder(Frequency const &frequency);
    explicit Decoder(std::unordered_map<uint8_t, Code> codes);
    std::vector<uint8_t> decode(Code const& code);
    void go(bool b);
    bool is_ready();
    uint8_t get_char();
private:
    Huffman huffman;
    ptr cur_node;
};

#endif //HUFFMAN_LIB_DECODERR_H
