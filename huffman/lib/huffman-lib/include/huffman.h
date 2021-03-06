//
// Created by Andrew Berlin on 5/6/18.
//

#ifndef HUFFMAN_LIB_HUFFMAN_H
#define HUFFMAN_LIB_HUFFMAN_H

#include <queue>
#include <array>
#include <climits>

#include "frequency.h"
#include "huffman_code.h"
#include "tree.h"

class Huffman {
public:
    Huffman() = delete;
    explicit Huffman(Frequency const &frequency);
    explicit Huffman(std::array<Code, 256> const &codes);
    std::array<Code, 256> const &get_codes();
    ptr root;
private:
    struct comparator {
        bool operator()(ptr const &l, ptr const &r) {
            return l->freq > r->freq;
        }
    };
    std::priority_queue<ptr, std::vector<ptr>, comparator> q;
protected:
    std::array<Code, 256> codes;
private:
    void build_tree();
    void store_codes(ptr const &root, Code& code);
};

#endif //HUFFMAN_LIB_HUFFMAN_H
