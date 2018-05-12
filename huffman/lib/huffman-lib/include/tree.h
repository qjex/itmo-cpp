//
// Created by Andrew Berlin on 5/7/18.
//

#ifndef HUFFMAN_LIB_TREE_H
#define HUFFMAN_LIB_TREE_H

#include <memory>
struct Node;

typedef std::shared_ptr<Node> ptr;

struct Node {
    ptr left;
    ptr right;
    uint8_t data = 0;
    int freq;

    Node(uint8_t c, int freq) {
        this->data = c;
        this->freq = freq;
        left = right = nullptr;
    }

    bool is_char() {
        return left == nullptr && right == nullptr;
    }

    Node(int freq, ptr const &left, ptr const& right) : freq(freq), left(left), right(right) {};
};

#endif //HUFFMAN_LIB_TREE_H
