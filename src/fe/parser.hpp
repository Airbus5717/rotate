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
    TknIdx val_idx;
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

// NOTE(5717): function params are const
struct FuncParam
{
    TknIdx id;
    Type type;
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
    SymbolTable symbol_table;
};

struct AstImport
{

    // 0  1  2     34    56
    // import "std/io" as io;
    //         ^^^^^^val   ^^id
    // index is the index of string token
    // It requires a single string Token for path
    // the path must be known at compile time
    TknIdx id_idx;
    TknIdx val_idx;
    bool aliased;

    AstImport(UINT val_idx) : val_idx(val_idx), aliased(false)
    {
    }

    AstImport(TknIdx id_idx, TknIdx val_idx) : id_idx(id_idx), val_idx(val_idx), aliased(true)
    {
    }

    ~AstImport() = default;
};

struct AstGlVar
{
    TknIdx id_idx;
    Type type;
};

struct AstStruct
{
    TknIdx id;
    // id, params(),
};

struct AstEnum
{
    TknIdx id;
};

struct AstBlock
{
    // TODO
};

struct AstFn
{
    TknIdx id;
    Type return_type;
    AstBlock block;
    // TODO: parameters
};

class Parser
{
    // not owned (weak ptr)
    file_t *file;
    //
    std::vector<Token> *tokens;
    PrsErr error;
    UINT idx;

    // starting point
    u8 parse_starter();

    // global stmts
    u8 parse_import();
    u8 parse_function();
    u8 parse_gl_var();
    u8 parse_struct();
    u8 parse_enum();

    // outline function specific
    u8 parse_block();
    u8 parse_fn_params();
    u8 parse_fn_params_types();

    // statements (inner) function specific
    u8 parse_var_def();
    u8 parse_var_decl();
    u8 parse_while_loop();
    u8 parse_voidfunction_call();
    u8 parse_for_loop();
    u8 parse_if_else();
    u8 parse_mutate_var();
    u8 parse_defer_stmt();
    u8 parse_switch_stmt();
    u8 parse_ignore_vars();

    // Types
    Type parse_type();

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

    // err msgs
    u8 parse_error_expect_token(TknType);
    PrsErr convert_tkn_type_to_parse_error(TknType);
    u8 parser_error(PrsErr);
    const char *parser_error_msg(PrsErr);
    const char *parser_error_advice(PrsErr);
    u8 parse_error_use_global_err();

  public:
    Ast ast;
    // Parser exports
    Parser(file_t *, Lexer *);
    ~Parser();
    u8 parse_lexer();
}; // Parser

} // namespace rotate
