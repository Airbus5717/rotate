#include "token.hpp"

// TODO: convert tokens to cstring funcs
namespace rotate
{

const char *tkn_type_describe(const token_type type) noexcept
{
    switch (type)
    {
        case token_type::In:
            return "in";
        case token_type::To:
            return "to";
        case token_type::Delete:
            return "delete";
        case token_type::CharKeyword:
            return "char";
        case token_type::Identifier:
            return "identifier";
        case token_type::BuiltinFunc:
            return "builtin_func";
        case token_type::Equal:
            return "assign'='";
        case token_type::Integer:
            return "integer";
        case token_type::BinaryInteger:
            return "binary int";
        case token_type::HexInteger:
            return "Hex int";
        case token_type::SemiColon:
            return "semicolon';'";
        case token_type::Colon:
            return "colon':'";
        case token_type::String:
            return "string";
        case token_type::Function:
            return "function'fn'";
        case token_type::PLUS:
            return "plus'+'";
        case token_type::MINUS:
            return "minus'-'";
        case token_type::Star:
            return "star'*'";
        case token_type::DIV:
            return "divide'/'";
        case token_type::OpenParen:
            return "openparen'('";
        case token_type::CloseParen:
            return "closeparen')'";
        case token_type::OpenCurly:
            return "opencurlybrkt'{'";
        case token_type::CloseCurly:
            return "closecurlybrkt'}'";
        case token_type::OpenSQRBrackets:
            return "opensqrbrkt']'";
        case token_type::CloseSQRBrackets:
            return "closesqrbrkt'['";
        case token_type::Char:
            return "char";
        case token_type::Return:
            return "return";
        case token_type::Import:
            return "import";
        case token_type::If:
            return "if";
        case token_type::Else:
            return "else";
        case token_type::For:
            return "for";
        case token_type::While:
            return "while";
        case token_type::Greater:
            return "greater'>'";
        case token_type::Less:
            return "less'<'";
        case token_type::True:
            return "bool:true";
        case token_type::False:
            return "bool:false";
        case token_type::Dot:
            return "dot'.'";
        case token_type::Not:
            return "not'!'";
        case token_type::And:
            return "and";
        case token_type::Or:
            return "or";
        case token_type::Float:
            return "float";
        case token_type::Comma:
            return "comma','";
        case token_type::Public:
            return "public";
        case token_type::NotEqual:
            return "`!=` not eql";
            return "char_word";
        case token_type::FloatKeyword:
            return "float_word";
        case token_type::IntKeyword:
            return "int_word";
        case token_type::Switch:
            return "switch";
        case token_type::BoolKeyword:
            return "bool_word";

        case token_type::EqualEqual:
            return "equality";
        case token_type::Break:
            return "break";
        case token_type::AddEqual:
            return "add_equal";
        case token_type::DivEqual:
            return "divide_equal";
        case token_type::MultEqual:
            return "multiply_equal";
        case token_type::SubEqual:
            return "subtract_equal";
        case token_type::Struct:
            return "struct";
        case token_type::Ref:
            return "ref";
        case token_type::Enum:
            return "enum";
        case token_type::Nil:
            return "nil (null)";
        case token_type::ColonColon:
            return "::";
        case token_type::UintKeyword:
            return "uint";
        case token_type::GreaterEql:
            return ">=";
        case token_type::LessEql:
            return "<=";
        case token_type::EOT:
            return "End OF Tokens";
        default:
            __builtin_unreachable();
            return "man IDK";
    }
}

const char *get_keyword_or_type(const char *string, const Token &tkn)
{

    switch (tkn.type)
    {
        case token_type::In:
            return "in";
        case token_type::To:
            return "..";
        case token_type::Delete:
            return "delete";
        case token_type::GreaterEql:
            return ">=";
        case token_type::LessEql:
            return "<=";
        case token_type::ColonColon:
            return "::";
        case token_type::UintKeyword:
            return "uint";
        case token_type::Function:
            return "fn";
        case token_type::If:
            return "if";
        case token_type::Or:
            return "or";
        case token_type::For:
            return "for";
        case token_type::Public:
            return "pub";
        // case token_type::StringKeyword:
        // return "str";
        case token_type::IntKeyword:
            return "int";
        case token_type::Ref:
            return "ref";
        case token_type::And:
            return "and";
        case token_type::Nil:
            return "nil";
        case token_type::Enum:
            return "enum";
        case token_type::Else:
            return "else";
        case token_type::True:
            return "true";
        case token_type::CharKeyword:
            return "char";
        case token_type::BoolKeyword:
            return "bool";
        case token_type::While:
            return "while";
        case token_type::False:
            return "false";
        case token_type::Switch:
            return "switch";
        case token_type::Break:
            return "break";
        case token_type::Return:
            return "return";
        case token_type::Import:
            return "import";
        case token_type::FloatKeyword:
            return "float";
        case token_type::Struct:
            return "struct";

        // symbols
        case token_type::EqualEqual:
            return "==";
        case token_type::Equal:
            return "=";
        case token_type::Colon:
            return ":";
        case token_type::SemiColon:
            return ";";
        case token_type::AddEqual:
            return "+=";
        case token_type::SubEqual:
            return "-=";
        case token_type::MultEqual:
            return "*=";
        case token_type::DivEqual:
            return "/=";
        case token_type::PLUS:
            return "+";
        case token_type::MINUS:
            return "-";
        case token_type::Star:
            return "*";
        case token_type::DIV:
            return "/";
        case token_type::OpenParen:
            return "(";
        case token_type::CloseParen:
            return ")";
        case token_type::OpenCurly:
            return "{";
        case token_type::CloseCurly:
            return "}";
        case token_type::OpenSQRBrackets:
            return "[";
        case token_type::CloseSQRBrackets:
            return "]";
        case token_type::Greater:
            return ">";
        case token_type::Less:
            return "<";
        case token_type::Dot:
            return ".";
        case token_type::NotEqual:
            return "!=";
        case token_type::Not:
            return "!";
        case token_type::Comma:
            return ",";
        case token_type::EOT:
            return "end_of_tokens";
        case token_type::Integer:
        case token_type::Float:
        case token_type::BinaryInteger:
        case token_type::HexInteger:
        case token_type::Identifier:
        case token_type::String:
        case token_type::Char:
        case token_type::BuiltinFunc:
            return strndup(string + tkn.index, tkn.length);
        default: {
            __builtin_unreachable();
            return "TODO: IMPLEMENT";
        }
    }
}

const char *lexer_err_msg(const LexerErrorType error) noexcept
{
    switch (error)
    {
        case LexerErrorType::OUT_OF_MEMORY:
            return "Out of memory";
        case LexerErrorType::LEXER_INVALID_CHAR:
            return "Invalid character";
        case LexerErrorType::TOO_LONG_IDENTIFIER:
            return "Identifier is too long";
        case LexerErrorType::TOO_LONG_NUMBER:
            return "Number is too long";
        case LexerErrorType::TOO_LONG_STRING:
            return "String is too long";
        case LexerErrorType::NOT_CLOSED_CHAR:
            return "The char is not closed.";
        case LexerErrorType::NOT_CLOSED_STRING:
            return "The string is not closed.";
        case LexerErrorType::END_OF_FILE:
            return "reached end of file.";
        case LexerErrorType::FILE_EMPTY:
            return "The file is empty.";
        case LexerErrorType::BAD_TOKEN_AT_GLOBAL:
            return "Found global token at its forbidden scope";
        case LexerErrorType::TABS:
            return "Tabs '\\t' are unsupported";
        case LexerErrorType::NOT_VALID_ESCAPE_CHAR:
            return "Invalid escaped char";
        default:
            break;
    }
    __builtin_unreachable();
    return "TODO: error msg implementation.";
}

const char *lexer_err_advice(const LexerErrorType error) noexcept
{
    switch (error)
    {
        case LexerErrorType::LEXER_INVALID_CHAR:
            return "remove this character";
        case LexerErrorType::OUT_OF_MEMORY:
            return "The compiler needs more RAM";
        case LexerErrorType::TOO_LONG_IDENTIFIER:
            return "Identifier must not exceed 100 characters";
        case LexerErrorType::TOO_LONG_NUMBER:
            return "Number must not exceed 100 digits";
        case LexerErrorType::TOO_LONG_STRING:
            return "String must not exceed (UINT_MAX / 100000) characters";
        case LexerErrorType::NOT_CLOSED_CHAR:
            return "Close the char with a quote";
        case LexerErrorType::NOT_CLOSED_STRING:
            return "Close the string with a double quote";
        case LexerErrorType::END_OF_FILE:
            return "Needs more code for compiling";
        case LexerErrorType::FILE_EMPTY:
            return "Do not compile empty files";
        case LexerErrorType::BAD_TOKEN_AT_GLOBAL:
            return "Do not put this token in global scope";
        case LexerErrorType::TABS:
            return "Convert the tabs to spaces";
        default:
            break;
    }
    return ("\x1b[33m"
            "TODO: error msg implementation."
            "\x1b[0m");
}

bool is_token_type_a_number(token_type tt)
{
    switch (tt)
    {
        case token_type::Integer:
        case token_type::BinaryInteger:
        case token_type::Float:
        case token_type::HexInteger:
            return true;
        default:
            return false;
    }
}

} // namespace rotate
