#include "include/parser.hpp"
#include "include/token.hpp"

namespace rotate
{

//
Parser::Parser(Lexer &lexer) : tokens(lexer.getTokens())
{
    ASSERT_NULL(&this->tokens, "lexer tokens passed is null");
}

// TODO:
Parser::~Parser()
{
    this->GLConstants.clear();
    this->GLFunctions.clear();
    this->GLEnums.clear();
    this->GLImports.clear();
    this->GLStructures.clear();
}

u8 Parser::parse()
{
    TODO("parser");
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

    return exit == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

bool Parser::expect(token_type _type)
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

bool Parser::consume(token_type _type)
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

inline Token Parser::peek()
{
    return tokens->at(index + 1);
}

u8 Parser::parse_gl_imports()
{
    TODO("import parser implementation");
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_function(bool is_public)
{
    TODO("function parser implementation");
    UNUSED(is_public);
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_var_const(bool is_public)
{
    advance();
    TODO("global variable parser implementation");
    UNUSED(is_public);
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_struct(bool is_public)
{
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
    TODO("Parser error reporting implementation");
    UNUSED(_type);
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

ASTNode *Parser::parse_node_helper()
{
    return nullptr;
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
    */

    return nullptr;
}

bool Parser::is_token_binary_op(token_type type)
{
    // `+`, `-`, `*`, `/`, `>`, `>=`
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
        case token_type::And:
        case token_type::Or:
            return true;
        default:
            break;
    }

    return false;
}

literal_type Parser::convert_tkn_type_to_literal_type(token_type tt)
{
    switch (tt)
    {
        case token_type::String:
            return literal_type::string;
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

} // namespace rotate
