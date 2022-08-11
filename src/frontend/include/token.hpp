#ifndef ROTATE_TOKEN
#define ROTATE_TOKEN

#include "../../include/common.hpp"
#include "../../include/file.hpp"

namespace rotate
{

enum class token_type : u8
{
    Identifier,       // ids
    BuiltinFunc,      // @ids
    Equal,            // =
    Let,              // 'let'
    Var,              // 'var'
    Const,            // 'const'
    Integer,          // refers to 10 digits ints
    HexInteger,       // refers to Hexidecimal ints
    BinaryInteger,    // refers to binary ints
    IntKeyword,       // 'int'
    INT_U8,           // u8
    INT_U16,          // u16
    INT_U32,          // u32
    INT_U64,          // u64
    INT_S8,           // s8
    INT_S16,          // s16
    INT_S32,          // s32
    INT_S64,          // s64
    Float,            // refer to floats
    FLOAT_f32,        // f32
    FLOAT_f64,        // f64
    FloatKeyword,     // 'float'
    String,           // refer to strings
    StringKeyword,    // 'str'
    Char,             // refers to chars
    EscapedChar,      // '\{}' chars i.e. '\n'
    CharKeyword,      // 'char'
    True,             // 'true'
    False,            // 'false'
    BoolKeyword,      // 'bool'
    SemiColon,        // ;
    Colon,            // :
    Function,         // 'fn'
    PLUS,             // +
    MINUS,            // -
    Star,             // *
    DIV,              // /
    OpenParen,        // (
    CloseParen,       // )
    OpenCurly,        // {
    CloseCurly,       // }
    OpenSQRBrackets,  // [
    CloseSQRBrackets, // ]
    Return,           // 'return'
    Import,           // 'import'
    If,               // 'if'
    Else,             // 'else'
    For,              // 'for'
    While,            // 'while'
    Greater,          // >
    Less,             // <
    Dot,              // .
    Not,              // "!"
    NotEqual,         // "!="
    And,              // 'and'
    Or,               // 'or'
    DoubleQuotes,     // "
    Quote,            // '
    Comma,            // ,
    Public,           // 'pub'
    Match,            // 'match'
    Enum,             // 'enum'
    EqualEqual,       // ==
    Break,            // 'break'
    AddEqual,         // +=
    SubEqual,         // -=
    MultEqual,        // *=
    DivEqual,         // /=
    Struct,           // 'struct'
    Ref,              // 'ref'
    Void,             // 'void'
    Include,          // 'include'
    Nil,              // `nil` basically null
    EOT,              // EOT - END OF TOKENS
};

enum class error_type : u32
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
    NOT_CLOSED_COMMENT,
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
    EXPECT_OPEN_PAREN_AFTER_FN_ID,
    EXPECT_CLOSE_PAREN_AFTER_OPEN_PAREN,
    EXPECTED_ARROW_OR_BLOCK,
    INVALID_EXPORT_DIR,
    FN_TYPE_REQUIRED,
};

struct token
{
    token_type type;
    u32 index;
    u32 length;

    token(token_type type, u32 index, u32 length) : type(type), index(index), length(length)
    {
    }
};

const char *tkn_type_describe(const token_type type) noexcept;
const char *get_keyword_or_type(const char *string, const token &tkn);
const char *advice(const error_type error) noexcept;
const char *err_msgsfunc(const error_type error) noexcept;
bool is_token_type_length_variable(token_type type);

} // namespace rotate

#endif // ROTATE_TOKEN