#include "include/parser.hpp"

namespace rotate_parser
{
using namespace rotate;
//
Parser::Parser(Lexer *lexer) : lexer(lexer)
{
    UNUSED(lexer);
}

//
Parser::~Parser() = default;

} // namespace rotate_parser
