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

class BufferedWriter {
public:
    BufferedWriter() = delete;
    BufferedWriter(std::string const &file, bool encode);
    ~BufferedWriter();

    void put_char(uint8_t c);
    void put_short(uint16_t x);
    void put_vector(std::vector<uint8_t> v);
    void put_bit(bool b);
private:
    uint8_t cur_char_size = 0;
    uint8_t cur_char = 0;
    std::ofstream stream;
    size_t cnt = 0;
    bool encode = false;
    char buffer[BUFFER_SIZE];
private:
    void write_buffer();
};

#endif //HUFFMAN_BUFFERED_WRITER_H
