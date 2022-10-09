#include "lexer.hpp"

namespace rotate
{
//! Keep code as simple as possible
//! for scope level bases
//
//
//
//
struct SymbolTable
{
};

struct Ast;
struct AstImport;
struct AstGlVar;
struct AstStruct;
struct AstEnum;
struct AstFn;

struct Ast
{
    std::vector<AstImport> imports;
    std::vector<AstGlVar> gl_variables;
    std::vector<AstStruct> structs;
    std::vector<AstEnum> enums;
    std::vector<AstFn> functions;
};

struct AstImport
{
    // io :: import("std")
    // ^^id          ^^^value
    Token &id;
    Token &value;
};

struct AstGlVar
{
    // id, (comptime)value, type
};

struct AstStruct
{
    // id, params(),
};

struct AstEnum
{
};

struct AstBlock
{
};

struct AstFn
{
};

class Parser
{

    u8 parse_director();
    // global
    u8 parse_import();
    u8 parse_function();
    u8 parse_gl_var();
    u8 parse_struct();
    u8 parse_enum();
    // non global
    u8 parse_block();

  public:
    Parser();
    ~Parser();
    u8 parse_lexer(Lexer *);
}; // Parser

} // namespace rotate
