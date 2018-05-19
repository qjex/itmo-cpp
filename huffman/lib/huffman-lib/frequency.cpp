//
// Created by Andrew Berlin on 5/6/18.
//

#include "include/frequency.h"

void Frequency::add_char(uint8_t c, unsigned long long cnt) {
    data[c] += cnt;
}

void Frequency::add_char(uint8_t c) {
    add_char(c, 1);
}

std::array<unsigned long long, 256> const &Frequency::get_data()const {
    return data;
}

Frequency::Frequency(std::string s) {
    for (char c : s) {
        add_char(c);
    }
}
