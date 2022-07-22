#ifndef ROTATE_PARSER
#define ROTATE_PARSER

#include "lexer.hpp"

namespace rotate
{

// clang-format off
enum type_kind : u8 
{
    invalid, 
    undecided, // To be known 
    opaque, // void
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

class ASTNode
{
    const char *class_name = NULL;

    virtual const char *get_class_name() const
    {
        return class_name;
    }

  public:
    ASTNode()
    {
    }

    virtual ~ASTNode() = default;
};

class ScopeNode : public ASTNode
{
    std::vector<const ASTNode *> *childs;

    virtual const char *get_class_name() const override
    {
        return "ScopeNode";
    }

  public:
    ScopeNode()
    {
        childs = new std::vector<const ASTNode *>();
    }

    virtual ~ScopeNode() override
    {
        delete childs;
    }

    std::vector<const ASTNode *> *get_children()
    {
        return childs;
    }

    void append_child(ASTNode *node)
    {
        childs->push_back(node);
    }
};

class Program : public ScopeNode
{
    ScopeNode body;
    virtual const char *get_class_name() const override
    {
        return "Program";
    }

  public:
    Program(ScopeNode body) : body(std::move(body))
    {
    }

    virtual ~Program() = default;

    const ScopeNode &get_body() const
    {
        return body;
    }
};

class BlockStmt : public ScopeNode
{
    virtual const char *get_class_name() const override
    {
        return "BlockStmt";
    }

  public:
    BlockStmt()
    {
    }

    virtual ~BlockStmt() = default;
};

class ReturnStmt : public ASTNode
{
    const ASTNode *value;

    virtual const char *get_class_name() const override
    {
        return "ReturnStmt";
    }

  public:
    ReturnStmt(ASTNode *value) : value(value)
    {
    }

    virtual ~ReturnStmt() = default;
};

enum binary_expr_type : u8
{
    bp_add,     // '+'
    bp_sub,     // '-'
    bp_mul,     // '*'
    bp_div,     // '/'
    bp_eqleql,  // '=='
    bp_gr_eql,  // '>='
    bp_ls_eql,  // '<='
    bp_gr,      // '>'
    bp_ls,      // '<'
    bp_not_eql, // '!='
};

class Expression : public ASTNode
{

  public:
    Expression()
    {
    }

    virtual ~Expression() = default;

    virtual const char *get_class_name() const override
    {
        return "Expression";
    }
};

class BinaryExpr : public Expression
{
    ASTNode *left;
    ASTNode *right;
    binary_expr_type operation;

    virtual const char *get_class_name() const override
    {
        return "BinaryExpr";
    }

  public:
    BinaryExpr(ASTNode *left, ASTNode *right, binary_expr_type op)
        : left(left), right(right), operation(op)
    {
    }

    virtual ~BinaryExpr() = default;

    binary_expr_type get_operation() const
    {
        return operation;
    }
};

class UnaryExpr : public Expression
{

    virtual const char *get_class_name() const override
    {
        return "UnaryExpr";
    }

  public:
    UnaryExpr()
    {
    }

    virtual ~UnaryExpr() = default;
};

class Literal : public ASTNode
{
    virtual const char *get_class_name() const override
    {
        return "Literal";
    }

  public:
    Literal()
    {
    }

    virtual ~Literal() = default;
};

class CallExpr : Expression
{
    virtual const char *get_class_name() const override
    {
        return "CallExpr";
    }

  public:
    CallExpr()
    {
    }

    virtual ~CallExpr() = default;
};

class CallStmt : public ASTNode
{

  public:
    CallStmt()
    {
    }

    virtual ~CallStmt() = default;
};

class FunctionDecl : public ASTNode
{
    const ScopeNode body;
    const u32 index_of_name;
    const type _type; // return type
    // TODO: params type
    std::vector<type> params;

    virtual const char *get_class_name() const override
    {
        return "FunctionDecl";
    }

  public:
    FunctionDecl(const ScopeNode &body, const u32 &index, const type &_type)
        : body(body), index_of_name(index), _type(_type)
    {
    }

    virtual ~FunctionDecl() = default;

    u32 get_index_of_name() const
    {
        return index_of_name;
    }

    const type get_type() const
    {
        return _type;
    }

    const ScopeNode &get_body() const
    {
        return body;
    }
};

class GLVarDecl : public ASTNode
{
    const ASTNode *value;
    u32 index_of_name;
    type _type;

  public:
    GLVarDecl(ASTNode *value, u32 index, type _type)
        : value(value), index_of_name(index), _type(_type)
    {
    }

    virtual ~GLVarDecl() = default;

    const ASTNode *get_value() const
    {
        return value;
    }

    u32 get_index_of_name() const
    {
        return index_of_name;
    }

    const type get_type() const
    {
        return _type;
    }
};

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
