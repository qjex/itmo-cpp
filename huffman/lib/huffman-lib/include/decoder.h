//
// Created by Andrew Berlin on 5/6/18.
//

#ifndef HUFFMAN_LIB_DECODERR_H
#define HUFFMAN_LIB_DECODERR_H

#include <vector>
#include <array>
#include <cassert>
#include "huffman_code.h"
#include "tree.h"
#include "huffman.h"

class Decoder : public Huffman {
public:
    Decoder() = delete;
    explicit Decoder(Frequency const &frequency);
    explicit Decoder(std::array<Code, 256> const &codes);
    std::vector<uint8_t> decode(Code const& code);
    void go(bool b);
    bool is_ready();
    uint8_t get_char();
    ptr get_cur_node();
private:
    ptr cur_node;
};

#endif //HUFFMAN_LIB_DECODERR_H
