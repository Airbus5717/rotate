#include "token.hpp"

// TODO: convert tokens to cstring funcs
namespace rotate
{

cstr
tkn_type_describe(const TknType type) noexcept
{
    switch (type)
    {
        case TknType::As: return "as";
        case TknType::In: return "in";
        case TknType::To: return "to";
        case TknType::Delete: return "delete";
        case TknType::CharKeyword: return "char";
        case TknType::Identifier: return "identifier";
        case TknType::BuiltinFunc: return "builtin_func";
        case TknType::Equal: return "assign'='";
        case TknType::Integer: return "integer";
        case TknType::Terminator: return "terminator';'|'\\n'";
        case TknType::Colon: return "colon':'";
        case TknType::String: return "string";
        case TknType::Function: return "function'fn'";
        case TknType::PLUS: return "plus'+'";
        case TknType::MINUS: return "minus'-'";
        case TknType::Star: return "star'*'";
        case TknType::DIV: return "divide'/'";
        case TknType::OpenParen: return "openparen'('";
        case TknType::CloseParen: return "closeparen')'";
        case TknType::OpenCurly: return "opencurlybrkt'{'";
        case TknType::CloseCurly: return "closecurlybrkt'}'";
        case TknType::OpenSQRBrackets: return "opensqrbrkt']'";
        case TknType::CloseSQRBrackets: return "closesqrbrkt'['";
        case TknType::Char: return "char";
        case TknType::Return: return "return";
        case TknType::Import: return "import";
        case TknType::If: return "if";
        case TknType::Else: return "else";
        case TknType::For: return "for";
        case TknType::While: return "while";
        case TknType::Greater: return "greater'>'";
        case TknType::Less: return "less'<'";
        case TknType::True: return "bool:true";
        case TknType::False: return "bool:false";
        case TknType::Dot: return "dot'.'";
        case TknType::Not: return "not'!'";
        case TknType::And: return "and";
        case TknType::Or: return "or";
        case TknType::Float: return "float";
        case TknType::Comma: return "comma','";
        case TknType::Public: return "public";
        case TknType::NotEqual: return "`!=` not eql"; return "char_word";
        case TknType::FloatKeyword: return "float_word";
        case TknType::IntKeyword: return "int_word";
        case TknType::Switch: return "switch";
        case TknType::BoolKeyword: return "bool_word";
        case TknType::EqualEqual: return "equality";
        case TknType::Break: return "break";
        case TknType::AddEqual: return "add_equal";
        case TknType::DivEqual: return "divide_equal";
        case TknType::MultEqual: return "multiply_equal";
        case TknType::SubEqual: return "subtract_equal";
        case TknType::Struct: return "struct";
        case TknType::Ref: return "ref";
        case TknType::Enum: return "enum";
        case TknType::Nil: return "nil (null)";
        case TknType::UintKeyword: return "uint";
        case TknType::GreaterEql: return ">=";
        case TknType::LessEql: return "<=";
        case TknType::Void: return "void";
        case TknType::EOT: return "End OF Tokens";

        default: return "UNKNOWN";
    }
}

cstr
get_keyword_or_type(cstr string, const Token &tkn)
{

    switch (tkn.type)
    {
        case TknType::As: return "as";
        case TknType::In: return "in";
        case TknType::To: return "..";
        case TknType::Delete: return "delete";
        case TknType::GreaterEql: return ">=";
        case TknType::LessEql: return "<=";
        case TknType::UintKeyword: return "uint";
        case TknType::Function: return "fn";
        case TknType::If: return "if";
        case TknType::Or: return "or";
        case TknType::For: return "for";
        case TknType::Public: return "pub";
        case TknType::IntKeyword: return "int";
        case TknType::Ref: return "ref";
        case TknType::And: return "and";
        case TknType::Nil: return "nil";
        case TknType::Enum: return "enum";
        case TknType::Else: return "else";
        case TknType::True: return "true";
        case TknType::CharKeyword: return "char";
        case TknType::BoolKeyword: return "bool";
        case TknType::While: return "while";
        case TknType::False: return "false";
        case TknType::Switch: return "switch";
        case TknType::Break: return "break";
        case TknType::Return: return "return";
        case TknType::Import: return "import";
        case TknType::FloatKeyword: return "float";
        case TknType::Struct: return "struct";
        case TknType::EqualEqual: return "==";
        case TknType::Equal: return "=";
        case TknType::Colon: return ":";
        case TknType::Terminator: return ";|\\n";
        case TknType::AddEqual: return "+=";
        case TknType::SubEqual: return "-=";
        case TknType::MultEqual: return "*=";
        case TknType::DivEqual: return "/=";
        case TknType::PLUS: return "+";
        case TknType::MINUS: return "-";
        case TknType::Star: return "*";
        case TknType::DIV: return "/";
        case TknType::OpenParen: return "(";
        case TknType::CloseParen: return ")";
        case TknType::OpenCurly: return "{";
        case TknType::CloseCurly: return "}";
        case TknType::OpenSQRBrackets: return "[";
        case TknType::CloseSQRBrackets: return "]";
        case TknType::Greater: return ">";
        case TknType::Less: return "<";
        case TknType::Dot: return ".";
        case TknType::NotEqual: return "!=";
        case TknType::Not: return "!";
        case TknType::Comma: return ",";
        case TknType::Void: return "void";
        case TknType::EOT: return "end_of_tokens";
        case TknType::Integer:
        case TknType::Float:
        case TknType::Identifier:
        case TknType::String:
        case TknType::Char:
        case TknType::BuiltinFunc:
            return strndup(string + tkn.index, tkn.length);
            /*default: {
                return "TODO: IMPLEMENT";
            }*/
    }
    ASSERT(false, "implement Type to string situation");
}

cstr
lexer_err_msg(const LexErr error) noexcept
{
    switch (error)
    {
        case LexErr::OUT_OF_MEMORY: return "Out of memory";
        case LexErr::LEXER_INVALID_CHAR: return "Invalid character";
        case LexErr::TOO_LONG_IDENTIFIER: return "Identifier is too long";
        case LexErr::TOO_LONG_NUMBER: return "Number is too long";
        case LexErr::TOO_LONG_STRING: return "String is too long";
        case LexErr::NOT_CLOSED_CHAR: return "The char is not closed.";
        case LexErr::NOT_CLOSED_STRING: return "The string is not closed.";
        case LexErr::END_OF_FILE: return "reached end of file.";
        case LexErr::FILE_EMPTY: return "The file is empty.";
        case LexErr::BAD_TOKEN_AT_GLOBAL: return "Found global token at its forbidden scope";
        case LexErr::TABS: return "Tabs '\\t' are unsupported";
        case LexErr::NOT_VALID_ESCAPE_CHAR: return "Invalid escaped char";
        case LexErr::WINDOWS_CRAP: return "Windows style files are not accepted \\r";
        case LexErr::NOT_CLOSED_COMMENT: return "Comment not closed";
        case LexErr::UNSUPPORTED: break;
        case LexErr::UNKNOWN: break;
    }
    return "TODO: error msg implementation.";
}

cstr
lexer_err_advice(const LexErr error) noexcept
{
    switch (error)
    {
        case LexErr::NOT_VALID_ESCAPE_CHAR: return "Change the letter after \\";
        case LexErr::NOT_CLOSED_COMMENT: return "Close the comment with delimiter";
        case LexErr::LEXER_INVALID_CHAR: return "remove this character";
        case LexErr::OUT_OF_MEMORY: return "The compiler needs more memory";
        case LexErr::TOO_LONG_IDENTIFIER: return "Identifier must not exceed 100 characters";
        case LexErr::TOO_LONG_NUMBER: return "Number must not exceed 100 digits";
        case LexErr::TOO_LONG_STRING: return "String must not exceed (UINT_MAX / 2) characters";
        case LexErr::NOT_CLOSED_CHAR: return "Close the char with a quote";
        case LexErr::NOT_CLOSED_STRING: return "Close the string with a double quote";
        case LexErr::END_OF_FILE: return "Needs more code for compiling";
        case LexErr::FILE_EMPTY: return "Do not compile empty files";
        case LexErr::BAD_TOKEN_AT_GLOBAL: return "Do not put this token in global scope";
        case LexErr::TABS: return "Convert the tabs to spaces";
        case LexErr::WINDOWS_CRAP: return "Files must be LF style";
        case LexErr::UNSUPPORTED: break;
        case LexErr::UNKNOWN: break;
    }
    return "TODO: error msg implementation.";
}

} // namespace rotate
