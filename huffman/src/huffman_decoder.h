//
// Created by Andrew Berlin on 5/8/18.
//

#ifndef HUFFMAN_HUFFMAN_DECODER_H
#define HUFFMAN_HUFFMAN_DECODER_H

#include "decoder.h"
#include "buffered_reader.h"
#include "buffered_writer.h"
#include "frequency.h"
#include <climits>


Decoder read_header(BufferedReader &reader);

void decode(BufferedReader &reader, BufferedWriter &writer);

#endif //HUFFMAN_HUFFMAN_DECODER_H
