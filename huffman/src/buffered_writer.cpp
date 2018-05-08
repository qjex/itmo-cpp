//
// Created by Andrew Berlin on 5/7/18.
//

#include "buffered_writer.h"

BufferedWriter::BufferedWriter(std::string const &file, bool encode) : stream(file), encode(encode) {
}

BufferedWriter::~BufferedWriter() {
    char sz = 0;
    if (cur_char_size != 0) {
        sz = cur_char_size;
        char pv = sz;
        while (pv < CHAR_BIT) {
            put_bit(false);
            pv++;
        }
    }
    if (encode) put_char(sz);

    write_buffer();
    stream.close();
}

void BufferedWriter::write_buffer() {
    stream.write(buffer, cnt);
    cnt = 0;
}

void BufferedWriter::put_char(char c) {
    if (cnt == WRITER_BUFFER_SIZE) {
        write_buffer();
    }
    buffer[cnt] = c;
    cnt++;
}

void BufferedWriter::put_short(short x) {
    put_char(static_cast<char>(x >> CHAR_BIT));
    put_char(static_cast<char>(x & UINT8_MAX));
}

void BufferedWriter::put_vector(std::vector<char> v) {
    for (auto x : v) {
        put_char(x);
    }
}

void BufferedWriter::put_bit(bool b) {
    cur_char_size++;
    cur_char <<= 1;
    cur_char += b;
    if (cur_char_size == CHAR_BIT) {
        put_char(cur_char);
        cur_char_size = 0;
        cur_char = 0;
    }

}

