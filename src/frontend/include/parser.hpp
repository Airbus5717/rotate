#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"
#include "token.hpp"

namespace rotate
{

enum class type_kind : u8
{
    invalid,
    undecided, // To be known
    opaque,    // void
    f32,
    f64,
    uint8,
    uint16,
    uint32,
    uint64,
    sint8,
    sint16,
    sint32,
    sint64,
    chr,
    boolean,

    enumeration,

    structure,
    // strings are char arrays (only ascii) (other unicodes won't be supported)
    stack_arr,
    heap_array, // length stored in runtime
};

struct rotate_type
{
    type_kind rtype;
    u32 size;

    rotate_type(type_kind i, u32 _size) : rtype(i)
    {
        if (i == type_kind::stack_arr)
            _size = size;
    }

    ~rotate_type() = default;
};

enum class rnode_type : u8
{
    literal = 0,
    literal_array,
    binary,
    unary,
};

enum class unary_type : u8
{
    negate_minus = 0,
    negate_bool,
};

enum class binary_type : u8
{
    plus = 0,    // +
    minus,       // -
    star,        // *
    divide,      // /
    greater,     // >
    greater_eql, // >=
    less,        // <
    less_eql,    // <=
    eql_eql,     // ==
    _and,        // and
    _or,         // or
};

struct node;
struct ast_node;

struct literal
{
    token *tkn;
};

struct literal_array
{
    std::vector<node *> children;
    u32 size;

    ~literal_array() = default;
};

struct grouping
{
    node *child;
};

struct unary_op
{
    unary_type _op;
};

struct bin_op
{
    binary_type _op;
};

struct node
{
    rnode_type _type;
    union {
        struct literal ltr;
        struct literal_array ltr_arr;
        struct unary_op unary;
        struct bin_op bin;
    } & child;
};

/// global stuff

enum class ast_gl_node : u8
{
    gl_function = 0,
    gl_variable,
    gl_import,
    gl_struct,
    gl_enum,
};

struct gl_fun
{
    rotate_type return_type;
    token &name;
    std::vector<node *> stmts;

    ~gl_fun()
    {
        stmts.clear();
    }
};

struct gl_variable
{
    rotate_type _type;
    node *value;
};

struct gl_import
{
    token &path;
};

struct gl_structure
{
    // TODO NOTE(5717): this is kinda more complex
};

struct gl_enum
{
    // TODO NOTE(5717): this is kinda more complex
};

struct gl_node
{
    ast_gl_node node_type;
    union {
        struct gl_fun fn;
        struct gl_import imprt;
        struct gl_variable var;
        struct gl_structure structure;
        struct gl_enum enumeration;
    } node;
};

/// Parser
class Parser
{
    // ptr to tokens from the lexer
    std::vector<token> *tokens;
    u32 index;

  public:
    Parser(Lexer &lexer);
    ~Parser(); // don't free lexer

    //
    void save_log();

    //
    bool expect(token_type _type);
    u32 consume();
    inline void advance();
    inline token current();
    inline token past();
    inline token peek();
    inline token next();

    // parser starting point
    u8 parse();

    // parser report errs
    u8 parser_report_error(token_type);

    // parsing gl_stmts
    u8 parse_gl_imports();
    u8 parse_gl_function();
    u8 parse_gl_var_const();
    u8 parse_gl_struct();
    u8 parse_gl_enum();

    rotate_type parse_type();
};

} // namespace rotate

#endif // ROTATE_PARSER
