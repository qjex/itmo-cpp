//
// Created by Andrew Berlin on 5/7/18.
//

#ifndef HUFFMAN_LIB_HUFFMAN_CODE_H
#define HUFFMAN_LIB_HUFFMAN_CODE_H

#include <vector>
#include <string>

struct Code {
    Code() : data(0), _size(0) {}
    Code(size_t sz, const std::vector<char> &d) : _size(sz), data(d) {};
    void add_bit(bool b);
    void pop_bit();
    bool get(size_t ind) const;
    size_t size() const;
    void add(Code &code);
    std::string to_string();
    std::vector<char> get_data();
private:
    std::vector<char> data;
    size_t _size;
private:
    size_t get_pos(size_t ind) const;
};

#endif //HUFFMAN_LIB_HUFFMAN_CODE_H
