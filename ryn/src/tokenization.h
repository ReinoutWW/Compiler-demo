#pragma once

#include <string>
#include <strings.h>

enum class TokenType {
    _exit,
    _int_lit,
    _semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};


class Tokenizer {
public:
    inline explicit Tokenizer(std::string src)
        : m_src(std::move(src))
    {

    }

    inline std::vector<Token> tokenize() {
        std::string buffer;
        std::vector<Token> tokens;
        while (peak().has_value()) {
            if (std::isalpha(peak().value())) {
                buffer.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value())) {
                    buffer.push_back(consume());


                }
                if (buffer == "exit") {
                    tokens.push_back({TokenType::_exit});
                    buffer.clear();
                    continue;
                }
                else {
                    std::cerr << "You messed up somewhere (best error you'll get)" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if (std::isdigit(peak().value())) {
                buffer.push_back(consume());
                while (peak().has_value() && std::isdigit(peak().value())) {
                    buffer.push_back(consume());
                    continue;
                }
                tokens.push_back(Token(TokenType::_int_lit, buffer));
                buffer.clear();
            }
            else if (peak().value() == ';') {
                consume();
                tokens.push_back(Token(TokenType::_semi));
                continue;
            }
            else if (std::isspace(peak().value())) {
                consume();
                continue;
            }
            else {
                std::cerr << "You messed up somewhere (best error you'll get)" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return tokens;
    }

private:
    [[nodiscard]] std::optional<char> peak(int ahead = 1) const {
        if (m_index+ahead > m_src.length()) {
            return {};
        } else {
            return m_src.at(m_index);
        }
    }

    inline char consume() {
        return m_src.at(m_index++);
    }

    const std::string m_src;
    int m_index = 0;
};