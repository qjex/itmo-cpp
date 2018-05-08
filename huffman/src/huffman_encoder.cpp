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
    auto codes = encoder.get_codes();

    writer.put_short(static_cast<short>(codes.size()));
    for (auto c : codes) {
        writer.put_char(c.first);
        writer.put_short(static_cast<short>(c.second.size()));
        writer.put_short(static_cast<short>(c.second.get_data().size()));
        writer.put_vector(c.second.get_data());
    }
}

void encode(Encoder &encoder, BufferedReader &reader, BufferedWriter &writer) {
    write_header(encoder, writer);

    while (reader.can_read()) {
        auto code = encoder.encode_char(reader.read_char());

        for (size_t i = 0; i < code.size(); i++) {
            writer.put_bit(code.get(i));
        }
    }
}