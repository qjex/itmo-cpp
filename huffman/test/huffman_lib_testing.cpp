//
// Created by Andrew Berlin on 5/7/18.
//

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>
#include <gtest/gtest.h>

#include "encoder.h"
#include "decoder.h"
#include "frequency.h"
#include "huffman_code.h"

using namespace std;

TEST(correctness, code) {
    Code code;
    size_t sz = 18;

    for (size_t i = 0; i < sz; i++) {
        code.add_bit(1);
    }

    EXPECT_EQ(sz, code.size());
    EXPECT_EQ(string(sz, '1'), code.to_string());
}

string rotate(string text) {
    Frequency frequency(text);
    Encoder encoder(frequency);
    Decoder decoder(frequency);
    string res;
    for (char c : decoder.decode(encoder.encode_segment(text))) res += c;
    return res;
}

TEST(correctness, simple) {
    string text = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";
    EXPECT_EQ(text, rotate(text));
}

TEST(correctness, patrial_decode) {
//    string text = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";
//    Frequency frequency(text);
//    Encoder encoder(frequency);
//    Decoder decoder(frequency);
//    auto code = encoder.encode_segment(text);
//
//    Code part1;
//    for (size_t i = 0; i < code.size() / 2; i++) {
//        part1.add_bit(code.get(i));
//    }

}
