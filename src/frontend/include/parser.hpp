#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"

namespace rotate
{

struct literal_expr;
struct unary_expr;
struct binary_expr;
struct expression;

using namespace rotate;


enum node_kind : u8
{
    nd_literal, // str, num, nil etc.
    nd_binary,  // `1 + 2` or `2 == 2`
    nd_unary,   // - or !
    nd_group,   // ()
};

enum binary_op : u8
{
    bp_plus,    // '+'
    bp_minus,   // '-'
    bp_star,    // '*'
    bp_slash,   // '/'
    bp_eql_eql, // '=='
    bp_gr_eql,  // '>='
    bp_ls_eql,  // '<='
    bp_gr,      // '>'
    bp_ls,      // '<'
    bp_not_eql, // '!='
};

enum lc_stmt_kind : u8
{
    lc_var_decl,
    lc_var_const_decl,
    lc_var_mut_decl,
    lc_while_loop,
    lc_for_loop,
    lc_if_else_stmt,
    lc_reassign_variable,
};

enum unary_kind : u8
{
    negate_logic, // '!'
    negate_sign,  // '-'
};

struct literal_expr
{
    u32 token_index;
};

struct binary_expr
{
    expression *left;
    expression *right;
    binary_op op;
};

struct unary_expr
{
    unary_kind kind;
    expression *expr;
};

struct expression
{
    node_kind kind;
    union elem {
        literal_expr *expr;
        binary_expr *expr;
        unary_expr *expr;
    };
};

struct stmt
{
    lc_stmt_kind kind;
};

struct block
{
    std::vector<stmt> stmts;
};

struct gl_import_t
{
    u32 identifier_index;
};

// variables in global scope are constant
struct gl_var_t
{
    u32 type;
};

struct gl_struct_t
{
    u32 type;
    std::vector<gl_var_t> inner_vars;
};

struct gl_enum_t
{
    // number is the index
    std::vector<token> ids;
};

struct gl_function_t
{
    // TODO type type;
    block blk;
};

class Parser
{
    // ptr to tokens from the lexer
    std::vector<token> *tokens;
    u32 index;
    std::vector<gl_function_t> gl_functions;
    std::vector<gl_struct_t> gl_structs;
    std::vector<gl_enum_t> gl_enums;
    std::vector<gl_import_t> gl_imports;
    std::vector<gl_var_t> gl_vars;

  public:
    Parser(Lexer *lexer);
    ~Parser(); // don't free lexer memory

    // parser starting point
    u8 parse();
    u32 consume();

    // parser report errs
    u8 parser_report_error();

    // parsing gl_stmts
    u8 parse_gl_imports();
    u8 parse_gl_function();
    u8 parse_gl_var_const();
    u8 parse_gl_struct();
    u8 parse_gl_enum();
};

} // namespace rotate

#endif // ROTATE_PARSER
