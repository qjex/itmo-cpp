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
    if (pos == cnt_block) {
        read_buff();
    }
    return cnt_block != 0;
}

uint8_t BufferedReader::read_char() {
    if (!can_read()) {
        throw std::runtime_error("Unexpected end of file");
    }

    if (pos == cnt_block) {
        read_buff();
    }
    return static_cast<uint8_t>(buffer[pos++]);
}

void BufferedReader::read_buff() {
    stream.read(buffer, BUFFER_SIZE);
    cnt_block = static_cast<size_t>(stream.gcount());
    pos = 0;
}

uint16_t BufferedReader::read_short() {
    uint16_t x = read_char();
    x <<= 8;
    x += read_char();
    return x;
}

std::vector<uint32_t> BufferedReader::read_vector(size_t sz) {
    std::vector<uint32_t> res;
    for (size_t i = 0; i < sz; i++) {
        res.push_back(read_int());
    }
    return res;
}

void BufferedReader::reset() {
    pos = 0;
    cnt_block = 0;
    stream.clear();
    stream.seekg(0, std::ios::beg);
}

uint32_t BufferedReader::read_int() {
    uint32_t res = read_short();
    res <<= 16;
    res |= read_short();
    return res;
}

