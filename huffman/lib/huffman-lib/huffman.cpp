//
// Created by Andrew Berlin on 5/6/18.
//

#include "include/huffman.h"
#include <stdexcept>

Huffman::Huffman(Frequency frequency) {
    for (auto x : frequency.get_data()) {
        auto t = ptr(new Node(x.first, x.second));
        q.push(t);
    }
    build_tree();
}

void Huffman::build_tree() {
    if (q.empty()) {
        return;
    }

    if (q.size() == 1) {
        root = ptr(new Node(0, q.top(), nullptr));
    } else {
        while (q.size() > 1) {
            auto l = q.top();
            q.pop();
            auto r = q.top();
            q.pop();

            auto top = ptr(new Node(l->freq + r->freq, l, r));
            q.push(top);
        }
        root = q.top();
    }
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

Huffman::Huffman(std::unordered_map<char, Code> codes) {
    root = ptr(new Node(0, 0));
    for (auto c : codes) {
        auto cur = root;

        for (size_t i = 0; i < c.second.size(); i++) {
            bool b = c.second.get(i);

            if (b) {
                if (cur->right == nullptr) {
                    cur->right = ptr(new Node(0, 0));
                }
                cur = cur->right;
            } else {
                if (cur->left == nullptr) {
                    cur->left = ptr(new Node(0, 0));
                }
                cur = cur->left;
            }
        }
        cur->data = c.first;
    }
}
