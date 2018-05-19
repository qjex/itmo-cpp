//
// Created by Andrew Berlin on 5/7/18.
//

#ifndef HUFFMAN_BUFFERED_WRITER_H
#define HUFFMAN_BUFFERED_WRITER_H

#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <climits>

#include "consts.h"
#include "impl_consts.h"
#include <huffman_code.h>

class BufferedWriter {
public:
    BufferedWriter() = delete;
    BufferedWriter(std::string const &file, bool encode);
    ~BufferedWriter();

    void put_char(uint8_t c);
    void put_short(uint16_t x);
    void put_vector(std::vector<uint32_t> const &v);
    void put_code(Code const &code);
    void put_bit(bool b);
    void complete_byte();
private:
    uint64_t small_buff = 0;
    size_t small_buff_size = 0;
    uint32_t cur_char_size = 0;
    uint32_t cur_char = 0;
    uint32_t last_byte_size = 0;
    bool last_byte_full = false;
    std::ofstream stream;
    size_t cnt = 0;
    bool encode = false;
    char buffer[BUFFER_SIZE];
private:
    void write_buffer();
    void put_int(uint32_t x);
};

#endif //HUFFMAN_BUFFERED_WRITER_H
