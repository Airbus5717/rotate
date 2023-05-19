#pragma once

#include "../include/common.hpp"
#include "../include/file.hpp"

namespace rotate
{
typedef uint TknIdx;

enum class TknType : u8
{
    Identifier = 0, // ids
    BuiltinFunc,    // @ids
    To,             // ..
    In,             // in
    As,             // 'as'
    Delete,         // 'delete'
    Equal,          // =
    Integer,        // refers to 10 digits ints
                    //
    //    HexInteger,       // refers to Hexidecimal ints
    //    BinaryInteger,    // refers to binary ints
    IntKeyword,       // 'int'
    UintKeyword,      // 'uint'
    Float,            // refer to floats
    FloatKeyword,     // 'float'
    String,           // refer to strings
    Char,             // refers to chars
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
    GreaterEql,       // >=
    Less,             // <
    LessEql,          // <=
    Dot,              // .
    Not,              // "!"
    NotEqual,         // "!="
    And,              // 'and'
    Or,               // 'or'
    Comma,            // ,
    Public,           // 'pub'
    Switch,           // 'switch'
    Enum,             // 'enum'
    EqualEqual,       // ==
    Break,            // 'break'
    AddEqual,         // +=
    SubEqual,         // -=
    MultEqual,        // *=
    DivEqual,         // /=
    Struct,           // 'struct'
    Ref,              // 'ref' // TODO later
    Nil,              // `nil` basically null
    Void,             // `void`
    EOT,              // EOT - END OF TOKENS
};                    // enum TknType

cstr tkn_type_describe(const TknType type) noexcept;

struct Token
{
    uint index, length, line;
    TknType type;

    Token(uint index, uint length, uint line, TknType type)
        : index(index), length(length), line(line), type(type)
    {
    }
};

enum class LexErr : u8
{
    // Unknown token/error (default)
    UNKNOWN,
    OUT_OF_MEMORY,
    LEXER_INVALID_CHAR,
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
    UNSUPPORTED,
}; // enum LexErr

cstr get_keyword_or_type(cstr, const Token &);
cstr lexer_err_advice(const LexErr) noexcept;
cstr lexer_err_msg(const LexErr) noexcept;
bool is_token_type_length_variable(TknType);
bool is_token_a_number(TknType);

} // namespace rotate
