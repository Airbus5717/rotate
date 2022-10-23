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
    // NOTE(5717): error handling in parser
    // The parser will stop at the first error occured
    return parse_director();
}

u8 Parser::parse_director()
{
    while (true)
    {
        TODO("parser Implementation");
        break;
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
