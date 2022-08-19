#include "include/parser.hpp"
#include "include/token.hpp"

#include "../include/defines.hpp"

namespace rotate
{

//
Parser::Parser(Lexer &lexer) : tokens(lexer.getTokens())
{
    ASSERT_NULL(&this->tokens, "lexer tokens passed is null");
}

// TODO:
Parser::~Parser() = default;

u8 Parser::parse()
{
    exit = 0, index = 0;
    for (;;)
    {
        token_type _type = current().type;
        bool is_public   = false;
        if (_type == token_type::Public)
        {
            advance();
            is_public = true;
        }
        switch (_type)
        {
            case token_type::Import:
                exit = parse_gl_imports();
                break;
            case token_type::Function:
                exit = parse_gl_function(is_public);
                break;
            case token_type::Const:
                exit = parse_gl_var_const(is_public);
                break;
            case token_type::Struct:
                exit = parse_gl_struct(is_public);
                break;
            case token_type::Enum:
                exit = parse_gl_enum(is_public);
                break;
            case token_type::EOT:
                return exit;
            default:
                return parser_report_error(_type);
        }
        if (exit == EXIT_FAILURE) parser_report_error(_type);
        advance();
    }
    return exit;
}

inline bool Parser::expect(token_type _type)
{
    if (_type == current().type)
    {
        advance();
        return true;
    }

    //! be careful of returning (cuz the return value is a u8)
    //! which defaults to EXIT_SUCCESS OR EXIT_FAILURE;
    //! bools are opposite
    //! ---V--- notice the negate symbol;
    return !parser_report_error(_type);
}

inline bool Parser::consume(token_type _type)
{
    // same as expect but returns false without throwing error
    if (_type == current().type)
    {
        advance();
        return true;
    }
    return false;
}

inline Token Parser::current()
{
    return tokens->at(index);
}

inline void Parser::advance()
{
    index++;
}

inline Token Parser::past()
{
    return tokens->at(index - 1);
}

Token Parser::peek()
{
    return tokens->at(index + 1);
}

u8 Parser::parse_gl_imports()
{
    advance();
    TODO("import parser implementation");
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_function(bool is_public)
{
    advance();
    TODO("function parser implementation");
    UNUSED(is_public);
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_var_const(bool is_public)
{
    advance();
    auto *x = new GLConst(current().index, is_public, RType(rt_type::undecided, true), nullptr);
    expect(token_type::Identifier);
    expect(token_type::Equal);
    ASSERT_NULL(x, "Error allocating for GLConstant");
    x->value = parse_node();
    expect(token_type::SemiColon);

    if (x->value != nullptr)
    {
        GLConstants.push_back(x);
        return EXIT_SUCCESS;
    }
    delete x;
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_struct(bool is_public)
{
    advance();
    TODO("structures parser implementation");
    UNUSED(is_public);
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_enum(bool is_public)
{
    advance();
    TODO("enum parser implementation");
    UNUSED(is_public);
    return EXIT_FAILURE;
}

u8 Parser::parser_report_error(token_type _type)
{
    fprintf(stderr, "Error at %s at index: %u\n", tkn_type_describe(_type), current().index);
    return exit = EXIT_FAILURE;
}

bool Parser::is_token_terminator(token_type _type)
{
    switch (_type)
    {
        case token_type::SemiColon:
        case token_type::CloseCurly:
        case token_type::CloseParen:
        case token_type::Comma:
        case token_type::Equal:
            return true;
        default:
            return false;
    }

    return false;
}

ASTNode *Parser::parse_node_helper(ASTNode *lhs, s8 minprec)
{
    advance();
    Token p = peek();
    while (is_token_binary_op(p.type) && precedence(p.type) >= minprec)
    {
        auto op = p;
        advance();
        ASTNode *rhs = parse_literal();

        p = peek();
        while (is_token_binary_op(p.type) && precedence(p.type) >= precedence(op.type))
        {
            rhs = parse_node_helper(rhs, precedence(op.type) +
                                             (precedence(p.type) > precedence(op.type)));

            p = peek();
        }
        // Binary Operator index
        lhs = new BinaryOpNode(op.index, op.type, rhs, lhs);
    }
    return lhs;
}

LiteralNode *Parser::parse_literal()
{
    return is_primary(current().type)
               ? new LiteralNode(current().index, convert_tkn_type_to_literal_type(current().type))
               : nullptr;
}

ASTNode *Parser::parse_node()
{
    //! Make sure to advance before entering this method
    // its critical due to recursive nature of this method
    /*
        * NOTE(Airbus5717): Expression Notes

        *Terminator = {
            `}`, `)`, `;`, `=` or `,` etc.
        }

        Literal = {
            String, Char(also EscapedChar), Integer, Float, Nil, True, False, Identifier,
            Builtin_function
        }

        TODO: Array_literal = {
            [ literals seperated with commas ]
        }

        Unary = {
            `!` or `-` then a ASTNode
            TODO: references
        }

        Operators = [
            `+`, `-`, `!=`, `==`,
            `>`, `<`, `>=`, `<=`,
            `*`, `/`, `and`, `or`,
        ]

        First check for unary, literal or grouping

        1 - Unary
        2 - Grouping
        3 - BinaryOp
    */

    return parse_node_helper(parse_literal(), 0);
}

bool Parser::is_primary(token_type type)
{
    switch (type)
    {
        case token_type::Integer:
        case token_type::Nil:
        case token_type::Float:
        case token_type::Char:
        case token_type::True:
        case token_type::False:
            return true;
        default:
            return false;
    }
    return false;
}

bool Parser::is_token_binary_op(token_type type)
{
    // `+`, `-`, `*`, `/`, `>`, `>=`, `!=`
    // `or`, `and`, `==`, `<`, `<=`
    switch (type)
    {
        case token_type::PLUS:
        case token_type::MINUS:
        case token_type::Star:
        case token_type::DIV:
        case token_type::Greater:
        case token_type::GreaterEql:
        case token_type::Less:
        case token_type::LessEql:
        case token_type::EqualEqual:
        case token_type::NotEqual:
        case token_type::And:
        case token_type::Or:
            return true;
        default:
            return false;
    }

    return false;
}

inline s8 Parser::precedence(token_type type)
{
    /*
    SOURCE(https://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy/)
      other       = -1;
      UNUSED(ASSIGNMENT) = 1;
      CONDITIONAL = 2;
      SUM         = 3;
      PRODUCT     = 4;
      UNUSED(EXPONENT) = 5;
      PREFIX      = 6;
      POSTFIX     = 7;
      CALL        = 8;
    */
    switch (type)
    {
        case token_type::Greater:
        case token_type::GreaterEql:
        case token_type::Less:
        case token_type::LessEql:
        case token_type::EqualEqual:
        case token_type::NotEqual:
            // `>`, `>=`, `<`, `<=`, `==`, `!=`,
            return 2;
        case token_type::PLUS:
        case token_type::MINUS:
            // `+`, `-`
            return 3;
        case token_type::Star:
        case token_type::DIV:
            // `*`, `/`
            return 4;
        case token_type::Or:
            // `or`
            return 5;
        case token_type::And:
            // `and`
            return 6;
        case token_type::Not:
            //* TAKE CARE OF PREFIX OPERATORS SUCH AS (-X) CUZ IT MIGHT BE WRONG
            return 7;
        case token_type::Identifier:
        case token_type::BuiltinFunc:
            // `id`, `@id`
            return 8;
        default:
            return -1;
    }

    return -1;
}

literal_type Parser::convert_tkn_type_to_literal_type(token_type tt)
{
    switch (tt)
    {
        case token_type::Char:
            return literal_type::chr;
        case token_type::Integer:
            return literal_type::integer;
        case token_type::Float:
            return literal_type::float_;
        case token_type::Nil:
            return literal_type::nil;
        case token_type::True:
        case token_type::False:
            return literal_type::boolean;

        default:
            break;
    }
    return literal_type::other;
}

RType Parser::parse_type(bool allow_mut)
{
    bool mut = consume(token_type::Mutable) && allow_mut;

    switch (current().type)
    {
        case token_type::INT_U8:
            return RType(rt_type::uint8, mut);
        case token_type::INT_U16:
            return RType(rt_type::uint16, mut);
        case token_type::INT_U32:
            return RType(rt_type::uint32, mut);
        case token_type::INT_U64:
            return RType(rt_type::uint64, mut);
        case token_type::INT_S8:
            return RType(rt_type::sint8, mut);
        case token_type::INT_S16:
            return RType(rt_type::sint16, mut);
        case token_type::INT_S32:
            return RType(rt_type::sint32, mut);
        case token_type::INT_S64:
            return RType(rt_type::sint64, mut);
        case token_type::FLOAT_f32:
            return RType(rt_type::float32, mut);
        case token_type::FLOAT_f64:
            return RType(rt_type::float64, mut);
        case token_type::IntKeyword:
            return RType(rt_type::sint32, mut);
        case token_type::FloatKeyword:
            return RType(rt_type::float32, mut);
        case token_type::Identifier: // will be decided during typechecking
            return RType(rt_type::undecided, mut);
        default:
            TODO("TODO parse this type");
            break;
    }
    return RType(rt_type::no_type, mut);
}

void Parser::save_log(FILE *file)
{
    fprintf(file, "--- GLOBAL IMPORTS ---\n");
    fprintf(file, "\n--- GLOBAL CONSTANTS ---\n");
    for (const auto &c : GLConstants)
        fprintf(file, "%s\n", c->to_string().c_str());
    fprintf(file, "\n--- FUNCTIONS ---\n");
    for (const auto &f : GLFunctions)
        fprintf(file, "TODO: function foo\n");

    fprintf(file, "\n--- STRUCTURES ---\n");
    for (const auto &s : GLStructures)
        fprintf(file, "TODO: s\n");

    fprintf(file, "\n--- ENUMERATIONS ---\n");
    for (const auto &e : GLEnums)
        fprintf(file, "TODO: enum\n");

    fprintf(file, "\n----- END OF PARSER -----\n");
}

} // namespace rotate
