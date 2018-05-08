//
// Created by Andrew Berlin on 5/8/18.
//

#include <unordered_map>
#include <vector>
#include <stdexcept>

#include "huffman_decoder.h"
#include "huffman_code.h"

Decoder read_header(BufferedReader &reader) {
    std::unordered_map<char, Code> codes;
    short size = reader.read_short();

    for (short i = 0; i < size; i++) {
        char c = reader.read_char();
        short sz = reader.read_short();
        short data_sz = reader.read_short();
        std::vector<char> v = reader.read_vector(static_cast<size_t>(data_sz));

        codes[c] = Code(static_cast<size_t>(sz), v);
    }
    return Decoder(codes);
}

void decode(BufferedReader &reader, BufferedWriter &writer) {
    Decoder decoder = read_header(reader);
    char cur = reader.read_char();

    if (!reader.can_read()) {
        throw std::runtime_error("Decoding file is corrupted");
    }

    do {
        char nxt = reader.read_char();
        char cnt_last = 0;

        if (!reader.can_read()) {
            cnt_last = nxt;
        }

        for (char i = 0; i < CHAR_BIT - cnt_last; i++) {
            bool b = static_cast<bool>(cur & (1 << (CHAR_BIT - i - 1)));

            decoder.go(b);
            if (decoder.is_ready()) {
                writer.put_char(decoder.get_char());
            }
        }

        cur = nxt;
    } while (reader.can_read());
}