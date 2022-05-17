#include "include/parser.hpp"

namespace rotate
{

//
Parser::Parser(Lexer *lexer) : tokens(lexer->getTokens())
{
    ASSERT_NULL(this->tokens, "lexer passed is null");
}

Parser::~Parser() = default;

u8 Parser::parse()
{
    auto *tkns = tokens;
    u8 exit    = 0;
    for (;;)
    {
        switch (tkns->at(index).type)
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
            default:
                return parser_report_error();
        }
        if (exit == EXIT_FAILURE) return parser_report_error();
    }

    return EXIT_SUCCESS;
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
    TODO("enums parser implementation");
    return EXIT_FAILURE;
}

u8 Parser::parser_report_error()
{
    TODO("Parser error reporting implementation");
    return EXIT_FAILURE;
}

} // namespace rotate
