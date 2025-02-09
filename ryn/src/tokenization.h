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
    eq,
    plus,
    star,
    minus,
    fslash,
    open_curly,
    close_curly,
    if_
};

std::optional<int> bin_prec(TokenType type) {
    switch (type) {
        case TokenType::plus:
        case TokenType::minus:
            return 0;
        case TokenType::star:
        case TokenType::fslash:
            return 1;
        default:
            return std::nullopt;
    }
}

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
                }
                else if (buffer == "let") {
                    tokens.push_back({TokenType::let});
                    buffer.clear();
                }
                else if (buffer == "if") {
                    tokens.push_back({TokenType::if_});
                    buffer.clear();
                }
                else {
                    tokens.push_back({ .type = TokenType::ident, .value = buffer });
                    buffer.clear();
                }
            }
            else if (std::isdigit(peek().value())) {
                buffer.push_back(consume());
                while (peek().has_value() && std::isdigit(peek().value())) {
                    buffer.push_back(consume());
                }
                tokens.push_back(Token(TokenType::int_lit, buffer));
                buffer.clear();
            }
            else if (peek().value() == '(' ) {
                consume();
                tokens.push_back({TokenType::open_paren});
            }
            else if (peek().value() == ')') {
                consume();
                tokens.push_back({TokenType::close_paren});
            }
            else if (peek().value() == ';') {
                consume();
                tokens.push_back(Token(TokenType::semi));
            }
            else if (peek().value() == '=') {
                consume();
                tokens.push_back(Token(TokenType::eq));
            }
            else if (peek().value() == '+') {
                consume();
                tokens.push_back(Token(TokenType::plus));
            }
            else if (peek().value() == '*') {
                consume();
                tokens.push_back(Token(TokenType::star));
            }
            else if (peek().value() == '-') {
                consume();
                tokens.push_back(Token(TokenType::minus));
            }
            else if (peek().value() == '/') {
                consume();
                tokens.push_back(Token(TokenType::fslash));
            }
            else if (peek().value() == '{') {
                consume();
                tokens.push_back(Token(TokenType::open_curly));
            }
            else if (peek().value() == '}') {
                consume();
                tokens.push_back(Token(TokenType::close_curly));
            }
            else if (std::isspace(peek().value())) {
                consume();
            }
            else {
                std::cerr << "You messed up!" << std::endl;
                exit(EXIT_FAILURE);
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