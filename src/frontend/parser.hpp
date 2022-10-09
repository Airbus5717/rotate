#pragma once

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
    // NOTE(5717): GL Variables must be const
    // and at comptime known their value
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
    u8 deep{0};
    u8 count{0};
};

struct AstFn
{
    AstBlock block;
};

class Parser
{
    // not owned
    Ast ast;
    std::vector<Token> *tokens;
    u32 idx;

    u8 parse_director();
    // global
    u8 parse_import();
    u8 parse_function();
    u8 parse_gl_var();
    u8 parse_struct();
    u8 parse_enum();
    // non global
    u8 parse_block();

    // utils
    Token current() const;
    Token past() const;
    Token peek() const;

  public:
    Parser(Lexer *);
    ~Parser();
    u8 parse_lexer();
}; // Parser

} // namespace rotate
