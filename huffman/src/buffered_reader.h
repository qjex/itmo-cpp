//
// Created by Andrew Berlin on 5/7/18.
//

#ifndef HUFFMAN_BUFFERED_READER_H
#define HUFFMAN_BUFFERED_READER_H

#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <climits>

#include "consts.h"

class BufferedReader {
public:
    BufferedReader() = delete;
    explicit BufferedReader(std::string file);
    ~BufferedReader();
    bool can_read();
    uint8_t read_char();
    uint16_t read_short();
    std::vector<uint8_t> read_vector(size_t sz);
    void reset();
private:
    std::ifstream stream;
    size_t pos = 0;
    size_t cnt_block = 0;
    char buffer[BUFFER_SIZE];
private:
    void read_buff();
};

#endif //HUFFMAN_BUFFERED_READER_H
