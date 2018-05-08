//
// Created by Andrew Berlin on 5/6/18.
//

#include "include/decoder.h"
#include <stdexcept>

Decoder::Decoder(Frequency const &frequency) : huffman(frequency), cur_node(huffman.root) {
}

std::vector<char> Decoder::decode(Code const &code) {
    std::vector<char> res;
    cur_node = huffman.root;

    for (size_t i = 0; i < code.size(); i++) {
        bool b = code.get(i);

        go(b);
        if (is_ready()) {
            res.push_back(get_char());
        }
    }
    return res;
}
void Decoder::go(bool b) {
    assert(cur_node != nullptr);

    if (cur_node->is_char()) {
        cur_node = huffman.root;
    }

    if (b) {
        cur_node = cur_node->right;
    } else {
        cur_node = cur_node->left;
    }
}
bool Decoder::is_ready() {
    assert(cur_node != nullptr);
    return cur_node->is_char();
}
char Decoder::get_char() {
    if (cur_node->is_char()) {
        return cur_node->data;
    }
    throw std::runtime_error("Can' get char from current stream of bits");
}

Decoder::Decoder(std::unordered_map<char, Code> codes) : huffman(codes), cur_node(huffman.root){

}
