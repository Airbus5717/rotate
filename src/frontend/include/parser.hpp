#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"
#include "token.hpp"
#include <memory>
#include <string>

namespace rotate
{

enum class rt_type : u8
{
    no_type = 0,
    nil,
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

enum class literal_type : u8
{
    string,
    chr,
    integer,
    float_,
    boolean,
    nil,
    other,
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
    reference_ptr, // node must be an identifier to a variable
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
    rt_and,      // and
    rt_or,       // or
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
    const u32 index; // token index
    ASTNode(u32 index) : index(index)
    {
    }
};

struct LiteralNode : public ASTNode
{
    literal_type type;
    LiteralNode(const u32 index, literal_type type) : ASTNode(index), type(type)
    {
    }
};

struct UnaryNode : public ASTNode
{
    unary_type type;
    ASTNode *arg;
    UnaryNode(const u32 index, unary_type untype, ASTNode *arg)
        : ASTNode(index), type(untype), arg(arg)
    {
    }

    ~UnaryNode()
    {
        delete arg;
    }
};

struct ArrayLiteralNode : public ASTNode
{
    std::vector<ASTNode *> elements;
    usize size;
    ArrayLiteralNode(u32 index, usize size) : ASTNode(index), size(size)
    {
    }

    ~ArrayLiteralNode()
    {
        for (const auto &s : elements)
            delete s;
    }
};

struct BinaryOpNode : public ASTNode
{
    binary_type type;
    ASTNode *rhs;
    ASTNode *lhs;
    BinaryOpNode(u32 index, binary_type bin_type, ASTNode *rhs, ASTNode *lhs)
        : ASTNode(index), type(bin_type), rhs(rhs), lhs(lhs)
    {
    }

    ~BinaryOpNode()
    {
        delete rhs;
        delete lhs;
    }
};

struct GroupingNode : public ASTNode
{
    ASTNode *arg;
    GroupingNode(u32 index, ASTNode *arg) : ASTNode(index), arg(arg)
    {
    }

    ~GroupingNode()
    {
        delete arg;
    }
};

struct FuncCallNode : public ASTNode
{
};

// don't use it directly
struct GLASTNode
{
    const u32 id_index;
    const bool is_public;
};

struct GLFunction : public GLASTNode
{
    RType type;
};

struct GLConst : public GLASTNode
{
    RType type;
    ASTNode *value;

    ~GLConst()
    {
        delete value;
    }
};

struct GLStructure : public GLASTNode
{
    // TODO: later
    std::vector<RType> types; // ordered

    ~GLStructure()
    {
        types.clear();
    }
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
/*
    allocate nodes into a pool
    which will be deleted all at once
*/
class Parser
{
  public:
    Parser(Lexer &);
    ~Parser(); // don't free lexer

    // parser starting point
    u8 parse();
    //
    void save_log();

  private:
    std::vector<GLConst> GLConstants;
    std::vector<GLFunction> GLFunctions;
    std::vector<GLStructure> GLStructures;
    std::vector<GLEnumeration> GLEnums;
    std::vector<GLImportStmt> GLImports;

    // weak ptr to tokens from the lexer(the owner)
    std::vector<Token> *tokens;
    u32 index;
    u8 exit;

    //
    bool expect(token_type);
    bool consume(token_type);
    inline void advance();
    inline Token current();
    inline Token past();
    inline Token peek();
    inline Token next();

    // parser report errs
    u8 parser_report_error(token_type);

    // parsing gl_stmts
    u8 parse_gl_imports();
    u8 parse_gl_function(bool);
    u8 parse_gl_var_const(bool);
    u8 parse_gl_struct(bool);
    u8 parse_gl_enum(bool);

    RType parse_type(bool);
    ASTNode *parse_node();        // nodes are allocated in the heap
    ASTNode *parse_node_helper(); // nodes are allocated in the heap
    bool is_token_terminator(token_type);

    literal_type convert_tkn_type_to_literal_type(token_type);
    bool is_token_binary_op(token_type);
};

} // namespace rotate

#endif // ROTATE_PARSER
