//
// Created by Andrew Berlin on 5/7/18.
//

#include "buffered_writer.h"
#include <algorithm>

BufferedWriter::BufferedWriter(std::string const &file, bool encode) : stream(file, std::ofstream::binary),
                                                                       encode(encode) {
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
    put_char(static_cast<uint8_t>(x >> 8));
    put_char(static_cast<uint8_t>(x & UINT8_MAX));
}

void BufferedWriter::put_vector(std::vector<uint32_t> const &v) {
    for (auto x : v) {
        put_int(x);
    }
}

void BufferedWriter::put_code(Code const &code) {
    for (size_t i = 0; i < code.size(); i++) {
        put_bit(code.get(i));
    }
//    auto data = code.get_data();
//    size_t code_size = code.size();
//
//    for (size_t i = 0; i < data.size(); i++) {
//        size_t cur_block_size = (i + 1 != data.size() ? 32 : code_size - (32 * i));
//        small_buff <<= cur_block_size;
//        small_buff &= (data[i] >> (32 - cur_block_size));
//        small_buff |= (data[i] >> (32 - cur_block_size));
//        small_buff_size += cur_block_size;
//
//        while (small_buff_size > 32) {
//            put_int(static_cast<uint32_t>((small_buff >> (small_buff_size - 32)) & UINT32_MAX));
//            small_buff <<= 32;
//            small_buff_size -= 32;
//        }
//    }

}
//
void BufferedWriter::put_bit(bool b) {
    last_byte_full = false;
    cur_char_size++;
    cur_char <<= 1;
    cur_char += b;
    if (cur_char_size == BLOCK_LEN) {
        put_int(cur_char);
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
    put_int(last_byte_size);
}

//void BufferedWriter::complete_byte() {
//    if (small_buff_size != 0) {
//        put_int(static_cast<uint32_t>((small_buff << (32 - small_buff_size)) & UINT32_MAX));
//    }
//    put_int(small_buff_size);
//}

void BufferedWriter::put_int(uint32_t x) {
    put_short(static_cast<uint16_t>(x >> 16));
    put_short(static_cast<uint16_t>(x & UINT16_MAX));
}
