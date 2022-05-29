#include "include/parser.hpp"
#include "include/token.hpp"

namespace rotate
{

//
Parser::Parser(Lexer *lexer)
{
    this->tokens = lexer->getTokens();
    ASSERT_NULL(this->tokens, "lexer passed is null");
}

// TODO: implementation
Parser::~Parser() = default;

u8 Parser::parse()
{
    u8 exit   = 0;
    auto tkns = tokens;
    for (;;)
    {
        switch (tkns->at(index).type)
        {
            case Import:
                exit = parse_gl_imports();
                break;
            case Function:
                exit = parse_gl_function();
                break;
            case Const:
                exit = parse_gl_var_const();
                break;
            case Struct:
                exit = parse_gl_struct();
                break;
            case Enum:
                exit = parse_gl_enum();
                break;
            case EOT:
                return exit;
                break;
            default:
                return parser_report_error();
        }
        advance();
        if (exit == EXIT_FAILURE) return parser_report_error();
    }

    return EXIT_SUCCESS;
}

u8 Parser::expect(token_type _type)
{
    if (_type == tokens->at(index).type)
    {
        index++;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
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

type Parser::parse_type()
{

    //* FULL TYPE CHECKING WILL DO AFTER PARSING
    /*
    invalid,
    f32, f64,
    uint8, uint16, uint32, uint64,
    sint8, sint16, sint32, sint64,
    chr, boolean,

    enumeration,
    arr, struct,
    type_identifier,
    */
    switch (current().type)
    {
        case FLOAT_f32:
            return type(f32);
        case FLOAT_f64:
            return type(f64);
        case INT_U8:
            return type(uint8);
        case INT_U16:
            return type(uint16);
        case INT_U32:
            return type(uint32);
        case INT_U64:
            return type(uint64);
        case INT_S8:
            return type(sint8);
        case INT_S16:
            return type(sint16);
        case INT_S32:
            return type(sint32);
        case INT_S64:
            return type(sint64);
        case CharKeyword:
            return type(chr);
        case BoolKeyword:
            return type(boolean);
        case OpenSQRBrackets:
            TODO("arr type parse");
            return array_type(dynamic_array, 3);
        case Identifier:
            return type(structure);
        default: {
            /*
                Arrays, structures and enums
            */
            TODO("parse type");
        }
    }

    return type(invalid);
}

} // namespace rotate
