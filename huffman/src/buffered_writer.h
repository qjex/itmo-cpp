//
// Created by Andrew Berlin on 5/7/18.
//

#ifndef HUFFMAN_BUFFERED_WRITER_H
#define HUFFMAN_BUFFERED_WRITER_H

#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>

static const size_t WRITER_BUFFER_SIZE = 524288;

class BufferedWriter {
public:
    BufferedWriter() = delete;
    BufferedWriter(std::string const &file, bool encode);
    ~BufferedWriter();

    void put_char(char c);
    void put_short(short x);
    void put_vector(std::vector<char> v);
    void put_bit(bool b);
private:
    char cur_char_size = 0;
    char cur_char = 0;
    std::ofstream stream;
    size_t cnt = 0;
    bool encode = false;
    char buffer[WRITER_BUFFER_SIZE];
private:
    void write_buffer();
};

#endif //HUFFMAN_BUFFERED_WRITER_H
