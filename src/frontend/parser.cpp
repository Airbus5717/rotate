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

    return exit == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

bool Parser::expect(token_type _type)
{
    if (_type == current().type)
    {
        advance();
        return true;
    }
    return !parser_report_error(_type);
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
    TODO("Parser error reporting implementation");
    return exit = EXIT_FAILURE;
}

RType Parser::parse_type()
{
    TODO("Parse type");
    return RType{._type = rt_type::no_type};
}

} // namespace rotate
