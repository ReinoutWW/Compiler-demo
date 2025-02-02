#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

// https://www.youtube.com/watch?v=vcSijrRsrY0

enum class TokenType {
    _return,
    _int_lit,
    _semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

/// Please forgive me for this code. Part of the vid and will refactor this
/// 'First make it work, then make it beautiful!'
///
/// @param str /
/// @return
std::vector<Token> tokenize(const std::string& str) {
    std::vector<Token> tokens;
    std::string buffer;
    // return 69;

    for (int i = 0; i < str.length(); i++) {
        char c = str.at(i);
        if (std::isalpha(c)) {
            buffer.push_back(c);
            i++;
            while (std::isalnum(str.at(i))) {
                buffer.push_back(str.at(i));
                i++;
            }
            i--;

            if (buffer == "return") {
                tokens.push_back(Token(TokenType::_return, buffer));
                buffer.clear();
                continue;
            } else {
                std::cerr << "You messed up somewhere (best error you'll get)" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if (std::isdigit(c)) {
            buffer.push_back(c);
            i++;
            while (std::isdigit(str.at(i))) {
                buffer.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back(Token(TokenType::_int_lit, buffer));
            buffer.clear();
        }
        else if (c == ';') {
            tokens.push_back(Token(TokenType::_semi, buffer));
        }
        else if (std::isspace(c)) {
            continue;
        }
        else {
            std::cerr << "You messed up somewhere (best error you'll get)" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return tokens;
}

std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream output;

    output << "global _start\nstart:\n";

    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens[i];
        if (token.type == TokenType::_return) {
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::_int_lit) {
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::_semi) {
                    output << "    mov rax, 60\n";
                    output << "    mov rbx, " << tokens.at(i + 1).value.value() << "\n";
                    output << "    syscall";
                }
            }
        }
    }

    return output.str();
}

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

    std::vector<Token> tokens = tokenize(contents);
    {
        std::fstream file("../out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
    }

    return EXIT_SUCCESS;
}