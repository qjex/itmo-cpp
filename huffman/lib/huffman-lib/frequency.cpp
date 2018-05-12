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

std::vector<occur> Frequency::get_data()const {
    std::vector<occur> res;
    for (auto p : data) {
        res.emplace_back(p);
    }
    return res;
}

size_t Frequency::size() {
    return data.size();
}

Frequency::Frequency(std::string s) {
    for (char c : s) {
        add_char(c);
    }
}
