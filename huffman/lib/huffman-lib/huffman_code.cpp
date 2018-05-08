//
// Created by Andrew Berlin on 5/7/18.
//

#include "include/huffman_code.h"
#include <limits>

void Code::add_bit(bool b) {
    _size++;
    if (data.size() * CHAR_BIT < _size) {
        data.push_back(0);
    }
    if (b) {
        data.back() ^= (1 << get_pos(_size - 1));
    }
}

size_t Code::get_pos(size_t ind)const {
    ind %= CHAR_BIT;
    return (CHAR_BIT - 1 - ind);
}

void Code::pop_bit() {
    _size--;
    if ((data.size() - 1) * CHAR_BIT >= _size) {
        data.pop_back();
    } else {
        data.back() &= UINT8_MAX ^ (1 << get_pos(_size));
    }
}

bool Code::get(size_t ind)const {
    return static_cast<bool>(data[ind / CHAR_BIT] & (1 << get_pos(ind)));
}
size_t Code::size()const {
    return _size;
}
void Code::add(Code &code) {
    for (size_t i = 0; i < code.size(); i++) {
        add_bit(code.get(i));
    }
}
std::string Code::to_string() {
    std::string res;
    for (size_t i = 0; i < _size; i++) {
        res += '0' + get(i);
    }
    return res;
}

std::vector<char> Code::get_data() {
    return data;
}
