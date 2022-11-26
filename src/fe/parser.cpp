#include "parser.hpp"
#include "token.hpp"

namespace rotate
{

// constructor
Parser::Parser(file_t *file, Lexer *lexer) : file(file), tokens(lexer->getTokens()), idx(0)
{
    ASSERT_NULL(lexer, "Passed lexer to parser was null");
    ASSERT_NULL(file, "Passed file to parser was null");
}

Parser::~Parser() = default;

u8
Parser::parse_lexer()
{
    // NOTE(5717): error handling in parser
    // The parser will stop at the first error occured
    if (tokens->size() < 2) return EXIT_FAILURE;
    return parse_director();
}

u8
Parser::parse_director()
{
    while (true)
    {
        u8 exit       = EXIT_SUCCESS;
        const Token c = current(), p = peek();
        error = PrsErr::Unknown;
        switch (c.type)
        {
        case TknType::Identifier: {
            if (p.type == TknType::ColonColon)
            {
                advance();
                advance();
                switch (current().type)
                {
                case TknType::Import: exit = parse_import(); break;
                case TknType::Function: exit = parse_function(); break;
                case TknType::Struct: exit = parse_struct(); break;
                case TknType::Enum: exit = parse_enum(); break;
                // TODO Global variables
                default: exit = parse_gl_var();
                }
                if (exit == EXIT_FAILURE) return parse_error_use_global_err();
            }
            else
            {
                return parse_error_expect_token(TknType::ColonColon);
            }
        }
        break;
        case TknType::EOT: return EXIT_SUCCESS;
        default: return parse_error_expect_token(TknType::Identifier);
        }
    }
    return EXIT_FAILURE;
}

// Import statements
u8
Parser::parse_import()
{
    advance(); // skip 'import'
    // Skip '('
    expect(current().type == TknType::OpenParen, advance(), error = PrsErr::OpenParents);
    // Skip the string
    expect(current().type == TknType::String, advance(), error = PrsErr::ImportStringExpect);

    ast.imports.push_back(AstImport(past().index));

    // TODO error Parser error types
    expect(current().type == TknType::CloseParen, advance(), error = PrsErr::CloseParents);
    expect_semicolon(advance(), error = PrsErr::SemicolonExpect);
    return EXIT_SUCCESS;
}

u8
Parser::parse_gl_var()
{
    TODO("parse global variables");
    return EXIT_FAILURE;
}

// Functions
u8
Parser::parse_function()
{
    TODO("parse functions");
    return EXIT_FAILURE;
}

// Enums
u8
Parser::parse_enum()
{
    TODO("parse enums");
    return EXIT_FAILURE;
}

u8
Parser::parse_struct()
{
    TODO("parse structs");
    return EXIT_FAILURE;
}

Token
Parser::current() const
{
    return tokens->at(idx);
}

Token
Parser::past() const
{
    return tokens->at(idx - 1);
}

Token
Parser::peek() const
{
    return tokens->at(idx + 1);
}

void
Parser::advance()
{
    idx++;
}

u8
Parser::parse_error_expect_token(TknType expected)
{
    return parser_error(convert_tkn_type_to_parse_error(expected));
}

PrsErr
Parser::convert_tkn_type_to_parse_error(TknType tkn)
{
    switch (tkn)
    {
    default: TODO("Parser handle tkn Type conversion");
    }
    return PrsErr::Unknown;
}

u8
Parser::parser_error(PrsErr err)
{
    Token c  = current();
    Uint low = c.index, line = c.line, len = c.length;

    //
    Uint col = 0;
    while (file->contents[low] != '\n' && low > 0)
    {
        low--;
        col++;
    }
    low = low > 1 ? low + 1 : 0;

    //
    Uint _length = c.index;
    while (file->contents[_length] != '\n' && _length + 1 < file->length)
        _length++;

    _length -= low;

    // error msg
    fprintf(stderr, "> %s%s%s:%u:%u: %serror: %s%s%s\n", BOLD, WHITE, file->name, line, col, LRED,
            LBLUE, parser_error_msg(err), RESET);

    // line from source code
    fprintf(stderr, " %s%u%s | %.*s\n", LYELLOW, line, RESET, _length, (file->contents + low));

    Uint num_line_digits = get_digits_from_number(line);

    // arrows pointing to error location
    Uint spaces = c.index - low + 1;
    if (len < 101)
    {
        char *arrows = (char *)alloca(len + 1);
        memset(arrows, '^', len);
        arrows[len] = '\0';

        fprintf(stderr, " %*c |%*c%s%s%s\n", num_line_digits, ' ', spaces, ' ', LRED, BOLD, arrows);
    }
    else
    {
        fprintf(stderr, " %*c |%*c%s%s^^^---...\n", num_line_digits, ' ', spaces, ' ', LRED, BOLD);
    }
    // error lexer_err_advice
    fprintf(stderr, "> Advice: %s%s\n", RESET, parser_error_advice(err));
    return EXIT_FAILURE;
}

const char *
Parser::parser_error_msg(PrsErr err)
{
    for (auto const &p : parser_errors)
        if (p.err == err) return p.msg;
    return "TODO: Parser error msg";
}

const char *
Parser::parser_error_advice(PrsErr err)
{
    for (auto const &p : parser_errors)
        if (p.err == err) return p.advice;
    return "TODO: Parser error msg";
}

u8
Parser::parse_error_use_global_err()
{
    return parser_error(error);
}
} // namespace rotate
