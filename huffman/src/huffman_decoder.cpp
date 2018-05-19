//
// Created by Andrew Berlin on 5/8/18.
//

#include <array>
#include <vector>
#include <stdexcept>

#include "huffman_decoder.h"
#include "huffman_code.h"

Decoder read_header(BufferedReader &reader) {
    std::array<Code, 256> codes;

    for (uint16_t i = 0; i < 256; i++) {
        uint8_t c = reader.read_char();
        uint16_t sz = reader.read_short();
        uint16_t data_sz = reader.read_short();
        std::vector<uint32_t> v = reader.read_vector(static_cast<size_t>(data_sz));

        codes[c] = Code(static_cast<size_t>(sz), v);
    }
    return Decoder(codes);
}

void decode(BufferedReader &reader, BufferedWriter &writer) {
    Decoder decoder = read_header(reader);
    uint32_t cur = reader.read_int();

    if (!reader.can_read()) {
        return;
    }

    do {
        uint32_t nxt = reader.read_int();
        uint32_t cnt_last = BLOCK_LEN;

        if (!reader.can_read()) {
            cnt_last = nxt;
        }

        for (uint32_t i = 0; i < cnt_last; i++) {
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