//
// Created by Andrew Berlin on 5/7/18.
//

#ifndef HUFFMAN_HUFFMAN_ENCODER_H
#define HUFFMAN_HUFFMAN_ENCODER_H

#include "encoder.h"
#include "buffered_reader.h"
#include "buffered_writer.h"
#include "frequency.h"

void write_header(Encoder &encoder, BufferedWriter &writer);

void encode(Encoder &encoder, BufferedReader &reader, BufferedWriter &writer);

Frequency calc_frequency(BufferedReader &reader);

#endif //HUFFMAN_HUFFMAN_ENCODER_H
