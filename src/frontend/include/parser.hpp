#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"

namespace rotate_parser
{
using namespace rotate;

struct gl_import_t {
};

struct gl_var_t {
};

struct gl_struct_t {
};

struct gl_enum_t {
};

struct gl_function_t {
};

enum gl_stmt_type: u8 {
   gl_import,
   gl_var,
   gl_function,
   gl_struct,
   gl_enum,
};

struct gl_stmt {
  gl_stmt_type type;
  union stmt {
     
  };
};

class Parser
{
  Lexer *lexer;
  public:
    Parser(Lexer *lexer);
    ~Parser();
};

} // namespace rotate

#endif // ROTATE_PARSER
