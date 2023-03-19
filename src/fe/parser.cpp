#include "parser.hpp"
#include "errmsgs.hpp"
#include "type.hpp"

namespace rotate
{

Parser::Parser(const file_t *file, const Lexer *lexer)
    : file(file), tokens(lexer->get_tokens()), idx(0), ExprIndex(0), LitIdx(0), BinIdx(0),
      UnaryIdx(0), ArrayExprIdx(0), ArraySubIdx(0), FnCallIdx(0), ast(new Ast())
{
    ASSERT_NULL(lexer, "Passed lexer to parser was null");
    ASSERT_NULL(file, "Passed file to parser was null");
}

Parser::~Parser()
{
    delete ast;
}

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
                return exit;
            }
            default: {
                error = PrsErr::GlobalScopeNotAllowed;
                parser_error(error);
            }
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
    TknIdx str = idx;
    expect(current().type == TknType::String, advance(), error = PrsErr::ImportStringExpect);
    if (current().type == TknType::As)
    {
        // import "std/io" as io;
        // import string as id;
        advance();
        TknIdx alias = idx;
        expect(current().type == TknType::Identifier, advance(), error = PrsErr::ImportId);
        ast->imports.push_back(AstImport(alias, str));
        log_debug("added an aliased import");
    }
    else
    {
        ast->imports.push_back(AstImport(str));
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
    TknIdx id = idx;
    advance();
    bool is_valid;
    expect(current().type == TknType::Colon, advance(), error = PrsErr::GlobalVarColon);
    Type t{};
    if (current().type == TknType::Colon || current().type == TknType::Equal)
    {
        // NOTE: type to be inferred in the typechecker
        advance();
        if (past().type == TknType::Colon) t.set_const();
        t.type = BaseType::TInvalid;
    }
    else
    {
        t = parse_type();
        if (peek().type == TknType::Colon) t.set_const();
        advance();
        expect(current().type == TknType::Colon || current().type == TknType::Equal, advance(),
               error = PrsErr::GlobalVarColon);
    }
    auto val = parse_expr(TknType::SemiColon, &is_valid);
    ast->gl_variables.push_back(AstGlVar(id, t, val));
    expect_semicolon(advance(), error = PrsErr::GlobalVarSemiColon);
    return !is_valid; // complement to get the integer truth
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
        auto type = parse_fn_type();
        expect(type.type != BaseType::TInvalid, advance(), );
    }

    // parse function block
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
Parser::parse_fn_type()
{
    // NOTE: allow only functions to optain void type
    Type type{};
    type.type = BaseType::TInvalid;
    switch (current().type)
    {
        case TknType::Void: {
            type.type = BaseType::TVoid;
            break;
        }
        default: return parse_type();
    }
    return type;
}

Type
Parser::parse_type()
{
    // TODO: optimize types using bitfields
    Type ftype{};
    BaseType btype    = BaseType::TInvalid;
    ftype.array_level = 0;
    // Parse Arrays
    if (current().type == TknType::OpenSQRBrackets)
    {
        advance(); // skip [
        if (current().type != TknType::CloseSQRBrackets)
        {
            ftype.type = BaseType::TInvalid;
            return ftype;
        }
        advance();             // skip ]
                               //
        ftype.array_level = 1; // single dimention array
    }

    if (current().type == TknType::Ref)
    {
        ftype.set_pointer();
        advance(); // skip 'ref'
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
            btype = BaseType::TId;
            TODO("add the id to symbol table");
            log_debug("add id to symbol table");
            break;
        }
        default: {
            log_error(tkn_type_describe(current().type));
            TODO("Parse specific type");
        }
    }

    // if (peek().type == TknType::Colon) ftype.is_const = true;
    ftype.type = btype;
    return ftype;
}

ExprIdx
Parser::parse_expr(const TknType delimiter, bool *is_valid)
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
     *  NOT Expressions
     *  */
    ExprIdx i;
    auto c = current(), p = peek();
    switch (c.type)
    {

        case TknType::CloseCurly:
        case TknType::CloseParen:
        case TknType::CloseSQRBrackets:
        case TknType::SemiColon: {
            *is_valid = false;
            return 0;
        }
        default: break;
    }
    // single token expression
    if (p.type == delimiter)
    {
        LitExpr l = parse_literal_expr(delimiter, is_valid);
        ASSERT(*is_valid, "Invalid literal expression");
        i = add_literal_expr(l);
        advance(); // NOTE: advance once becuz of 'single' token
        return i;
    }

    // multi token
    switch (c.type)
    {
        // NOTE: Array Literal
        case TknType::OpenSQRBrackets: {
            ArrayExpr a = parse_array_literal_expr(delimiter, is_valid);
            TODO("check if nothing is after the array");
            return add_array_literal_expr(a);
        }
        default: break;
    }
    // TODO: multi token expression
    TODO("Implement multi token expression");
    return i;
}

LitExpr
Parser::parse_literal_expr(const TknType delimiter, bool *is_valid)
{
    auto c = current(), p = peek();
    if (p.type != delimiter) TODO("non single literal");
    *is_valid = true;
    switch (c.type)
    {
        case TknType::String:
        case TknType::Float:
        case TknType::Char:
        case TknType::True:
        case TknType::False:
        case TknType::Identifier:
        case TknType::Nil:
        case TknType::Integer: {
            return LitExpr(c.index);
        }
        default: break; *is_valid = false;
    }
    return LitExpr(0);
}

ArrayExpr
Parser::parse_array_literal_expr(const TknType delimiter, bool *is_valid)
{
    TODO("Parse array literals");
    ArrayExpr a{};
    UNUSED(delimiter), UNUSED(is_valid), UNUSED(a);
    return ArrayExpr();
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
    ++idx;
}

ExprIdx
Parser::add_literal_expr(const LitExpr lit)
{
    ast->literals.push_back(lit);
    ast->expressions.push_back(Expr(LitIdx++, ExprType::Literal));
    return ExprIndex++;
}

ExprIdx
Parser::add_array_literal_expr(const ArrayExpr a)
{
    ast->arr_exprs.push_back(a);
    ast->expressions.push_back(Expr(ArrayExprIdx++, ExprType::ArrayLiteral));
    return ExprIndex++;
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
    fprintf(stderr, "> %s%s%s:%u:%u: %serror: %s%s%s\n", BOLD, WHITE, file->name, line, col, LRED,
            LBLUE, parser_error_msg(err), RESET);

    // line from source code
    fprintf(stderr, " %s%u%s | %.*s\n", LYELLOW, line, RESET, _length, (file->contents + low));

    UINT num_line_digits = get_digits_from_number(line);

    // arrows pointing to error location
    UINT spaces = c.index - low + 1;
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
    return FAILURE;
}

const char *
Parser::parser_error_msg(PrsErr err)
{
    // TODO: Convert O(n) to O(1)
    for (auto const &p : parser_errors)
        if (p.err == err) return p.msg;
    return "TODO: Parser error msg";
}

const char *
Parser::parser_error_advice(PrsErr err)
{
    // TODO: Convert O(n) to O(1)
    // not important to refactor to O(1) right now
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
