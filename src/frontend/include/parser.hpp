#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"
#include "token.hpp"
#include <memory>
#include <string>

namespace rotate
{

struct RType;

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

    // strings are char arrays (only ascii) (other unicodes won't be supported)
    stack_arr,
    heap_array, // length stored in runtime
};

enum class literal_type : u8
{
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

// kept as non class to use as int;
enum class binary_type : u8
{
    plus = 1,    // +
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

static const std::string rt_type_to_string(rt_type t)
{
    switch (t)
    {
        case rt_type::no_type:
            return "no_type";
        case rt_type::undecided:
            return "undecided";
        case rt_type::float32:
            return "f32";
        case rt_type::float64:
            return "f64";
        case rt_type::uint8:
            return "u8";
        case rt_type::uint16:
            return "u16";
        case rt_type::uint32:
            return "u32";
        case rt_type::uint64:
            return "u64";
        case rt_type::sint8:
            return "s8";
        case rt_type::sint16:
            return "s16";
        case rt_type::sint32:
            return "s32";
        case rt_type::sint64:
            return "s64";
        case rt_type::chr:
            return "char";
        case rt_type::boolean:
            return "bool";
        case rt_type::stack_arr:
            return "stack array";
        case rt_type::heap_array:
            return "heap array";
        default:
            break;
    }
    return "UNKNOWN";
}

struct RType
{
    rt_type _type;
    bool mut = false;
    RType(rt_type _type, bool mut) : _type(_type), mut(mut)
    {
    }

    ~RType() = default;

    virtual std::string to_string()
    {
        return "RType(" + rt_type_to_string(_type) + ", mut: " + (mut ? "true" : "false") + " )";
    }
};

struct RTArrayType : public RType
{
    usize size;
    RTArrayType(rt_type _type, bool mut, usize size) : RType(_type, mut), size(size)
    {
    }

    ~RTArrayType() = default;
    std::string to_string()
    {
        TODO("TO_STRING RTArrayType");
        return "";
    }
};

struct RTStructType : public RType
{
    // TODO later
    std::vector<RType> types;

    ~RTStructType() = default;
    std::string to_string()
    {
        TODO("TO_STRING RTStructType");
        return "";
    }
};

static const std::string literal_type_to_string(literal_type t)
{
    switch (t)
    {
        case literal_type::chr:
            return "char";
        case literal_type::integer:
            return "int";
        case literal_type::float_:
            return "float";
        case literal_type::boolean:
            return "bool";
        case literal_type::nil:
            return "nil";
        case literal_type::other:
            return "other";
        default:
            return "UNKNOWN";
    }
}

struct ASTNode
{
    const u32 index; // token index
    ASTNode(u32 index) : index(index)
    {
    }

    virtual ~ASTNode() = default;

    virtual std::string to_string()
    {
        return std::string{"ASTNode(index: " + std::to_string(index) + ")"};
    }
};

struct LiteralNode : public ASTNode
{
    literal_type type;
    LiteralNode(const u32 index, literal_type type) : ASTNode(index), type(type)
    {
    }

    ~LiteralNode() = default;

    std::string to_string()
    {
        return std::string{"LiteralNode(Type: " + literal_type_to_string(type) +
                           ", index: " + std::to_string(index) + ")"};
    }
};

struct ArrayIndexLiteral : public ASTNode
{
    ASTNode *arr_index;
    ArrayIndexLiteral(const u32 name_index, ASTNode *arr_index)
        : ASTNode(name_index), arr_index(arr_index)
    {
    }

    ~ArrayIndexLiteral() = default;

    std::string to_string()
    {
        return "TODO To_String()";
    }
};

struct StringLiteralNode : public ASTNode
{
    u32 size;
    StringLiteralNode(const u32 index, const u32 token_length)
        : ASTNode(index), size(token_length - 2)
    {
    }

    ~StringLiteralNode() = default;

    std::string to_string()
    {
        return std::string{"String(index: " + std::to_string(index) +
                           ", size: " + std::to_string(size) + ")"};
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

    std::string to_string()
    {
        return std::string{
            "UnaryNode(type: " + std::string((type == unary_type::negate_bool) ? "`!`" : "`-`") +
            ", arg: " + arg->to_string() + " )"};
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

    std::string to_string()
    {
        TODO("to_string ArrayLiteralNode implementation");
    }
};

struct BinaryOpNode : public ASTNode
{
    token_type type;
    ASTNode *rhs;
    ASTNode *lhs;
    // index is the binary operator
    BinaryOpNode(u32 index, token_type bin_type, ASTNode *rhs, ASTNode *lhs)
        : ASTNode(index), type(bin_type), rhs(rhs), lhs(lhs)
    {
    }

    ~BinaryOpNode()
    {
        delete rhs;
        delete lhs;
    }

    std::string to_string()
    {
        return std::string{"BinaryNode(Operator: " + std::string(tkn_type_describe(type)) +
                           ", binary index: " + std::to_string(index) +
                           ", RHS: " + rhs->to_string() + ", LHS: " + lhs->to_string() + ")"};
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

    std::string to_string()
    {
        return std::string{"GroupingNode(" + arg->to_string() + ")"};
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

    GLASTNode(u32 index, bool pub) : id_index(index), is_public(pub)
    {
    }

    ~GLASTNode() = default;

    virtual std::string to_string()
    {
        return "ASTNode(index: " + std::to_string(id_index) +
               ", public: " + (is_public ? "true" : "false") + ")";
    }
};

struct GLFunction : public GLASTNode
{
    RType type;
};

struct GLConst : public GLASTNode
{
    RType type; // CHANGE IN TYPECHECKING PHASE
    ASTNode *value;
    GLConst(u32 index, bool pub, RType type, ASTNode *value)
        : GLASTNode(index, pub), type(type), value(value)
    {
    }

    ~GLConst()
    {
        delete value;
    }

    std::string to_string()
    {
        return "GLConst(index: " + std::to_string(id_index) +
               ", public: " + (is_public ? "true" : "false") +
               ", type: " + rt_type_to_string(type._type) + ", value: " + value->to_string() + " )";
    }
};

struct GLStructure : public GLASTNode
{
    // TODO: later
    std::vector<RType> types; // ordered

    ~GLStructure() = default;
};

struct GLEnumeration : public GLASTNode
{
    // TODO: later
    ~GLEnumeration() = default;
};

struct GLImportStmt
{
    // index is for the string;
    const u32 string_index;
    GLImportStmt(u32 index) : string_index(index)
    {
    }

    ~GLImportStmt() = default;

    std::string to_string()
    {
        return std::string{"GLImport(index: " + std::to_string(string_index) + ")"};
    }
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
    void save_log(FILE *);

  private:
    std::vector<GLConst *> GLConstants;
    std::vector<GLFunction *> GLFunctions;
    std::vector<GLStructure *> GLStructures;
    std::vector<GLEnumeration *> GLEnums;
    std::vector<GLImportStmt *> GLImports;

    // weak ptr to tokens from the lexer(the owner)
    std::vector<Token> *tokens;
    u32 index;
    u8 exit;

    //
    inline bool expect(token_type);
    inline bool consume(token_type);
    inline void advance();
    inline Token current();
    inline Token past();
    Token peek(); // compiler recommends against inlining
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
    ASTNode *parse_node(); // nodes are allocated in the heap
    LiteralNode *parse_literal();
    ASTNode *parse_node_helper(ASTNode *, s8); // nodes are allocated in the heap
    bool is_token_terminator(token_type);

    literal_type convert_tkn_type_to_literal_type(token_type);
    bool is_token_binary_op(token_type);
    bool is_primary(token_type);
    inline s8 precedence(token_type);
};

} // namespace rotate

#endif // ROTATE_PARSER
