#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"
#include "token.hpp"

namespace rotate
{

enum class rt_type : u8
{
    no_type = 0,
    undecided,
    float32,
    float64,

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
    bool mut = false;
    RType(rt_type _type, bool mut) : _type(_type), mut(mut)
    {
    }
};

struct RTArrayType : public RType
{
    usize size;
    RTArrayType(rt_type _type, bool mut, usize size) : RType(_type, mut), size(size)
    {
    }
};

struct RTStructType : public RType
{
    // TODO later
    std::vector<RType> types;
};

struct ASTNode
{
    RType type;
    ASTNode(RType type): type(type) {}
};

struct Literal : public ASTNode
{
    Token token;
    Literal(RType type, Token token): ASTNode(type), token(token) {}
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
    const u32 id_index; const bool is_public;
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
    std::vector<Token> *tokens;
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
    inline Token current();
    inline Token past();
    inline Token peek();
    inline Token next();

    // parser starting point
    u8 parse();

    // parser report errs
    u8 parser_report_error(token_type);

    // parsing gl_stmts
    u8 parse_gl_imports();
    u8 parse_gl_function(bool is_public);
    u8 parse_gl_var_const(bool is_public);
    u8 parse_gl_struct(bool is_public);
    u8 parse_gl_enum(bool is_public);

    RType parse_type();
    ASTNode *parse_node(); // nodes are allocated in the heap
};

} // namespace rotate

#endif // ROTATE_PARSER
