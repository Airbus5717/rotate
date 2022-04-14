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
    TknTypeInteger,          // refers to ints
    TknTypeIntKeyword,       // 'int'
    TknTypeFloat,            // refer to floats
    TknTypeFloatKeyword,     // 'float'
    TknTypeString,           // refer to strings
    TknTypeStringKeyword,    // 'str'
    TknTypeChar,             // refers to chars
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

const char *tkn_type_describe(const token_type type);
const char *get_keyword_or_type(const token_type type);

struct token
{
    token_type type;
    usize line = 0, col = 0, index = 0;
    std::string_view value;

    token(token_type type, usize line, usize col, usize index, std::string_view str)
    {
        this->type  = type;
        this->line  = line;
        this->col   = col;
        this->index = index;
        this->value = str;
    }
};

} // namespace rotate

#endif // ROTATE_TOKEN