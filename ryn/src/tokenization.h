#pragma once

#include <string>

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
        while (peek().has_value()) {
            if (std::isalpha(peek().value())) {
                buffer.push_back(consume());
                while (peek().has_value() && std::isalnum(peek().value())) {
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
            else if (std::isdigit(peek().value())) {
                buffer.push_back(consume());
                while (peek().has_value() && std::isdigit(peek().value())) {
                    buffer.push_back(consume());
                    continue;
                }
                tokens.push_back(Token(TokenType::_int_lit, buffer));
                buffer.clear();
            }
            else if (peek().value() == ';') {
                consume();
                tokens.push_back(Token(TokenType::_semi));
                continue;
            }
            else if (std::isspace(peek().value())) {
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
    [[nodiscard]] std::optional<char> peek(int ahead = 1) const {
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