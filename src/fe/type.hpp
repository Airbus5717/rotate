#pragma once

#include "token.hpp"

namespace rotate
{
// NOTE(5717): not to be confused with TknIdx
// Type Index will pointer multiple symbol tables and will differtiate
// by the type
typedef UINT TypeIndex;

enum class BaseType : u8
{
    TVoid, // NOTE(5717): ONLY ALLOWED FOR FUNCTIONS
    TInt,
    TUInt,
    TFloat,
    TChar, // u8
    TBool,
    TArray, // strings are char arrays
    TStruct,
    TEnum,
};

enum class TypeAttr : u8
{
    // NOTE(5717): mutable
    mutableVariable = 0,
    staticMutable   = 1, // NOTE(): unused for now
    // NOTE(5717): immutable
    staticConstVar = 2,
    constVar       = 3,
};

struct ArrayType
{
    BaseType type;
    u8 level;
};

struct StructType
{
    TknIdx id;
    // TODO
};

struct EnumType
{
    TknIdx id;
};

struct Type
{
    TypeIndex idx;
    BaseType type;
    TypeAttr attr;
};

struct SymbolTable
{
    std::vector<Type> types;
    std::vector<EnumType> enums_types;
    std::vector<StructType> structs_types;
    std::vector<ArrayType> array_types;
};

const char *get_type_string(Type);
const char *get_type_modifier_attr_string(TypeAttr);

}; // namespace rotate
