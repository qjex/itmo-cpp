cmake_minimum_required(VERSION 2.8)

project(asm)

set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS "asm")
set(CMAKE_ASM_COMPILE_OBJECT "nasm -f macho64 -o <OBJECT> <SOURCE>")
SET(CMAKE_ASM_LINK_EXECUTABLE "/usr/bin/ld -macosx_version_min 10.7.0 -lSystem <OBJECTS> -o <TARGET>")
enable_language(ASM)

add_executable(hello hello.asm)
add_executable(add add.asm)
add_executable(sub sub.asm)
add_executable(mul mul.asm)