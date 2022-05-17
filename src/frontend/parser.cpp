#include "include/parser.hpp"

namespace rotate
{

//
Parser::Parser(Lexer *lexer) : lexer(lexer)
{
    ASSERT_NULL(this->lexer, "lexer passed is null");
}


} // namespace rotate
