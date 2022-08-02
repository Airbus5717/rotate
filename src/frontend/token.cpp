#include "include/token.hpp"

// TODO: convert tokens to cstring funcs
namespace rotate
{

const char *tkn_type_describe(const token_type type) noexcept
{
    switch (type)
    {
        case Identifier:
            return "identifier";
        case BuiltinFunc:
            return "builtin_func";
        case Equal:
            return "assign'='";
        case Integer:
            return "integer";
        case BinaryInteger:
            return "binary int";
        case HexInteger:
            return "Hex int";
        case SemiColon:
            return "semicolon';'";
        case Colon:
            return "colon':'";
        case String:
            return "string";
        case Function:
            return "function'fn'";
        case PLUS:
            return "plus'+'";
        case MINUS:
            return "minus'-'";
        case Star:
            return "star'*'";
        case DIV:
            return "divide'/'";
        case OpenParen:
            return "openparen'('";
        case CloseParen:
            return "closeparen')'";
        case OpenCurly:
            return "opencurlybrkt'{'";
        case CloseCurly:
            return "closecurlybrkt'}'";
        case OpenSQRBrackets:
            return "opensqrbrkt']'";
        case CloseSQRBrackets:
            return "closesqrbrkt'['";
        case Char:
            return "char";
        case Return:
            return "return";
        case Import:
            return "import";
        case If:
            return "if";
        case Else:
            return "else";
        case For:
            return "for";
        case While:
            return "while";
        case Greater:
            return "greater'>'";
        case Less:
            return "less'<'";
        case True:
            return "bool:true";
        case False:
            return "bool:false";
        case Dot:
            return "dot";
        case Not:
            return "not'!'";
        case And:
            return "and";
        case Or:
            return "or";
        case DoubleQuotes:
            return "double quotes";
        case Quote:
            return "quote";
        case Float:
            return "float";
        case Let:
            return "let";
        case Comma:
            return "comma','";
        case Public:
            return "public";
        case NotEqual:
            return "`!=` not eql";
        case Const:
            return "const";
        case CharKeyword:
            return "char_word";
        case FloatKeyword:
            return "float_word";
        case IntKeyword:
            return "int_word";
        case Match:
            return "match";
        case StringKeyword:
            return "str_word";
        case BoolKeyword:
            return "bool_word";

        case EqualEqual:
            return "equality";
        case Break:
            return "break";
        case AddEqual:
            return "add_equal";
        case DivEqual:
            return "divide_equal";
        case MultEqual:
            return "multiply_equal";
        case SubEqual:
            return "subtract_equal";
        case Struct:
            return "struct";
        case Ref:
            return "ref";
        case Void:
            return "void";
        case Include:
            return "include";
        case Enum:
            return "enum";
        case Nil:
            return "nil (null)";
        case Var:
            return "var";
        case EscapedChar:
            return "escaped_char";
        case EOT:
            return "End OF Tokens";
        case INT_U8:
            return "u8";
        case INT_U16:
            return "u16";
        case INT_U32:
            return "u32";
        case INT_U64:
            return "u64";
        case INT_S8:
            return "s8";
        case INT_S16:
            return "s16";
        case INT_S32:
            return "s32";
        case INT_S64:
            return "s64";
        default:
            return "???";
    }
}

const char *get_keyword_or_type(const char *string, const token &tkn)
{
    switch (tkn.type)
    {

        case Function:
            return "fn";
        case If:
            return "if";
        case Or:
            return "or";
        case For:
            return "for";
        case Let:
            return "let";
        case Public:
            return "pub";
        case Const:
            return "const";
        case StringKeyword:
            return "str";
        case IntKeyword:
            return "int";
        case Ref:
            return "ref";
        case And:
            return "and";
        case Nil:
            return "nil";
        case Var:
            return "var";
        case Enum:
            return "enum";
        case Else:
            return "else";
        case True:
            return "true";
        case CharKeyword:
            return "char";
        case BoolKeyword:
            return "bool";
        case Void:
            return "void";
        case While:
            return "while";
        case False:
            return "false";
        case Match:
            return "match";
        case Break:
            return "break";
        case Return:
            return "return";
        case Import:
            return "import";
        case FloatKeyword:
            return "float";
        case Struct:
            return "struct";

        // symbols
        case EqualEqual:
            return "==";
        case Equal:
            return "=";
        case Colon:
            return ":";
        case SemiColon:
            return ";";
        case AddEqual:
            return "+=";
        case SubEqual:
            return "-=";
        case MultEqual:
            return "*=";
        case DivEqual:
            return "/=";
        case PLUS:
            return "+";
        case MINUS:
            return "-";
        case Star:
            return "*";
        case DIV:
            return "/";
        case OpenParen:
            return "(";
        case CloseParen:
            return ")";
        case OpenCurly:
            return "{";
        case CloseCurly:
            return "}";
        case OpenSQRBrackets:
            return "[";
        case CloseSQRBrackets:
            return "]";
        case Greater:
            return ">";
        case Less:
            return "<";
        case Dot:
            return ".";
        case NotEqual:
            return "!=";
        case Not:
            return "!";
        case Comma:
            return ",";
        case EOT:
            return "end_of_tokens";
        case INT_U8:
            return "u8";
        case INT_U16:
            return "u16";
        case INT_U32:
            return "u32";
        case INT_U64:
            return "u64";
        case INT_S8:
            return "s8";
        case INT_S16:
            return "s16";
        case INT_S32:
            return "s32";
        case INT_S64:
            return "s64";
        default:
            return strndup(string + tkn.index, tkn.length);
    }
}

const char *err_msgsfunc(const error_type error) noexcept
{
    switch (error)
    {
        case EXPECTED_ID_FOR_FN:
            return "Id expected after `fn` keyword";
        case OUT_OF_MEMORY:
            return "Out of memory";
        case LEXER_INVALID_CHAR:
            return "Invalid character";
        case TOO_LONG_IDENTIFIER:
            return "Identifier is too long";
        case TOO_LONG_NUMBER:
            return "Number is too long";
        case TOO_LONG_STRING:
            return "String is too long";
        case NOT_CLOSED_CHAR:
            return "The char is not closed.";
        case NOT_CLOSED_STRING:
            return "The string is not closed.";
        case END_OF_FILE:
            return "reached end of file.";
        case FILE_EMPTY:
            return "The file is empty.";
        case BAD_TOKEN_AT_GLOBAL:
            return "Found global token at its forbidden scope";
        case IMPORT_NOT_SUPPORTED:
            return "Import external files unsupported";
        case GLOBAL_VAR_MUTABLE:
            return "Global variables cannot be mutable";
        case EXPECT_ID_AFTER_LET:
            return "Id expected after `let` keyword";
        case EXPECT_EQUAL_AFTER_ID:
            return "Equal expected after Id";
        case REQUIRED_GLOBAL_VAR_TYPE:
            return "Global variables require variable type";
        case EXPECTED_VALUE_AFTER_EQUAL:
            return "Variable requires a value";
        case VARIABLE_TYPE_MATCH:
            return "Variable type and value's type do not match";
        case SEMICOLON_END_REQUIREMENT:
            return "Semicolon required at the end of the statement";
        case EXPECT_OPEN_PAREN_AFTER_FN_ID:
            return "Parentheses required after function identifier";
        case EXPECT_CLOSE_PAREN_AFTER_OPEN_PAREN:
            return "Closing parentheses required after opening one";
        case EXPECTED_ARROW_OR_BLOCK:
            return "Expected Arrow or Block";
        case EXPECTED_IMPORT_ID:
            return "Expected Id after import";
        case INVALID_EXPORT_DIR:
            return "No such directory for export";
        case FN_TYPE_REQUIRED:
            return "Function requires return type";
        case TABS:
            return "Tabs '\\t' are unsupported";
        case NOT_VALID_ESCAPE_CHAR:
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
        case EXPECT_OPEN_PAREN_AFTER_FN_ID:
            return "Add parentheses after function identifier";
        case LEXER_INVALID_CHAR:
            return "remove this character";
        case OUT_OF_MEMORY:
            return "The compiler needs more RAM";
        case TOO_LONG_IDENTIFIER:
            return "Identifier must not exceed 100 characters";
        case TOO_LONG_NUMBER:
            return "Number must not exceed 100 digits";
        case TOO_LONG_STRING:
            return "String must not exceed (UINT_MAX / 100000) characters";
        case EXPECTED_ID_FOR_FN:
            return "Add an Identifier after 'fn' keyword";
        case NOT_CLOSED_CHAR:
            return "Close the char with a quote";
        case NOT_CLOSED_STRING:
            return "Close the string with a double quote";
        case END_OF_FILE:
            return "Needs more code for compiling";
        case FILE_EMPTY:
            return "Do not compile empty files";
        case BAD_TOKEN_AT_GLOBAL:
            return "Do not put this token in global scope";
        case IMPORT_NOT_SUPPORTED:
            return "Remove the import statement or comment it";
        case GLOBAL_VAR_MUTABLE:
            return "Remove the mutable `mut` keyword";
        case EXPECT_ID_AFTER_LET:
            return "Add an Identifier after 'let' keyword";
        case EXPECT_EQUAL_AFTER_ID:
            return "Add an equal '=' after the Id";
        case REQUIRED_GLOBAL_VAR_TYPE:
            return "Add type after colon";
        case EXPECTED_VALUE_AFTER_EQUAL:
            return "Add value to variable";
        case VARIABLE_TYPE_MATCH:
            return "Change variable types to match its value type";
        case SEMICOLON_END_REQUIREMENT:
            return "Add a semicolon after the expression";
        case INVALID_EXPORT_DIR:
            return "Directory specified after '-o' flag is invalid";
        case EXPECT_CLOSE_PAREN_AFTER_OPEN_PAREN:
            return "Function parameters unsupported CLOSE now";
        case FN_TYPE_REQUIRED:
            return "Add return type to function";
        case TABS:
            return "Convert the tabs to spaces";
        default:
            break;
    }
    return ("\x1b[33m"
            "TODO: error msg implementation."
            "\x1b[0m");
}

} // namespace rotate
