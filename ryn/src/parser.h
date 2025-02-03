#pragma once

#include "tokenization.h"

struct NodeExpr {
    Token int_lit;
};

struct NodeExit {
    NodeExpr expr;
};

class Parser {
public:
    inline explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens))
    {

    }

    std::optional<NodeExpr> parse_expr() {
        if (peak().has_value() && peak().value().type == TokenType::_int_lit) {
            return std::make_optional(NodeExpr{ consume() });
        }

        return std::nullopt;
    }

    std::optional<NodeExit> parse() {
        while (peak().has_value()) {
            if (peak().value().type == TokenType::_exit) {
                consume();
                if (auto node_expr = parse_expr()) {
                    std::optional<NodeExit> exit_node = NodeExit{.expr = node_expr.value()};
                } else {
                    std::cerr << "Invalid Expression" << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (!peak().has_value() || peak().value().type != TokenType::_semi) {
                    std::cerr << "Invalid Expression" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }

        m_index = 0;
        return NodeExit();
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