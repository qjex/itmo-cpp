#include <iostream>
#include <vector>
#include <set>
#include <ctime>

#include "frequency.h"
#include "buffered_reader.h"
#include "buffered_writer.h"
#include "huffman_encoder.h"
#include "huffman_decoder.h"

using namespace std;

void print_usage() {
    printf("Usage:\nEncode: -e [src] [dst] \nDecode: -d [src] [dst]");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage();
        return 0;
    }

    string mode = argv[1];
    string src_path = argv[2];
    string dst_path = argv[3];

    if (mode != "-e" && mode != "-d") {
        print_usage();
        return 0;
    }

    try {
        BufferedReader reader(src_path);

        if (mode == "-e") {
            BufferedWriter writer(dst_path, true);
            Encoder encoder(calc_frequency(reader));
            encode(encoder, reader, writer);
        } else {
            BufferedWriter writer(dst_path, false);
            decode(reader, writer);
        }

    } catch (std::exception &e) {
        printf("%s ", e.what());
    }
    return 0;
}