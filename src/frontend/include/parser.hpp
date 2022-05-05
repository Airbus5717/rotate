#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"

namespace rotate
{

class Parser
{
  Lexer *lexer;
  public:
    Parser(Lexer *lexer);
    ~Parser();
};

} // namespace rotate

#endif // ROTATE_PARSER
