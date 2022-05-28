#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"

namespace rotate
{

struct literal_expr;
struct unary_expr;
struct binary_expr;
struct expression;
struct type;
struct token;

// clang-format off
enum type_kind : u8 
{
    invalid,
    f32, f64,
    uint8, uint16, uint32, uint64,
    sint8, sint16, sint32, sint64,
    chr, boolean,

    enumeration,

    structure,
    // strings are char arrays (only ascii) (other unicodes won't be supported)
    arr, 
    dynamic_array, // length stored in runtime
};
// clang-format on

struct type
{
    type_kind kind;

    type(type_kind kind) : kind(kind)
    {
    }
};

struct array_type : type
{
    u32 size; // must not be a dynamic array

    array_type(type child, u32 size) : type(child), size(size)
    {
    }
};

struct struct_type : type
{
    Vector<type> fields_types;

    struct_type(type first_type) : type(first_type)
    {
    }

    void add_field_type(type _type)
    {
        fields_types.push_back(_type);
    }
};

enum node_kind : u8
{
    nd_literal, // str, num, nil, id etc.
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

    //* !s or -2
    unary_kind kind;
    expression *expr;
};

struct expression
{
    type _type;
    node_kind kind;
    union value {
        binary_expr *binary;
        unary_expr *unary;
        literal_expr *literal;
    };
};

enum lc_stmt_kind : u8
{
    lc_var_decl,
    lc_var_const_decl,
    lc_var_mut_decl,
    lc_while_loop,
    lc_for_loop,
    lc_if_else_stmt,
    lc_mutable_var,
};

struct lc_reassign_var
{
    u32 id;
    expression *val;
};

struct lc_var_decl
{
};

struct lc_let_var_decl
{
};

struct lc_while_loop
{
};

struct lc_for_loop
{
};

struct lc_if_else_stmt
{
};

struct stmt
{
    lc_stmt_kind kind;
};

struct block
{
    Vector<stmt> stmts;
};

struct gl_import_t
{
    u32 identifier_index;
};

// variables in global scope are constant
struct gl_var_t
{
    type _type;
    expression val;
};

struct gl_struct_t
{
    Vector<gl_var_t> inner_vars;
};

struct gl_enum_t
{
    // number is the index
    token *id;
    Vector<token *> ids;
};

struct gl_function_t
{
    type _type;
    block blk;
};

class Parser
{
    // ptr to tokens from the lexer
    Vector<token> *tokens;
    u32 index;
    Vector<gl_function_t> gl_functions;
    Vector<gl_struct_t> gl_structs;
    Vector<gl_enum_t> gl_enums;
    Vector<gl_import_t> gl_imports;
    Vector<gl_var_t> gl_vars;

  public:
    Parser(Lexer *lexer);
    ~Parser(); // don't free lexer memory

    //
    void save_log();

    //
    u8 expect(token_type _type);
    inline void advance();
    inline token current();
    inline token past();
    inline token peek();
    inline token next();

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

    type parse_type();
};

} // namespace rotate

#endif // ROTATE_PARSER
