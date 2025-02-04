#pragma once

#include <string>

enum class TokenType {
    exit,
    int_lit,
    semi,
    open_paren,
    close_paren,
    ident,
    let,
    eq
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
                    tokens.push_back({TokenType::exit});
                    buffer.clear();
                    continue;
                }
                else if (buffer == "let") {
                    tokens.push_back({TokenType::let});
                    buffer.clear();
                    continue;
                }
                else {
                    tokens.push_back({ .type = TokenType::ident, .value = buffer });
                    buffer.clear();
                    continue;
                }
            }
            else if (std::isdigit(peek().value())) {
                buffer.push_back(consume());
                while (peek().has_value() && std::isdigit(peek().value())) {
                    buffer.push_back(consume());
                    continue;
                }
                tokens.push_back(Token(TokenType::int_lit, buffer));
                buffer.clear();
            }
            else if (peek().value() == '(' ) {
                consume();
                tokens.push_back({TokenType::open_paren});
                continue;
            }
            else if (peek().value() == ')') {
                consume();
                tokens.push_back({TokenType::close_paren});
                continue;
            }
            else if (peek().value() == ';') {
                consume();
                tokens.push_back(Token(TokenType::semi));
                continue;
            }
            else if (peek().value() == '=') {
                consume();
                tokens.push_back(Token(TokenType::eq));
                continue;
            }
            else if (std::isspace(peek().value())) {
                consume();
                continue;
            }
            else {
                tokens.push_back({TokenType::ident, buffer});
                buffer.clear();
                continue;
            }
        }
        m_index = 0;
        return tokens;
    }

private:
    [[nodiscard]] std::optional<char> peek(int offset = 0) const {
        if (m_index + offset >= m_src.length()) {
            return {};
        } else {
            return m_src.at(m_index + offset);
        }
    }

    inline char consume() {
        return m_src.at(m_index++);
    }

    const std::string m_src;
    int m_index = 0;
};