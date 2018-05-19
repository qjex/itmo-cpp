//
// Created by Andrew Berlin on 5/7/18.
//

#include <unordered_map>

#include "huffman_encoder.h"
#include "huffman_code.h"

Frequency calc_frequency(BufferedReader &reader) {
    Frequency frequency;
    while (reader.can_read()) {
        frequency.add_char(reader.read_char());
    }
    reader.reset();
    return frequency;
}

void write_header(Encoder &encoder, BufferedWriter &writer) {
    auto const& codes = encoder.get_codes();

    for (size_t i = 0; i < 256; i++) {
        auto const &c = codes[i];
        auto const &data = c.get_data();
        writer.put_char(static_cast<uint8_t>(i));
        writer.put_short(static_cast<uint16_t>(c.size()));
        writer.put_short(static_cast<uint16_t>(data.size()));
        writer.put_vector(data);
    }
}

void encode(Encoder &encoder, BufferedReader &reader, BufferedWriter &writer) {
    write_header(encoder, writer);

    while (reader.can_read()) {
        auto const &code = encoder.encode_char(reader.read_char());
        for (size_t i = 0; i < code.size(); i++) {
            writer.put_bit(code.get(i));
        }
    }
    writer.complete_byte();
}