#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

#include "./parser.h"
#include "./tokenization.h"
#include "./generation.h"

// https://www.youtube.com/watch?v=vcSijrRsrY0


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect usage. The correct usage:" << std::endl;
        std::cout << "ryn <input.ryn>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<NodeProg> prog = parser.parse_prog();

    if (!prog.has_value()) {
        std::cerr << "Invalid program" << std::endl;
        exit(EXIT_FAILURE);
    } {
        Generator generator(prog.value());
        std::fstream file("out.asm", std::ios::out);
        file << generator.gen_prog();
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}