#include "include/parser.hpp"

namespace rotate
{

//
Parser::Parser(Lexer *lexer) : tokens(lexer->getTokens())
{
    ASSERT_NULL(this->tokens, "lexer passed is null");
}

// TODO: implementation
Parser::~Parser() = default;

u8 Parser::parse()
{
    u8 exit = 0;

    for (;;)
    {
        switch (tokens->at(index).type)
        {
            case TknTypeImport:
                exit = parse_gl_imports();
                break;
            case TknTypeFunction:
                exit = parse_gl_function();
                break;
            case TknTypeConst:
                exit = parse_gl_var_const();
                break;
            case TknTypeStruct:
                exit = parse_gl_struct();
                break;
            case TknTypeEnum:
                exit = parse_gl_enum();
                break;
            case TknTypeEOT:
                return exit;
                break;
            default:
                return parser_report_error();
        }
        if (exit == EXIT_FAILURE) return parser_report_error();
    }

    return EXIT_SUCCESS;
}

u8 Parser::consume(token_type _type)
{
    if (_type == tokens->at(index).type)
    {
        index++;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void Parser::advance()
{
    index++;
}

token Parser::past()
{
    return tokens->at(index - 1);
}

token Parser::peek()
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
    TODO("global variables parser implementation");
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

u8 Parser::parser_report_error()
{
    TODO("Parser error reporting implementation");
    return EXIT_FAILURE;
}

} // namespace rotate
