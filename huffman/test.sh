#!/bin/sh

./huffman -e $1 "$1-e"
./huffman -d "$1-e" "$1-d"
diff "$1-d" $1