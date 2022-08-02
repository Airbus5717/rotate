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
            case Import:
                parse_gl_imports();
                break;
            case Function:
                parse_gl_function();
                break;
            case Const:
                parse_gl_var_const();
                break;
            case Struct:
                parse_gl_struct();
                break;
            case Enum:
                parse_gl_enum();
                break;
            case EOT:
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
        case Void:
            return rotate_type(opaque, 0);
        case FLOAT_f32:
            return rotate_type(f32, 0);
        case FLOAT_f64:
            return rotate_type(f64, 0);
        case INT_U8:
            return rotate_type(uint8, 0);
        case INT_U16:
            return rotate_type(uint16, 0);
        case INT_U32:
            return rotate_type(uint32, 0);
        case INT_U64:
            return rotate_type(uint64, 0);
        case INT_S8:
            return rotate_type(sint8, 0);
        case INT_S16:
            return rotate_type(sint16, 0);
        case INT_S32:
            return rotate_type(sint32, 0);
        case INT_S64:
            return rotate_type(sint64, 0);
        case CharKeyword:
            return rotate_type(chr, 0);
        case BoolKeyword:
            return rotate_type(boolean, 0);
        case OpenSQRBrackets:
            TODO("arr rotate_type parse");
            return rotate_type(dynamic_array, 0);
        case Identifier:
            return rotate_type(undecided, 0);
        default: {
            /*
                Arrays, structures and enums
            */
            TODO("parse type");
        }
    }

    return rotate_type(invalid, 0);
}

} // namespace rotate
