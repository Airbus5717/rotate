#include "parser.hpp"

namespace rotate
{

// constructor
Parser::Parser(Lexer *lexer) : tokens(lexer->getTokens()), idx(0)
{
}

Parser::~Parser() = default;

u8 Parser::parse_lexer()
{
    while (idx < tokens->size())
    {
        auto c = tokens[idx];
        UNUSED(c);
        TODO("implement parser");
    }
    return EXIT_FAILURE;
}

Token Parser::current() const
{
    return tokens->at(idx);
}

Token Parser::past() const
{
    return tokens->at(idx - 1);
}

Token Parser::peek() const
{
    return tokens->at(idx + 1);
}

} // namespace rotate
