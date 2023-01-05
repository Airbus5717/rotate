#pragma once

#include "errmsgs.hpp"
#include "lexer.hpp"
#include "type.hpp"

namespace rotate
{

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

enum class BinaryOpType : u8
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
             // TODO: more operators
};

enum class UnaryOpType : u8
{
    logical_negate,   // '!'
    numerical_negate, // -
    new_opr,          // new (only allowed as expressions)
    delete_opr,       // delete (only allowed as stmts)
};

struct LitExpr
{
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

struct ArrayExpr
{
    Type type;
    usize size;
    std::vector<Expr> expressions;
};

struct FnCallExpr
{
    TknIdx id;
    Type return_type;
    std::vector<Expr> input_params;
    // NOTE(5717): must be non-void return type
};

enum class ExprType : u8
{
    Literal,
    ArraySub,
    UnaryExpr,
    ArrayLiteral,
    BinaryExpr,
    FuncCallNonVoid,
};

struct Expr
{
    ExprIdx idx;
    ExprType type;
};

// NOTE: array[i]
struct ArraySubExpr
{
    TknIdx id; // array id
    Expr i;
};

// NOTE(5717): used as func params, struct params or variables
struct VarDef
{
    TknIdx id;
    Type type;
};
/*
 * Abstract Syntax tree
 */
struct Ast
{
    // Globals
    std::vector<AstImport> imports;
    std::vector<AstGlVar> gl_variables;
    std::vector<AstStruct> structs;
    std::vector<AstEnum> enums;
    std::vector<AstFn> functions;
    // Expressions
    std::vector<Expr> expressions;
    std::vector<LitExpr> literals;
    std::vector<BinaryExpr> bin_exprs;
    std::vector<UnaryExpr> unary_exprs;
    std::vector<ArrayExpr> arr_exprs;
    std::vector<FnCallExpr> fn_exprs;
    // types
    SymbolTable stable;
};

struct AstImport
{

    // 0  1  2     34    56
    // import "std/io" as io;
    //         ^^^^^^val   ^^id
    // index is the index of string token
    // It requires a single string Token for path
    // the path must be known at compile time
    TknIdx alias_id;
    TknIdx import_str;
    bool aliased;

    AstImport(UINT import_str) : import_str(import_str), aliased(false)
    {
    }

    AstImport(TknIdx alias_id, TknIdx import_str)
        : alias_id(alias_id), import_str(import_str), aliased(true)
    {
    }

    ~AstImport() = default;
};

struct AstGlVar
{
    TknIdx id_idx;
    Type type;
    ExprIdx value;

    AstGlVar(TknIdx i, Type t, ExprIdx e) : id_idx(i), type(t), value(e)
    {
    }

    ~AstGlVar() = default;
};

struct AstStruct
{
    TknIdx id;
    std::vector<VarDef> members;
};

struct AstEnum
{
    TknIdx id;
    std::vector<TknIdx> members;
};

struct AstBlock
{
    // TODO: vector of statements
};

struct AstFn
{
    TknIdx id;
    Type return_type;
    AstBlock block;
    std::vector<VarDef> params;
};

class Parser
{
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
    ExprIdx parse_expr(TknType, bool *);
    BinaryExpr parse_bin_expr(TknType, bool *);
    UnaryExpr parse_unary_expr(TknType, bool *);
    Expr parse_grouping_expr(TknType, bool *);
    ArrayExpr parse_array_literal_expr(TknType, bool *);
    LitExpr parse_literal_expr(TknType, bool *);
    FnCallExpr parse_void_fn_call_expr(TknType, bool *);

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

    // add expressions
    ExprIdx add_literal_expr(LitExpr);
    ExprIdx add_binary_expr(BinaryExpr);
    ExprIdx add_unary_expr(UnaryExpr);
    ExprIdx add_array_literal_expr(ArrayExpr);
    ExprIdx add_array_sub_literal_expr(ArraySubExpr);
    ExprIdx add_nonvoid_fn_call_expr(FnCallExpr);
    // add globals
    // NOTE: not needed for now

    // PRIVATE members
    // not owned (weak ptr)
    file_t *file;
    std::vector<Token> *tokens;
    //
    PrsErr error;
    UINT idx; // of tokens
    UINT ExprIndex, LitIdx, BinIdx, UnaryIdx, ArrayExprIdx, ArraySubIdx, FnCallIdx;

  public:
    Ast *ast;
    // Parser exports
    Parser(file_t *, Lexer *);
    ~Parser();
    u8 parse_lexer();
}; // Parser

} // namespace rotate
