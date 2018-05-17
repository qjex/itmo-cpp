//
// Created by Andrew Berlin on 5/8/18.
//

#include <unordered_map>
#include <vector>
#include <stdexcept>

#include "huffman_decoder.h"
#include "huffman_code.h"

Decoder read_header(BufferedReader &reader) {
    std::unordered_map<uint8_t, Code> codes;
    short size = reader.read_short();

    for (uint16_t i = 0; i < size; i++) {
        uint8_t c = reader.read_char();
        uint16_t sz = reader.read_short();
        uint16_t data_sz = reader.read_short();
        std::vector<uint8_t> v = reader.read_vector(static_cast<size_t>(data_sz));

        codes[c] = Code(static_cast<size_t>(sz), v);
    }
    return Decoder(codes);
}

void decode(BufferedReader &reader, BufferedWriter &writer) {
    Decoder decoder = read_header(reader);
    uint8_t cur = reader.read_char();

    if (decoder.get_codes().empty()) {
        return;
    }

    if (!reader.can_read()) {
        throw std::runtime_error("Decoding file is corrupted");
    }

    do {
        uint8_t nxt = reader.read_char();
        uint8_t cnt_last = BLOCK_LEN;

        if (!reader.can_read()) {
            cnt_last = nxt;
        }

        for (uint8_t i = 0; i < cnt_last; i++) {
            bool b = static_cast<bool>(cur & (1 << (BLOCK_LEN - i - 1)));

            decoder.go(b);
            if (decoder.get_cur_node() == nullptr) {
                throw std::runtime_error("Decoding file is corrupted");
            }
            if (decoder.is_ready()) {
                writer.put_char(decoder.get_char());
            }
        }

        cur = nxt;
    } while (reader.can_read());
}