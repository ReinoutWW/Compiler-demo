#pragma once

class Parser {
public:
    inline explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens))
    {

    }

private:
    inline [[nodiscard]] std::optional<Token> peak(int ahead = 1) const {
        if (m_index+ahead > m_tokens.size()) {
            return {};
        } else {
            return m_tokens.at(m_index);
        }
    }

    inline Token consume() {
        return m_tokens.at(m_index++);
    }

    const std::vector<Token> m_tokens;
    size_t m_index = 0;
};