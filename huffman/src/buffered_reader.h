//
// Created by Andrew Berlin on 5/7/18.
//

#ifndef HUFFMAN_BUFFERED_READER_H
#define HUFFMAN_BUFFERED_READER_H

#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>

static const size_t READER_BUFFER_SIZE = 524288;

class BufferedReader {
public:
    BufferedReader() = delete;
    explicit BufferedReader(std::string file);
    ~BufferedReader();
    bool can_read();
    char read_char();
    short read_short();
    std::vector<char> read_vector(size_t sz);
    void reset();
private:
    std::ifstream stream;
    size_t pos = 0;
    size_t cnt_block = 0;
    char buffer[READER_BUFFER_SIZE];
private:
    void read_buff();
};

#endif //HUFFMAN_BUFFERED_READER_H
