//
// Created by Andrew Berlin on 5/7/18.
//

#include "buffered_writer.h"

BufferedWriter::BufferedWriter(std::string const &file, bool encode) : stream(file, std::ofstream::binary), encode(encode) {
}

BufferedWriter::~BufferedWriter() {
    write_buffer();
    stream.close();
}

void BufferedWriter::write_buffer() {
    stream.write(buffer, cnt);
    cnt = 0;
}

void BufferedWriter::put_char(uint8_t c) {
    if (cnt == BUFFER_SIZE) {
        write_buffer();
    }
    buffer[cnt] = c;
    cnt++;
}

void BufferedWriter::put_short(uint16_t x) {
    put_char(static_cast<uint8_t>(x >> BLOCK_LEN));
    put_char(static_cast<uint8_t>(x & UINT8_MAX));
}

void BufferedWriter::put_vector(std::vector<uint8_t> v) {
    for (auto x : v) {
        put_char(x);
    }
}

void BufferedWriter::put_bit(bool b) {
    last_byte_full = false;
    cur_char_size++;
    cur_char <<= 1;
    cur_char += b;
    if (cur_char_size == BLOCK_LEN) {
        put_char(cur_char);
        cur_char_size = 0;
        cur_char = 0;
        last_byte_full = true;
    }

}
void BufferedWriter::complete_byte() {
    if (cur_char_size != 0) {
        last_byte_size = cur_char_size;
        uint8_t pv = cur_char_size;
        while (pv < BLOCK_LEN) {
            put_bit(false);
            pv++;
        }
    } else {
        last_byte_size = last_byte_full ? BLOCK_LEN : 0;
    }
    put_char(last_byte_size);
}
