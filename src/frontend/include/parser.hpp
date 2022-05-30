#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"

namespace rotate
{

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
    std::vector<type> fields_types;

    struct_type(type first_type) : type(first_type)
    {
    }

    void add_field_type(type _type)
    {
        fields_types.push_back(_type);
    }
};

class Parser
{
    // ptr to tokens from the lexer
    std::vector<token> *tokens;
    u32 index;

  public:
    Parser(Lexer *lexer);
    ~Parser(); // don't free lexer 

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
