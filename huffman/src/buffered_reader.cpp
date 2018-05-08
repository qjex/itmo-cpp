//
// Created by Andrew Berlin on 5/7/18.
//

#include "buffered_reader.h"

BufferedReader::BufferedReader(std::string file) : stream(file, std::ios::in | std::ifstream::binary){
    if (stream.fail()) {
        stream.close();
        throw std::runtime_error("Can't read file " + file);
    }
}

BufferedReader::~BufferedReader() {
    stream.close();
}

bool BufferedReader::can_read() {
    return !(stream.peek() == EOF && pos == cnt_block);
}

char BufferedReader::read_char() {
    if (!can_read()) {
        throw std::runtime_error("Unexpected end of file");
    }

    if (pos == cnt_block) {
        read_buff();
    }
    return buffer[pos++];
}

void BufferedReader::read_buff() {
    stream.read(buffer, READER_BUFFER_SIZE);
    cnt_block = static_cast<size_t>(stream.gcount());
    pos = 0;
}

short BufferedReader::read_short() {
    short x = read_char();
    x <<= 8;
    x += read_char();
    return x;
}

std::vector<char> BufferedReader::read_vector(size_t sz) {
    std::vector<char> res;
    for (size_t i = 0; i < sz; i++) {
        res.push_back(read_char());
    }
    return res;
}

void BufferedReader::reset() {
    pos = 0;
    cnt_block = 0;
    stream.clear();
    stream.seekg(0, std::ios::beg);
}


