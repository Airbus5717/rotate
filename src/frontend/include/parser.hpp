#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"
#include "token.hpp"

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
    other,
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

    virtual const std::string to_string() const
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

    const std::string to_string() const
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

    const std::string to_string() const
    {
        TODO("RTStructType");
        return std::string{"TODO"};
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

// clang-format off
// ART_SOURCE(https://manytools.org/hacker-tools/ascii-banner/)
/*
 ┌─┐┌─┐┌┬┐  ┌┐┌┌─┐┌┬┐┌─┐┌─┐
 ├─┤└─┐ │   ││││ │ ││├┤ └─┐
 ┴ ┴└─┘ ┴   ┘└┘└─┘─┴┘└─┘└─┘                                                                                                       
*/
// clang-format on

struct ASTNode
{
    const u32 index; // token index

    ASTNode(u32 index) : index(index)
    {
    }

    virtual ~ASTNode() = default;

    virtual const std::string to_string() const
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

    const std::string to_string() const
    {
        return std::string{"LiteralNode(Type: " + literal_type_to_string(type) +
                           ", index: " + std::to_string(index) + ")"};
    }
};

struct ArrayIndexLiteral : public ASTNode
{

    // index is name
    ASTNode *arr_index;

    ArrayIndexLiteral(const u32 name_index, ASTNode *arr_index)
        : ASTNode(name_index), arr_index(arr_index)
    {
    }

    ~ArrayIndexLiteral() = default;

    const std::string to_string() const
    {
        return std::string{"ArrayIndexLiteral(name_index: " + std::to_string(index) +
                           ", array_index: " + arr_index->to_string() + ")"};
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

    const std::string to_string() const
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

    const std::string to_string() const
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

    const std::string to_string() const
    {
        TODO("to_string ArrayLiteralNode implementation");
    }
};

struct BinaryOpNode : public ASTNode
{
    token_type type;
    ASTNode *rhs, *lhs;

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

    const std::string to_string() const
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

    const std::string to_string() const
    {
        return std::string{"GroupingNode(" + arg->to_string() + ")"};
    }
};

// functionCall must not be void
struct FuncCallNode : public ASTNode
{
};

// clang-format off
/*
┌─┐┌┬┐┌─┐┌┬┐┌─┐┌┬┐┌─┐┌┐┌┌┬┐┌─┐
└─┐ │ ├─┤ │ ├┤ │││├┤ │││ │ └─┐
└─┘ ┴ ┴ ┴ ┴ └─┘┴ ┴└─┘┘└┘ ┴ └─┘
*/
// clang-format on
// don't use it directly
struct Statement
{
    const u32 index;
    Statement(u32 index) : index(index)
    {
    }

    virtual ~Statement() = default;

    virtual const std::string to_string() const
    {
        return std::string{"Statement(index: " + std::to_string(index) + ")"};
    }
};

struct BlockStmt : public Statement
{
    std::vector<Statement *> stmts;
    BlockStmt(u32 index) : Statement(index)
    {
    }

    ~BlockStmt() = default;

    const std::string to_string() const
    {
        std::string s{"BlockStmt(index: " + std::to_string(index) + ", stmts: "};

        for (const auto *st : stmts)
            s += "\t\t" + st->to_string();
        s += ")";
        return s;
    }
};

struct VoidFuncCall : public Statement
{
    std::vector<ASTNode *> params;

    VoidFuncCall(u32 index) : Statement(index)
    {
    }
    ~VoidFuncCall() = default;

    const std::string to_string() const
    {
        return std::string{""};
    }
};

struct DefineVariable : public Statement
{
};
// clang-format off
/*
┌─┐┬  ┌─┐┌┐ ┌─┐┬    ┌─┐┌─┐┌┬┐  ┌┐┌┌─┐┌┬┐┌─┐┌─┐
│ ┬│  │ │├┴┐├─┤│    ├─┤└─┐ │   ││││ │ ││├┤ └─┐
└─┘┴─┘└─┘└─┘┴ ┴┴─┘  ┴ ┴└─┘ ┴   ┘└┘└─┘─┴┘└─┘└─┘                                                                                                       
*/
// clang-format on

/// AST NODES IN GLOBAL SCOPE
// don't use GLASTNode directly
struct GLASTNode
{
    const u32 id_index;
    const bool is_public;

    GLASTNode(u32 index, bool pub) : id_index(index), is_public(pub)
    {
    }

    virtual ~GLASTNode() = default;

    virtual const std::string to_string() const
    {
        return "ASTNode(index: " + std::to_string(id_index) +
               ", public: " + (is_public ? "true" : "false") + ")";
    }
};

struct GLFunction : public GLASTNode
{
    RType return_type;
    BlockStmt *block;
    GLFunction(u32 id_index, bool is_pub, RType r_type, BlockStmt *block)
        : GLASTNode(id_index, is_pub), return_type(r_type), block(block)
    {
    }
    ~GLFunction() = default;

    const std::string to_string() const
    {
        return std::string{"GLFunction(id_index: " + std::to_string(id_index) + ", return type: " +
                           return_type.to_string() + ", public: " + (is_public ? "true" : "false") +
                           ", \n\tBody: " + block->to_string() + ")\n"};
    }
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

    const std::string to_string() const
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

    const std::string to_string() const
    {
        return std::string{"GLStructure(TODO)"};
    }
};

struct GLEnumeration : public GLASTNode
{
    GLEnumeration() : GLASTNode(0, false)
    {
        TODO("GLEnumeration constructor implementation");
    }
    // TODO: later
    ~GLEnumeration() = default;

    const std::string to_string() const
    {
        return std::string{"GLEnumeration(TODO)"};
    }
};

struct GLImportStmt
{
    // index is for the string;
    const u32 string_index;
    GLImportStmt(u32 index) : string_index(index)
    {
    }

    ~GLImportStmt() = default;

    const std::string to_string() const
    {
        return std::string{"GLImport(string index: " + std::to_string(string_index) + ")"};
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
    Parser(Lexer *);
    ~Parser(); // don't free lexer

    // parser starting point
    u8 parse();
    //
    void save_log(FILE *);

  private:
    std::vector<GLConst> GLConstants;
    std::vector<GLFunction> GLFunctions;
    std::vector<GLStructure> GLStructures;
    std::vector<GLEnumeration> GLEnums;
    std::vector<GLImportStmt> GLImports;

    // weak ptr to the lexer(parser doesnt own the lexer)
    Lexer *lexer;
    std::vector<Token> *tokens; // also this

    u32 index;
    u8 exit;

    //
    inline bool expect(token_type);
    inline bool consume(token_type);
    inline void advance();
    inline Token current();
    inline Token past();
    inline Token peek(); // compiler recommends against inlining
    inline Token next();

    // parser report errs
    u8 parser_report_error(Token);

    // parsing gl_stmts
    u8 parse_gl_imports();
    u8 parse_gl_function(bool);
    u8 parse_gl_var_const(bool);
    u8 parse_gl_struct(bool);
    u8 parse_gl_enum(bool);

    RType parse_type(bool);
    void parse_function_params();
    BlockStmt *parse_block_stmt();

    // ALL nodes are heap allocated
    ASTNode *parse_node();
    ASTNode *parse_next_node();

    ASTNode *parse_primary();
    void parse_literal(ASTNode *, Token);
    void parse_unary(ASTNode *, Token);
    void parse_grouping(ASTNode *);
    ASTNode *parse_binary(ASTNode *);
    void parse_function_or_name(ASTNode *);
    void parse_array_index(ASTNode *);
    void parse_array_literal(ASTNode *);
    void parse_function_call(ASTNode *);
    void parse_object_member(ASTNode *);

    literal_type convert_tkn_type_to_literal_type(token_type);
    unary_type convert_tkn_type_to_unary_type(token_type);
    bool is_token_binary_op(token_type);
    bool is_literal(token_type);
    bool is_unary(token_type);
    bool is_token_terminator(token_type);
    inline s8 precedence(token_type);
};

} // namespace rotate

#endif // ROTATE_PARSER
