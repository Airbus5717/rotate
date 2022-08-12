#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"
#include "token.hpp"

namespace rotate
{

enum class rt_type : u8
{
    no_type,
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

struct RType
{
    rt_type _type;
};

struct RTArrayType : public RType
{
    usize size;
};

struct RTStructType : public RType
{
    // TODO later
    std::vector<RType> types;
};

struct ASTNode
{
    RType type;
};

struct Literal : public ASTNode
{
};
struct Unary : public ASTNode
{
};
struct ArrayLiteral : public ASTNode
{
};
struct BinaryOp : public ASTNode
{
};
struct Grouping : public ASTNode
{
};
struct FuncCall : public ASTNode
{
};

// don't use it directly
struct GLASTNode
{
    const u32 id_index;
};

struct GLFunction : public GLASTNode
{
    RType type;
};

struct GLConst : public GLASTNode
{
    RType type;
    std::unique_ptr<ASTNode> value;
};

struct GLStructure : public GLASTNode
{
    // TODO: later
    std::vector<RType> types; // ordered
};

struct GLEnumeration : public GLASTNode
{
    // TODO: later
};

struct GLImportStmt : public GLASTNode
{
    // TODO: later
};

/// Parser
class Parser
{
    // ptr to tokens from the lexer
    std::vector<token> *tokens;
    u32 index;
    u8 exit;
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

    RType parse_type();
    ASTNode *parse_node(); // nodes are in the heap
};

} // namespace rotate

#endif // ROTATE_PARSER
