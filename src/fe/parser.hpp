#pragma once

#include "errmsgs.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include "type.hpp"

namespace rotate
{
//! Keep code as simple as possible
//! for scope level bases

// Expressions
struct Expr;
struct LitExpr;
struct BinaryExpr;
struct UnaryExpr;

// Ast and Symbol table
struct Ast;
struct AstImport;
struct AstGlVar;
struct AstStruct;
struct AstEnum;
struct AstFn;

/*
 * Expressions
 */

enum class BinaryOpType
{
    // TODO: add all
    add,     // +
    sub,     // -
    mult,    // *
    div,     // '/'
    eqleql,  // ==
    greater, // >
    greql,   // >=
    less,    // <
    leseql,  // <=
};

enum class UnaryOpType
{
    logical_negate,   // '!'
    numerical_negate, // -
    new_opr,          // new (only allowed as expressions)
    delete_opr,       // delete (only allowed as stmts)
};

struct LitExpr
{ //
    // LiteralExprType type;
    UINT val_idx;
};

struct UnaryExpr
{
    UnaryOpType op;
    Expr *expr;
};

struct BinaryExpr
{
    BinaryOpType op;
    Expr *left, *right;
};

struct Expr
{
    // TODO: Make a union
    LitExpr literal;
    UnaryExpr unary;
    BinaryExpr binary;
};

/*
 * Abstract Syntax tree
 */
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

    // 0  1  2     34    56
    // io :: import("std");
    // ^^id          ^^^value
    // index is the index of string token
    // It requires a single string Token for path
    // the path must be known at compile time
    UINT id_idx;
    UINT val_idx;

    AstImport(UINT val_idx) : id_idx(val_idx - 4), val_idx(val_idx)
    {
    }

    ~AstImport() = default;
};

struct AstGlVar
{
    UINT id_idx;
    // Type type;
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
    AstBlock block;
};

class Parser
{
    // not owned (weak ptr)
    file_t *file;
    //
    Ast ast;
    std::vector<Token> *tokens;
    PrsErr error;
    UINT idx;

    u8 parse_starter();
    // global stmts
    u8 parse_import();
    u8 parse_function();
    u8 parse_gl_var();
    u8 parse_struct();
    u8 parse_enum();
    // non global
    u8 parse_block();

    // expressions
    u8 parse_expr();
    u8 parse_bin_expr();
    u8 parse_unary_expr();
    u8 parse_grouping_expr();
    u8 parse_literal();

    // utils
    Token current() const;
    Token past() const;
    Token peek() const;
    void advance();

    //
    u8 parse_error_expect_token(TknType);
    PrsErr convert_tkn_type_to_parse_error(TknType);
    u8 parser_error(PrsErr);
    const char *parser_error_msg(PrsErr);
    const char *parser_error_advice(PrsErr);
    u8 parse_error_use_global_err();

    // Parser exports
    std::vector<AstImport> imports;
    std::vector<AstFn> functions;
    std::vector<AstEnum> enums;
    std::vector<AstStruct> structs;
    std::vector<AstGlVar> glVars;

  public:
    Parser(file_t *, Lexer *);
    ~Parser();
    u8 parse_lexer();
}; // Parser

} // namespace rotate
