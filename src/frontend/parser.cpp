#include "include/parser.hpp"
#include "include/token.hpp"

namespace rotate
{

//
Parser::Parser(Lexer &lexer) : tokens(lexer.getTokens()), index(0)
{
    ASSERT_NULL(&this->tokens, "lexer passed is null");
}

// TODO: implementation
Parser::~Parser() = default;

u8 Parser::parse()
{
    TODO("parser");
    u8 exit = 0;
    for (;;)
    {
        token_type _type = current().type;
        switch (_type)
        {
            case token_type::Import:
                parse_gl_imports();
                break;
            case token_type::Function:
                parse_gl_function();
                break;
            case token_type::Const:
                parse_gl_var_const();
                break;
            case token_type::Struct:
                parse_gl_struct();
                break;
            case token_type::Enum:
                parse_gl_enum();
                break;
            case token_type::EOT:
                return exit;
            default:
                return parser_report_error(_type);
        }
        advance();
    }

    return EXIT_SUCCESS;
}

bool Parser::expect(token_type _type)
{
    if (_type == current().type)
    {
        advance();
        return true;
    }
    return parser_report_error(_type);
}

inline token Parser::current()
{
    return this->tokens->at(index);
}

inline void Parser::advance()
{
    index++;
}

inline token Parser::past()
{
    return tokens->at(index - 1);
}

inline token Parser::peek()
{
    return tokens->at(index + 1);
}

u8 Parser::parse_gl_imports()
{
    TODO("import parser implementation");
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_function()
{
    TODO("function parser implementation");
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_var_const()
{
    TODO("global variable parser implementation");
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_struct()
{
    TODO("structures parser implementation");
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_enum()
{
    advance();
    TODO("enum parser implementation");
    return EXIT_FAILURE;
}

u8 Parser::parser_report_error(token_type _type)
{
    using std::exit;
    TODO("Parser error reporting implementation");
    exit(1);
    return EXIT_FAILURE;
}

rotate_type Parser::parse_type()
{
    //* FULL TYPE CHECKING WILL DO AFTER PARSING
    switch (current().type)
    {
        case token_type::FLOAT_f32:
            return rotate_type(type_kind::f32, 0);
        case token_type::FLOAT_f64:
            return rotate_type(type_kind::f64, 0);
        case token_type::INT_U8:
            return rotate_type(type_kind::uint8, 0);
        case token_type::INT_U16:
            return rotate_type(type_kind::uint16, 0);
        case token_type::INT_U32:
            return rotate_type(type_kind::uint32, 0);
        case token_type::INT_U64:
            return rotate_type(type_kind::uint64, 0);
        case token_type::INT_S8:
            return rotate_type(type_kind::sint8, 0);
        case token_type::INT_S16:
            return rotate_type(type_kind::sint16, 0);
        case token_type::INT_S32:
            return rotate_type(type_kind::sint32, 0);
        case token_type::INT_S64:
            return rotate_type(type_kind::sint64, 0);
        case token_type::CharKeyword:
            return rotate_type(type_kind::chr, 0);
        case token_type::BoolKeyword:
            return rotate_type(type_kind::boolean, 0);
        case token_type::OpenSQRBrackets:
            TODO("arr rotate_type parse");
            return rotate_type(type_kind::heap_array, 0);
        case token_type::Identifier:
            return rotate_type(type_kind::no_type, 0);
        default: {
            /*
                Arrays, structures and enums
            */
            TODO("parse type");
        }
    }

    return rotate_type(type_kind::no_type, 0);
}

} // namespace rotate
