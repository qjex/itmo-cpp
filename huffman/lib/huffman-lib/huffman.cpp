//
// Created by Andrew Berlin on 5/6/18.
//

#include "include/huffman.h"

Huffman::Huffman(Frequency frequency) {
    for (auto x : frequency.get_data()) {
        auto t = ptr(new Node(x.first, x.second));
        q.push(t);
    }
    build_tree();
}

void Huffman::build_tree() {
    while (q.size() != 1) {
        auto l = q.top();
        q.pop();
        auto r = q.top();
        q.pop();

        auto top = ptr(new Node(l->freq + r->freq, l, r));
        q.push(top);
    }
    root = q.top();
    q.pop();

    Code code;
    store_codes(root, code);
}

void Huffman::store_codes(ptr const &v, Code &code) {
    if (v == nullptr) {
        return;
    }

    if (v->is_char()) {
        codes[v->data] = code;
        return;
    }

    code.add_bit(0);
    store_codes(v->left, code);
    code.pop_bit();
    code.add_bit(1);
    store_codes(v->right, code);
    code.pop_bit();
}

std::unordered_map<char, Code> Huffman::get_codes() {
    return codes;
}