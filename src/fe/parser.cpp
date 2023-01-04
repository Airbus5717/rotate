#include "parser.hpp"
#include "errmsgs.hpp"
#include "token.hpp"
#include "type.hpp"

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
    if (tokens->size() < 2) return FAILURE;
    return parse_starter();
}

u8
Parser::parse_starter()
{
    for (;;)
    {
        u8 exit = SUCCESS;
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
            return SUCCESS;
        }
        default: return parse_error_use_global_err();
        }
        if (exit == FAILURE) return parser_error(error);
    }
    UNREACHABLE();
    return FAILURE;
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
    return SUCCESS;
}

u8
Parser::parse_gl_var()
{
    // NOTE(5717):
    // Global Const | Variable
    TknIdx id = current().index;
    UNUSED(id);
    advance();
    if (current().type == TknType::ColonColon)
    {
        // NOTE(5717): a const variable branch
        // NOTE(5717): requires type to be inferred
        auto value = parse_expr(TknType::SemiColon);
        UNUSED(value);
        TODO("parse expression gl vr");
    }
    expect(current().type == TknType::Colon, advance(), error = PrsErr::GlobalVarColon);
    switch (current().type)
    {
    case TknType::Colon: {
        TODO("Parse inferred const");
        break;
    }
    case TknType::Equal: {
        TODO("Parse inferred mutable");
        break;
    }
    default: break;
    }
    Type t = parse_type();
    expect(t.type != BaseType::TInvalid, advance(), error = PrsErr::ParseType);
    if (current().type == TknType::Colon)
    {
        t.is_const = true;
    }
    else
    {
        expect(current().type == TknType::Equal, advance(), error = PrsErr::GlobalVarEql);
        t.is_const = false;
    }
    TODO("parse expr");
    expect_semicolon(advance(), error = PrsErr::GlobalVarSemiColon);
    return SUCCESS;
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

    // Id
    expect(current().type == TknType::Identifier, advance(), error = PrsErr::FnId);
    // (
    expect(current().type == TknType::OpenParen, advance(), error = PrsErr::OpenParents);
    // parse function parameters
    if (current().type != TknType::CloseParen)
    {
        TODO("Parse function parameters");
    }
    // close function params with close parent ')'
    expect(current().type == TknType::CloseParen, advance(), error = PrsErr::CloseParents);
    // if not void function parse type
    if (current().type != TknType::OpenCurly)
    {
        TODO("parse function non-void types");
    }
    else // parse function block
    {
        expect(current().type == TknType::OpenCurly, advance(), error = PrsErr::OpenCurly);
        TODO("parse function blocks");
    }

    // ast.functions.push_back(AstFn());
    TODO("Functions");
    return FAILURE;
}

// Enums
u8
Parser::parse_enum()
{
    TODO("parse enums");
    return FAILURE;
}

u8
Parser::parse_struct()
{
    TODO("parse structs");
    return FAILURE;
}

Type
Parser::parse_type()
{
    // TODO: optimize types using bitfields
    Type ftype{};
    BaseType btype = BaseType::TInvalid;

    // Parse Arrays
    if (current().type == TknType::OpenSQRBrackets)
    {
        TODO("Parse Array type");
    }

    if (current().type == TknType::Ref)
    {
        ftype.is_pointer = true;
        advance();
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

u8
Parser::parse_expr(TknType delimiter)
{
    /*
     * NOTE(5717): An Expression definition will be either
     *
     * - Parse expr Primary
     *   Primary_literal (usually a single token)
     *   i.e. numbers, chars etc.
     *
     * - Parse Array literal
     *   [ Expr... ] // seperated with commas [0, inf(hardcode a max number)]
     *
     * - Parse expr Binary
     *   Expr Binary_operator Expr
     *   i.e. `1 + 1`
     *
     * - Parse expr Unary (Prefix)
     *   Unary_operator Expr
     *   i.e. `-1`
     *
     * - Parse expr Grouping (no special representation(will reorder child of trees))
     *  ( Expr )
     *  i.e. (1 + 1)
     *
     * - Function Call (Non void type)
     *  NOTE(5717): Void function calls are statements
     *  */
    TODO("Implement parsing expressions");
    return FAILURE;
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
    return FAILURE;
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
