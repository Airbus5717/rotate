#include "include/parser.hpp"
#include "include/token.hpp"

#include "../include/defines.hpp"

namespace rotate
{

//
Parser::Parser(Lexer *lexer) : lexer(lexer), tokens(lexer->getTokens())
{
    ASSERT_NULL(this->tokens, "lexer tokens passed is null");
}

// TODO:
Parser::~Parser() = default;

u8 Parser::parse()
{
    exit = 0, index = 0;
    for (;;)
    {
        bool is_public = false;
        if (current().type == token_type::Public)
        {
            advance();
            is_public = true;
        }
        switch (current().type)
        {
            case token_type::Import:
                exit = parse_gl_imports();
                break;
            case token_type::Function:
                exit = parse_gl_function(is_public);
                break;
            case token_type::Const:
                exit = parse_gl_var_const(is_public);
                break;
            case token_type::Struct:
                exit = parse_gl_struct(is_public);
                break;
            case token_type::Enum:
                exit = parse_gl_enum(is_public);
                break;
            case token_type::EOT:
                return exit;
            default:
                return parser_report_error(current());
        }
        if (exit == EXIT_FAILURE) return parser_report_error(current());
    }
    return exit;
}

// throws error msg during mismatch
inline bool Parser::expect(token_type _type)
{
    if (_type == current().type)
    {
        advance();
        return true;
    }

    //! be careful of returning (cuz the return value is a u8)
    //! which defaults to EXIT_SUCCESS OR EXIT_FAILURE;
    //! bools are opposite
    //! ---V--- notice the negate symbol;
    return !parser_report_error(current());
}

// does not throw error msgs
inline bool Parser::consume(token_type _type)
{
    // same as expect but returns false without throwing error
    if (_type == current().type)
    {
        advance();
        return true;
    }
    return false;
}

inline Token Parser::current()
{
    return tokens->at(index);
}

inline void Parser::advance()
{
    index++;
}

inline Token Parser::past()
{
    return tokens->at(index - 1);
}

inline Token Parser::peek()
{
    return tokens->at(index + 1);
}

u8 Parser::parse_gl_imports()
{
    advance();
    // advance ----v to next (which should be a string)
    // EXAMPLE() `import "io";`
    //                     | |
    if (expect(token_type::String)) GLImports.emplace_back(past().index);
    //                       |
    //                       |
    if (expect(token_type::SemiColon)) return EXIT_SUCCESS;
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_function(bool is_public)
{
    advance();
    TODO("function parser implementation");
    UNUSED(is_public);
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_var_const(bool is_public)
{
    // const id = node;
    advance();
    auto _id_index = current().index;
    if (!expect(token_type::Identifier)) return EXIT_FAILURE;
    if (!expect(token_type::Equal)) return EXIT_FAILURE;
    GLConstants.emplace_back(_id_index, is_public, RType(rt_type::undecided, true), parse_node());
    return !expect(token_type::SemiColon);
}

u8 Parser::parse_gl_struct(bool is_public)
{
    advance();
    TODO("structures parser implementation");
    UNUSED(is_public);
    return EXIT_FAILURE;
}

u8 Parser::parse_gl_enum(bool is_public)
{
    advance();
    TODO("enum parser implementation");
    UNUSED(is_public);
    return EXIT_FAILURE;
}

bool Parser::is_token_terminator(token_type _type)
{
    // `;`, `}`, `)`, `,`
    switch (_type)
    {
        case token_type::SemiColon:
        case token_type::CloseCurly:
        case token_type::CloseParen:
        case token_type::Comma:
            return true;
        default:
            return false;
    }

    return false;
}

ASTNode *Parser::parse_next_node()
{
    advance();
    return parse_node();
}

void Parser::parse_grouping(ASTNode *node)
{
    advance();
    node = new GroupingNode(current().index, parse_node());
    expect(token_type::CloseParen);
    UNUSED(node);
}

ASTNode *Parser::parse_primary()
{
    auto tkn      = current();
    ASTNode *node = nullptr;
    if (tkn.type == token_type::OpenParen)
        parse_grouping(node);
    else if (is_unary(tkn.type))
        parse_unary(node, tkn);
    else if (tkn.type == token_type::Identifier)
        parse_function_or_name(node);
    else if (is_literal(tkn.type) && is_token_terminator(peek().type))
        parse_literal(node, tkn);
    else if (tkn.type == token_type::String)
        node = new StringLiteralNode(current().index, current().length);
    else
        node = parse_binary(node);

    if (!is_token_terminator(current().type) || node == nullptr)
    {
        puts("at parse node");
        parser_report_error(current());
        TODO(tkn_type_describe(current().type));
        return nullptr;
    }

    return node;
}

void Parser::parse_literal(ASTNode *node, Token tkn)
{
    node = new LiteralNode(tkn.index, convert_tkn_type_to_literal_type(tkn.type));
    advance();
    UNUSED(node); // make the compiler shutup
}

void Parser::parse_unary(ASTNode *node, Token tkn)
{
    node = new UnaryNode(tkn.index, convert_tkn_type_to_unary_type(tkn.type), parse_next_node());
    UNUSED(node); // make the compiler shutup
}

ASTNode *Parser::parse_binary(ASTNode *rhs)
{
    TODO("parse _ binary");
    UNUSED(rhs);
    return nullptr;
}

void Parser::parse_function_or_name(ASTNode *node)
{
    if (peek().type == token_type::OpenParen)
        parse_function_call(node);
    else if (peek().type == token_type::Dot)
        parse_object_member(node);
    else if (peek().type == token_type::OpenSQRBrackets)
        parse_array_index(node);
    else
        parse_literal(node, current());
}

void Parser::parse_object_member(ASTNode *node)
{
    // struct.member
    /*
        struct x {
            g: int;
        }

        parsing ---> x.g;
    */
    TODO("parse members");
    UNUSED(node);
}

void Parser::parse_function_call(ASTNode *node)
{
    // TODO: parse arguments + consume commas
    UNUSED(node);
    TODO("parse function call");
}

void Parser::parse_array_index(ASTNode *node)
{
    consume(token_type::OpenSQRBrackets);
    node = new ArrayIndexLiteral(past().index, parse_node());
    expect(token_type::CloseSQRBrackets);
    UNUSED(node); // makes the compiler shutup
}

ASTNode *Parser::parse_node()
{
    //! Make sure to advance before entering this method
    // its critical due to recursive nature of this method
    /*
        * NOTE(Airbus5717): Expression Notes

        *Terminator = {
            `}`, `)`, `;`, `=` or `,` etc.
        }

        Literal = {
            String, Char(also EscapedChar), Integer, Float, Nil, True, False, Identifier,
            ArrayID[index] Builtin_function,
        }

        TODO: Array_literal = {
            [ Nodes seperated with commas ]
        }

        Array = [
          ArrayIdentifier[index]
        ]

        Unary = {
            `!` or `-` then a ASTNode
            TODO: references
        }

        Operators = [
            `+`, `-`, `!=`, `==`,
            `>`, `<`, `>=`, `<=`,
            `*`, `/`, `and`, `or`,
        ]

        First check for unary, literal or grouping

        1 - Unary
        2 - Grouping
        3 - BinaryOp
    */

    return parse_primary();
}

bool Parser::is_literal(token_type type)
{
    switch (type)
    {
        case token_type::Integer:
        case token_type::Nil:
        case token_type::Float:
        case token_type::Char:
        case token_type::True:
        case token_type::False:
            return true;
        default:
            return false;
    }
    return false;
}

bool Parser::is_token_binary_op(token_type type)
{
    // `+`, `-`, `*`, `/`, `>`, `>=`, `!=`
    // `or`, `and`, `==`, `<`, `<=`
    switch (type)
    {
        case token_type::PLUS:
        case token_type::MINUS:
        case token_type::Star:
        case token_type::DIV:
        case token_type::Greater:
        case token_type::GreaterEql:
        case token_type::Less:
        case token_type::LessEql:
        case token_type::EqualEqual:
        case token_type::NotEqual:
        case token_type::And:
        case token_type::Or:
            return true;
        default:
            return false;
    }

    return false;
}

inline s8 Parser::precedence(token_type type)
{
    /*
    SOURCE(https://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy/)
      other       = -1;
      UNUSED(ASSIGNMENT) = 1;
      CONDITIONAL = 2;
      SUM         = 3;
      PRODUCT     = 4;
      UNUSED(EXPONENT) = 5;
      PREFIX      = 6;
      POSTFIX     = 7;
      CALL        = 8;
    */
    switch (type)
    {
        case token_type::Greater:
        case token_type::GreaterEql:
        case token_type::Less:
        case token_type::LessEql:
        case token_type::EqualEqual:
        case token_type::NotEqual:
            // `>`, `>=`, `<`, `<=`, `==`, `!=`,
            return 2;
        case token_type::PLUS:
        case token_type::MINUS:
            // `+`, `-`
            return 3;
        case token_type::Star:
        case token_type::DIV:
            // `*`, `/`
            return 4;
        case token_type::Or:
            // `or`
            return 5;
        case token_type::And:
            // `and`
            return 6;
        case token_type::Not:
            //* TAKE CARE OF PREFIX OPERATORS SUCH AS (-X) CUZ IT MIGHT BE WRONG
            return 7;
        case token_type::Identifier:
        case token_type::BuiltinFunc:
            // `id`, `@id`
            return 8;
        default:
            return -1;
    }

    return -1;
}

bool Parser::is_unary(token_type tt)
{
    switch (tt)
    {
        case token_type::Not:
        case token_type::MINUS: // it could be a binary operator too
            return true;
        default:
            return false;
    }
}

literal_type Parser::convert_tkn_type_to_literal_type(token_type tt)
{
    switch (tt)
    {
        case token_type::Char:
            return literal_type::chr;
        case token_type::Integer:
            return literal_type::integer;
        case token_type::Float:
            return literal_type::float_;
        case token_type::Nil:
            return literal_type::nil;
        case token_type::True:
        case token_type::False:
            return literal_type::boolean;

        default:
            return literal_type::other;
    }
    return literal_type::other;
}

unary_type Parser::convert_tkn_type_to_unary_type(token_type tt)
{
    switch (tt)
    {
        case token_type::MINUS:
            return unary_type::negate_minus;
        case token_type::Not:
            return unary_type::negate_bool;
        default:
            return unary_type::other;
    }
}

RType Parser::parse_type(bool allow_mut)
{

    // TODO: deal with allow_mut
    bool mut = consume(token_type::Mutable) && allow_mut;

    switch (current().type)
    {
        case token_type::INT_U8:
            return RType(rt_type::uint8, mut);
        case token_type::INT_U16:
            return RType(rt_type::uint16, mut);
        case token_type::INT_U32:
            return RType(rt_type::uint32, mut);
        case token_type::INT_U64:
            return RType(rt_type::uint64, mut);
        case token_type::INT_S8:
            return RType(rt_type::sint8, mut);
        case token_type::INT_S16:
            return RType(rt_type::sint16, mut);
        case token_type::INT_S32:
            return RType(rt_type::sint32, mut);
        case token_type::INT_S64:
            return RType(rt_type::sint64, mut);
        case token_type::FLOAT_f32:
            return RType(rt_type::float32, mut);
        case token_type::FLOAT_f64:
            return RType(rt_type::float64, mut);
        case token_type::IntKeyword:
            return RType(rt_type::sint32, mut);
        case token_type::FloatKeyword:
            return RType(rt_type::float32, mut);
        case token_type::Identifier: // will be decided during typechecking
            return RType(rt_type::undecided, mut);
        default:
            TODO("TODO parse this type");
            break;
    }
    return RType(rt_type::no_type, mut);
}

// WARN(Airbus5717): Can slow the compiler by multiple magnitutes
void Parser::save_log(FILE *file)
{
    fprintf(file, "*** GLOBAL IMPORTS\n");
    for (const auto &a : GLImports)
        fprintf(file, "%s\n", a.to_string().c_str());

    // GL constants
    fprintf(file, "\n*** GLOBAL CONSTANTS \n");
    for (const auto &c : GLConstants)
        fprintf(file, "%s\n", c.to_string().c_str());

    // GL Functions (functions are always global)
    fprintf(file, "\n*** FUNCTIONS \n");
    for (const auto &f : GLFunctions)
    {
        fprintf(file, "TODO: function foo\n");
        UNUSED(f);
    }

    // Structures
    fprintf(file, "\n*** STRUCTURES \n");
    for (const auto &s : GLStructures)
    {
        fprintf(file, "TODO: s\n");
        UNUSED(s);
    }

    // Enumerations
    fprintf(file, "\n*** ENUMERATIONS \n");
    for (const auto &e : GLEnums)
    {
        fprintf(file, "TODO: enum\n");
        UNUSED(e);
    }
}

u8 Parser::parser_report_error(const Token tkn)
{
    auto file  = lexer->getFile();
    auto len   = tkn.length;
    auto index = tkn.index;
    auto line  = tkn.line;

    u32 low = index, col = 0;
    while (file->contents[low] != '\n' && low > 0)
    {
        low--;
        col++;
    }

    //
    u32 _length = index;
    while (file->contents[_length] != '\n' && _length + 1 < file->length)
        _length++;

    _length -= low;

    // error msg
    fprintf(stderr, "> %s%s%s:%u:%u: %serror: expected :%s found :%s %s%s\n", BOLD, WHITE,
            file->name, line, col, LRED, LBLUE, tkn_type_describe(tkn.type),
            tkn_type_describe(tkn.type), RESET);

    // line from source code
    fprintf(stderr, " %s%u%s | %.*s\n", LYELLOW, line, RESET, _length, (file->contents + low));

    u32 num_line_digits = get_digits_from_number(line);

    // arrows pointing to error location
    u32 spaces = index - low + 1;
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
    TODO("Parser error messages fix implementation");
    return exit = EXIT_FAILURE;
}

} // namespace rotate
