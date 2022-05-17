#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"

namespace rotate
{

enum node_kind : u8
{
    nd_literal, // str, num, nil etc.
    nd_binary,  // `1 + 2` or `2 == 2`
    nd_unary,   // - or !
    nd_group,   // ()
};

struct node
{
    node_kind kind;
    union el {
    };
};

enum gl_stmt_kind : u8
{
    gl_import,
    gl_var_const,
    gl_function,
    gl_struct,
    gl_enum,
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

struct gl_stmt
{
    gl_stmt_kind kind;
    union stmt {
        gl_function_t function;
        gl_struct_t structure;
        gl_var_t var_const;
        gl_enum_t enumeration;
        gl_import_t import_s;
    };
};

class Parser
{
    // ptr to original lexer
    Lexer *lexer;
    std::vector<gl_stmt> gl_stmts;

  public:
    Parser(Lexer *lexer);
    ~Parser() = default;

    // parser starting point
    u8 parse_lexer();

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
