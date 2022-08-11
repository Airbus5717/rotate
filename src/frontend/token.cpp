#include "include/token.hpp"

// TODO: convert tokens to cstring funcs
namespace rotate
{

const char *tkn_type_describe(const token_type type) noexcept
{
    switch (type)
    {
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
        case token_type::DoubleQuotes:
            return "double quotes `\"`";
        case token_type::Quote:
            return "quote `'`";
        case token_type::Float:
            return "float";
        case token_type::Let:
            return "let";
        case token_type::Comma:
            return "comma','";
        case token_type::Public:
            return "public";
        case token_type::NotEqual:
            return "`!=` not eql";
        case token_type::Const:
            return "const";
        case token_type::CharKeyword:
            return "char_word";
        case token_type::FloatKeyword:
            return "float_word";
        case token_type::IntKeyword:
            return "int_word";
        case token_type::Match:
            return "match";
        case token_type::StringKeyword:
            return "str_word";
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
        case token_type::Include:
            return "include";
        case token_type::Enum:
            return "enum";
        case token_type::Nil:
            return "nil (null)";
        case token_type::Var:
            return "var";
        case token_type::EscapedChar:
            return "escaped_char";
        case token_type::EOT:
            return "End OF Tokens";
        case token_type::INT_U8:
            return "u8";
        case token_type::INT_U16:
            return "u16";
        case token_type::INT_U32:
            return "u32";
        case token_type::INT_U64:
            return "u64";
        case token_type::INT_S8:
            return "s8";
        case token_type::INT_S16:
            return "s16";
        case token_type::INT_S32:
            return "s32";
        case token_type::INT_S64:
            return "s64";
        default:
            return "???";
    }
}

const char *get_keyword_or_type(const char *string, const token &tkn)
{
    switch (tkn.type)
    {
        case token_type::Function:
            return "fn";
        case token_type::If:
            return "if";
        case token_type::Or:
            return "or";
        case token_type::For:
            return "for";
        case token_type::Let:
            return "let";
        case token_type::Public:
            return "pub";
        case token_type::Const:
            return "const";
        case token_type::StringKeyword:
            return "str";
        case token_type::IntKeyword:
            return "int";
        case token_type::Ref:
            return "ref";
        case token_type::And:
            return "and";
        case token_type::Nil:
            return "nil";
        case token_type::Var:
            return "var";
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
        case token_type::Match:
            return "match";
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
        case token_type::INT_U8:
            return "u8";
        case token_type::INT_U16:
            return "u16";
        case token_type::INT_U32:
            return "u32";
        case token_type::INT_U64:
            return "u64";
        case token_type::INT_S8:
            return "s8";
        case token_type::INT_S16:
            return "s16";
        case token_type::INT_S32:
            return "s32";
        case token_type::INT_S64:
            return "s64";
        default:
            return strndup(string + tkn.index, tkn.length);
    }
}

const char *err_msgsfunc(const error_type error) noexcept
{
    switch (error)
    {
        case error_type::EXPECTED_ID_FOR_FN:
            return "Id expected after `fn` keyword";
        case error_type::OUT_OF_MEMORY:
            return "Out of memory";
        case error_type::LEXER_INVALID_CHAR:
            return "Invalid character";
        case error_type::TOO_LONG_IDENTIFIER:
            return "Identifier is too long";
        case error_type::TOO_LONG_NUMBER:
            return "Number is too long";
        case error_type::TOO_LONG_STRING:
            return "String is too long";
        case error_type::NOT_CLOSED_CHAR:
            return "The char is not closed.";
        case error_type::NOT_CLOSED_STRING:
            return "The string is not closed.";
        case error_type::END_OF_FILE:
            return "reached end of file.";
        case error_type::FILE_EMPTY:
            return "The file is empty.";
        case error_type::BAD_TOKEN_AT_GLOBAL:
            return "Found global token at its forbidden scope";
        case error_type::IMPORT_NOT_SUPPORTED:
            return "Import external files unsupported";
        case error_type::GLOBAL_VAR_MUTABLE:
            return "Global variables cannot be mutable";
        case error_type::EXPECT_ID_AFTER_LET:
            return "Id expected after `let` keyword";
        case error_type::EXPECT_EQUAL_AFTER_ID:
            return "Equal expected after Id";
        case error_type::REQUIRED_GLOBAL_VAR_TYPE:
            return "Global variables require variable type";
        case error_type::EXPECTED_VALUE_AFTER_EQUAL:
            return "Variable requires a value";
        case error_type::VARIABLE_TYPE_MATCH:
            return "Variable type and value's type do not match";
        case error_type::SEMICOLON_END_REQUIREMENT:
            return "Semicolon required at the end of the statement";
        case error_type::EXPECT_OPEN_PAREN_AFTER_FN_ID:
            return "Parentheses required after function identifier";
        case error_type::EXPECT_CLOSE_PAREN_AFTER_OPEN_PAREN:
            return "Closing parentheses required after opening one";
        case error_type::EXPECTED_ARROW_OR_BLOCK:
            return "Expected Arrow or Block";
        case error_type::EXPECTED_IMPORT_ID:
            return "Expected Id after import";
        case error_type::INVALID_EXPORT_DIR:
            return "No such directory for export";
        case error_type::FN_TYPE_REQUIRED:
            return "Function requires return type";
        case error_type::TABS:
            return "Tabs '\\t' are unsupported";
        case error_type::NOT_VALID_ESCAPE_CHAR:
            return "Invalid escaped char";
        default:
            break;
    }
    return "TODO: error msg implementation.";
}

const char *advice(const error_type error) noexcept
{
    switch (error)
    {
        case error_type::EXPECT_OPEN_PAREN_AFTER_FN_ID:
            return "Add parentheses after function identifier";
        case error_type::LEXER_INVALID_CHAR:
            return "remove this character";
        case error_type::OUT_OF_MEMORY:
            return "The compiler needs more RAM";
        case error_type::TOO_LONG_IDENTIFIER:
            return "Identifier must not exceed 100 characters";
        case error_type::TOO_LONG_NUMBER:
            return "Number must not exceed 100 digits";
        case error_type::TOO_LONG_STRING:
            return "String must not exceed (UINT_MAX / 100000) characters";
        case error_type::EXPECTED_ID_FOR_FN:
            return "Add an Identifier after 'fn' keyword";
        case error_type::NOT_CLOSED_CHAR:
            return "Close the char with a quote";
        case error_type::NOT_CLOSED_STRING:
            return "Close the string with a double quote";
        case error_type::END_OF_FILE:
            return "Needs more code for compiling";
        case error_type::FILE_EMPTY:
            return "Do not compile empty files";
        case error_type::BAD_TOKEN_AT_GLOBAL:
            return "Do not put this token in global scope";
        case error_type::IMPORT_NOT_SUPPORTED:
            return "Remove the import statement or comment it";
        case error_type::GLOBAL_VAR_MUTABLE:
            return "Remove the mutable `mut` keyword";
        case error_type::EXPECT_ID_AFTER_LET:
            return "Add an Identifier after 'let' keyword";
        case error_type::EXPECT_EQUAL_AFTER_ID:
            return "Add an equal '=' after the Id";
        case error_type::REQUIRED_GLOBAL_VAR_TYPE:
            return "Add type after colon";
        case error_type::EXPECTED_VALUE_AFTER_EQUAL:
            return "Add value to variable";
        case error_type::VARIABLE_TYPE_MATCH:
            return "Change variable types to match its value type";
        case error_type::SEMICOLON_END_REQUIREMENT:
            return "Add a semicolon after the expression";
        case error_type::INVALID_EXPORT_DIR:
            return "Directory specified after '-o' flag is invalid";
        case error_type::EXPECT_CLOSE_PAREN_AFTER_OPEN_PAREN:
            return "Function parameters unsupported CLOSE now";
        case error_type::FN_TYPE_REQUIRED:
            return "Add return type to function";
        case error_type::TABS:
            return "Convert the tabs to spaces";
        default:
            break;
    }
    return ("\x1b[33m"
            "TODO: error msg implementation."
            "\x1b[0m");
}

} // namespace rotate
