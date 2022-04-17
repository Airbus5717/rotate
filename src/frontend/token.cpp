#include "include/token.hpp"

// TODO: convert tokens to cstring funcs
namespace rotate
{

const char *tkn_type_describe(const token_type type) noexcept
{
    switch (type)
    {
        case TknTypeIdentifier:
            return "identifier";
        case TknTypeBuiltinFunc:
            return "builtin_func";
        case TknTypeEqual:
            return "assign'='";
        case TknTypeInteger:
            return "integer";
        case TknTypeBinaryInteger:
            return "binary int";
        case TknTypeHexInteger:
            return "Hex int";
        case TknTypeSemiColon:
            return "semicolon';'";
        case TknTypeColon:
            return "colon':'";
        case TknTypeString:
            return "string";
        case TknTypeFunction:
            return "function'fn'";
        case TknTypePLUS:
            return "plus'+'";
        case TknTypeMINUS:
            return "minus'-'";
        case TknTypeStar:
            return "star'*'";
        case TknTypeDIV:
            return "divide'/'";
        case TknTypeLeftParen:
            return "leftparen'('";
        case TknTypeRightParen:
            return "rightparen')'";
        case TknTypeLeftCurly:
            return "leftcurlybrkt'{'";
        case TknTypeRightCurly:
            return "rightcurlybrkt'}'";
        case TknTypeLeftSQRBrackets:
            return "leftsqrbrkt']'";
        case TknTypeRightSQRBrackets:
            return "rightsqrbrkt'['";
        case TknTypeChar:
            return "char";
        case TknTypeReturn:
            return "return";
        case TknTypeImport:
            return "import";
        case TknTypeIf:
            return "if";
        case TknTypeElse:
            return "else";
        case TknTypeFor:
            return "for";
        case TknTypeWhile:
            return "while";
        case TknTypeGreater:
            return "greater'>'";
        case TknTypeLess:
            return "less'<'";
        case TknTypeTrue:
            return "bool:true";
        case TknTypeFalse:
            return "bool:false";
        case TknTypeDot:
            return "dot";
        case TknTypeNot:
            return "not'!'";
        case TknTypeAnd:
            return "and";
        case TknTypeOr:
            return "or";
        case TknTypeDoubleQuotes:
            return "double quotes";
        case TknTypeQuote:
            return "quote";
        case TknTypeFloat:
            return "float";
        case TknTypeLet:
            return "let";
        case TknTypeComma:
            return "comma','";
        case TknTypePublic:
            return "public";
        case TknTypeNotEqual:
            return "`!=` not eql";
        case TknTypeMutable:
            return "mutable";
        case TknTypeCharKeyword:
            return "char_word";
        case TknTypeFloatKeyword:
            return "float_word";
        case TknTypeIntKeyword:
            return "int_word";
        case TknTypeMatch:
            return "match";
        case TknTypeStringKeyword:
            return "str_word";
        case TknTypeBoolKeyword:
            return "bool_word";
        case TknTypeAs:
            return "as";
        case TknTypeEqualEqual:
            return "equality";
        case TknTypeBreak:
            return "break";
        case TknTypeAddEqual:
            return "add_equal";
        case TknTypeDivEqual:
            return "divide_equal";
        case TknTypeMultEqual:
            return "multiply_equal";
        case TknTypeSubEqual:
            return "subtract_equal";
        case TknTypeStruct:
            return "struct";
        case TknTypeRef:
            return "ref";
        case TknTypeVoid:
            return "void";
        case TknTypeInclude:
            return "include";
        case TknTypeEnum:
            return "enum";
        case TknTypeNil:
            return "nil (null)";
        case TknTypeVar:
            return "var";
        case TknTypeEOT:
            return "End OF Tokens";
        default:
            return "???";
    }
}

const char *get_keyword_or_type(const token_type type) noexcept
{
    switch (type)
    {
        case TknTypeAs:
            return "as";
        case TknTypeFunction:
            return "fn";
        case TknTypeIf:
            return "if";
        case TknTypeOr:
            return "or";
        case TknTypeFor:
            return "for";
        case TknTypeLet:
            return "let";
        case TknTypePublic:
            return "pub";
        case TknTypeMutable:
            return "mut";
        case TknTypeStringKeyword:
            return "str";
        case TknTypeIntKeyword:
            return "int";
        case TknTypeRef:
            return "ref";
        case TknTypeAnd:
            return "and";
        case TknTypeNil:
            return "nil";
        case TknTypeVar:
            return "var";
        case TknTypeEnum:
            return "enum";
        case TknTypeElse:
            return "else";
        case TknTypeTrue:
            return "true";
        case TknTypeCharKeyword:
            return "char";
        case TknTypeBoolKeyword:
            return "bool";
        case TknTypeVoid:
            return "void";
        case TknTypeWhile:
            return "while";
        case TknTypeFalse:
            return "false";
        case TknTypeMatch:
            return "match";
        case TknTypeBreak:
            return "break";
        case TknTypeReturn:
            return "return";
        case TknTypeImport:
            return "import";
        case TknTypeFloatKeyword:
            return "float";
        case TknTypeStruct:
            return "struct";

        // symbols
        case TknTypeEqualEqual:
            return "==";
        case TknTypeEqual:
            return "=";
        case TknTypeColon:
            return ":";
        case TknTypeSemiColon:
            return ";";
        case TknTypeAddEqual:
            return "+=";
        case TknTypeSubEqual:
            return "-=";
        case TknTypeMultEqual:
            return "*=";
        case TknTypeDivEqual:
            return "/=";
        case TknTypePLUS:
            return "+";
        case TknTypeMINUS:
            return "-";
        case TknTypeStar:
            return "*";
        case TknTypeDIV:
            return "/";
        case TknTypeLeftParen:
            return "(";
        case TknTypeRightParen:
            return ")";
        case TknTypeLeftCurly:
            return "{";
        case TknTypeRightCurly:
            return "}";
        case TknTypeLeftSQRBrackets:
            return "[";
        case TknTypeRightSQRBrackets:
            return "]";
        case TknTypeGreater:
            return ">";
        case TknTypeLess:
            return "<";
        case TknTypeDot:
            return ".";
        case TknTypeNotEqual:
            return "!=";
        case TknTypeNot:
            return "!";
        case TknTypeComma:
            return ",";
        case TknTypeEOT:
            return "end_of_tokens";
        default:
            return NULL;
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
        case EXPECT_LEFT_PAREN_AFTER_FN_ID:
            return "Parentheses required after function identifier";
        case EXPECT_RIGHT_PAREN_AFTER_LEFT_PAREN:
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
        default:
            break;
    }
    return "TODO: error msg implementation.";
}

const char *advice(const error_type error) noexcept
{
    switch (error)
    {
        case EXPECT_LEFT_PAREN_AFTER_FN_ID:
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
        case EXPECT_RIGHT_PAREN_AFTER_LEFT_PAREN:
            return "Function parameters unsupported right now";
        case FN_TYPE_REQUIRED:
            return "Add return type to function";
        case TABS:
            return "Convert the tabs to spaces";
        default:
            break;
    }
    return "Error message unimplemented";
}

} // namespace rotate
