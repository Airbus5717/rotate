#include "parser.hpp"
#include "errmsgs.hpp"
#include "token.hpp"

namespace rotate
{

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
    return parse_starter();
}

u8
Parser::parse_starter()
{
    for (;;)
    {
        u8 exit = EXIT_SUCCESS;
        Token c = current();
        error   = PrsErr::Unknown;
        switch (c.type)
        {
        case TknType::Import: exit = parse_import(); break;
        case TknType::Function: exit = parse_function(); break;

        // NOTE(5717): Global Variable
        case TknType::Identifier: exit = parse_gl_var(); break;
        case TknType::Struct: exit = parse_struct(); break;
        case TknType::Enum: exit = parse_enum(); break;
        // NOTE(5717): End of parsing phase
        case TknType::EOT: {
            log_debug("End of Parsing");
            return EXIT_SUCCESS;
        }
        default: return parse_error_use_global_err();
        }
        if (exit == EXIT_FAILURE) return parser_error(error);
    }
    UNREACHABLE();
    return EXIT_FAILURE;
}

// Import statements
u8
Parser::parse_import()
{
    // import "std/io";
    advance(); // skip 'import'
    // Skip the string
    expect(current().type == TknType::String, advance(), error = PrsErr::ImportStringExpect);
    if (current().type == TknType::As)
    {
        // import "std/io" as io;
        // import string as id;
        advance();
        expect(current().type == TknType::Identifier, advance(), error = PrsErr::ImportId);
        ast.imports.push_back(AstImport(past().index - 2, past().index));
        // ast.imports.emplace_back(past().index - 2, past().index);
        log_debug("added an aliased import");
    }
    else
    {
        ast.imports.push_back(AstImport(past().index));
        // ast.imports.emplace_back(past().index);
        log_debug("added an import");
    }
    expect_semicolon(advance(), error = PrsErr::SemicolonExpect);
    return EXIT_SUCCESS;
}

u8
Parser::parse_gl_var()
{
    // NOTE(5717):
    // Global Const | Variable
    // public or not
    // static or not
    TODO("parse global variables");
    return EXIT_FAILURE;
}

// Functions
u8
Parser::parse_function()
{
    /* NOTE(5717):
     fn id() {...}
     fn id(params...) {...}
     fn id() type {...}
     fn id(params...) type {...}
    */
    advance(); // skip 'fn'

    // TODO:
    expect(current().type == TknType::Identifier, advance(), error = PrsErr::FnId);
    expect(current().type == TknType::OpenParen, advance(), error = PrsErr::OpenParents);
    if (current().type != TknType::CloseParen)
    {
        TODO("Parse function parameters");
    }

    expect(current().type == TknType::CloseParen, advance(), error = PrsErr::CloseParents);
    if (current().type != TknType::OpenCurly)
    {
        TODO("parse function non-void types");
    }
    else
    {
        TODO("parse function blocks");
    }

    // ast.functions.push_back(AstFn());
    TODO("Functions");
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

Type
Parser::parse_type()
{
    Type ftype{};
    BaseType btype = BaseType::TInvalid;

    if (current().type == TknType::Ref)
    {

        ftype.is_pointer = true;
        advance();
    }

    if (current().type == TknType::OpenSQRBrackets)
    {
        TODO("Parse Array type");
    }

    // Base
    switch (current().type)
    {
    case TknType::UintKeyword: btype = BaseType::TUInt; break;
    case TknType::IntKeyword: btype = BaseType::TInt; break;
    case TknType::BoolKeyword: btype = BaseType::TBool; break;
    case TknType::FloatKeyword: btype = BaseType::TFloat; break;
    case TknType::CharKeyword: btype = BaseType::TChar; break;
    case TknType::Identifier: {
        // aliases, structs, enums
        TODO("parse identifier types");
        btype = BaseType::TId_Struct_or_Enum;
        break;
    }
    default: break;
    }
    // if (peek().type == TknType::Colon) ftype.is_const = true;
    ftype.type = btype;
    return ftype;
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
    UINT low = c.index, line = c.line, len = c.length;

    //
    UINT col = 0;
    while (file->contents[low] != '\n' && low > 0)
    {
        low--;
        col++;
    }
    low = low > 1 ? low + 1 : 0;

    //
    UINT _length = c.index;
    while (file->contents[_length] != '\n' && _length + 1 < file->length)
        _length++;

    _length -= low;

    // error msg
    fprintf(rstderr, "> %s%s%s:%u:%u: %serror: %s%s%s\n", BOLD, WHITE, file->name, line, col, LRED,
            LBLUE, parser_error_msg(err), RESET);

    // line from source code
    fprintf(rstderr, " %s%u%s | %.*s\n", LYELLOW, line, RESET, _length, (file->contents + low));

    UINT num_line_digits = get_digits_from_number(line);

    // arrows pointing to error location
    UINT spaces = c.index - low + 1;
    if (len < 101)
    {
        char *arrows = (char *)alloca(len + 1);
        memset(arrows, '^', len);
        arrows[len] = '\0';

        fprintf(rstderr, " %*c |%*c%s%s%s\n", num_line_digits, ' ', spaces, ' ', LRED, BOLD,
                arrows);
    }
    else
    {
        fprintf(rstderr, " %*c |%*c%s%s^^^---...\n", num_line_digits, ' ', spaces, ' ', LRED, BOLD);
    }
    // error lexer_err_advice
    fprintf(rstderr, "> Advice: %s%s\n", RESET, parser_error_advice(err));
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
