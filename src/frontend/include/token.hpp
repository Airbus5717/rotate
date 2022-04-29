#ifndef ROTATE_TOKEN
#define ROTATE_TOKEN

#include "../../include/common.hpp"
#include "../../include/file.hpp"

namespace rotate
{

enum token_type : u8
{
    TknTypeIdentifier,       // ids
    TknTypeBuiltinFunc,      // @ids
    TknTypeEqual,            // =
    TknTypeLet,              // 'let'
    TknTypeInteger,          // refers to 10 digits ints
    TknTypeHexInteger,       // refers to Hexidecimal ints
    TknTypeBinaryInteger,    // refers to binary ints
    TknTypeIntKeyword,       // 'int'
    TknTypeFloat,            // refer to floats
    TknTypeFloatKeyword,     // 'float'
    TknTypeString,           // refer to strings
    TknTypeStringKeyword,    // 'str'
    TknTypeChar,             // refers to chars
    TknTypeEscapedChar,      // '\{}' chars i.e. '\n'
    TknTypeCharKeyword,      // 'char'
    TknTypeTrue,             // 'true'
    TknTypeFalse,            // 'false'
    TknTypeBoolKeyword,      // 'bool'
    TknTypeSemiColon,        // ;
    TknTypeColon,            // :
    TknTypeFunction,         // 'fn'
    TknTypePLUS,             // +
    TknTypeMINUS,            // -
    TknTypeStar,             // *
    TknTypeDIV,              // /
    TknTypeLeftParen,        // (
    TknTypeRightParen,       // )
    TknTypeLeftCurly,        // {
    TknTypeRightCurly,       // }
    TknTypeLeftSQRBrackets,  // [
    TknTypeRightSQRBrackets, // ]
    TknTypeReturn,           // 'return'
    TknTypeImport,           // 'import'
    TknTypeIf,               // 'if'
    TknTypeElse,             // 'else'
    TknTypeFor,              // 'for'
    TknTypeWhile,            // 'while'
    TknTypeGreater,          // >
    TknTypeLess,             // <
    TknTypeDot,              // .
    TknTypeNot,              // "!"
    TknTypeNotEqual,         // "!="
    TknTypeAnd,              // 'and'
    TknTypeOr,               // 'or'
    TknTypeDoubleQuotes,     // "
    TknTypeQuote,            // '
    TknTypeComma,            // ,
    TknTypePublic,           // 'pub'
    TknTypeMutable,          // 'mut'
    TknTypeMatch,            // 'match'
    TknTypeAs,               // 'as'
    TknTypeEnum,             // 'enum'
    TknTypeEqualEqual,       // ==
    TknTypeBreak,            // 'break'
    TknTypeAddEqual,         // +=
    TknTypeSubEqual,         // -=
    TknTypeMultEqual,        // *=
    TknTypeDivEqual,         // /=
    TknTypeStruct,           // 'struct'
    TknTypeRef,              // 'ref'
    TknTypeVoid,             // 'void'
    TknTypeInclude,          // 'include'
    TknTypeNil,              // `nil` basically null
    TknTypeVar,              // 'var' mutable variable declaration
    TknTypeEOT,              // EOT - END OF TOKENS
};

enum error_type : u32
{
    // Unknown token/error (default)
    UNKNOWN,
    OUT_OF_MEMORY,
    LEXER_INVALID_CHAR,
    LEXER_INVALID_BUILTN_FN,
    // an ID can have up to a specific length
    TOO_LONG_IDENTIFIER,
    TOO_LONG_NUMBER,
    TOO_LONG_STRING,
    // Tabs are not supported
    TABS,
    // '\r'
    WINDOWS_CRAP,
    // Single quote not closed
    NOT_CLOSED_CHAR,
    // Double quote not closed
    NOT_VALID_ESCAPE_CHAR,
    NOT_CLOSED_STRING,
    // File empty error
    FILE_EMPTY,
    // End of file error
    END_OF_FILE,
    // forbidden token in global scope
    BAD_TOKEN_AT_GLOBAL,

    IMPORT_NOT_SUPPORTED,
    // Global variables must not be mutable error,
    EXPECTED_IMPORT_ID,
    GLOBAL_VAR_MUTABLE,
    EXPECT_ID_AFTER_LET,
    EXPECT_EQUAL_AFTER_ID,
    REQUIRED_GLOBAL_VAR_TYPE,
    EXPECTED_VALUE_AFTER_EQUAL,
    VARIABLE_TYPE_MATCH,
    SEMICOLON_END_REQUIREMENT,
    UNSUPPORTED,
    EXPECTED_ID_FOR_FN,
    EXPECT_LEFT_PAREN_AFTER_FN_ID,
    EXPECT_RIGHT_PAREN_AFTER_LEFT_PAREN,
    EXPECTED_ARROW_OR_BLOCK,
    INVALID_EXPORT_DIR,
    FN_TYPE_REQUIRED,
};

struct token
{
    token_type type;
    u32 index;
    u32 length;
    u32 line;
    u32 col; // starts from 1

    token(token_type type, u32 index, u32 length, u32 line, u32 col)
        : type(type), index(index), length(length), line(line), col(col)
    {
    }

    ~token() = default;

};

const char *tkn_type_describe(const token_type type) noexcept;
const char *get_keyword_or_type(const char *string, const token &tkn);
const char *advice(const error_type error) noexcept;
const char *err_msgsfunc(const error_type error) noexcept;
bool is_token_type_length_variable(token_type type);

} // namespace rotate

#endif // ROTATE_TOKEN