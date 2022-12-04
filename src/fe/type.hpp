#pragma once

#include "token.hpp"

namespace rotate
{

enum class BaseType : u8
{
    TInt = 0,
    TUint,
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
    staticMutable   = 1,
    // NOTE(5717): immutable
    staticConstVar = 2,
    constVar       = 3,
};

static const char *
get_type_modifier_attr_string(TypeAttr a)
{
    switch (a)
    {
    case TypeAttr::staticConstVar: return "static const";
    case TypeAttr::constVar: return "const";
    case TypeAttr::staticMutable: return "static";
    case TypeAttr::mutableVariable: return "mutable";
    default: break;
    }
    TODO("implement get type modifier as string method");
    return "UNKNOWN";
}

struct Type
{
    // NOTE(5717): index in symbol tables [3 tables (structs, enums, arrays)]
    u32 type_index;
    BaseType type;
    TypeAttr modifier;
};

// TODO: more complex types such as multidimention arrays, structures, enums and type aliases
struct SymbolTable
{
};

struct Structure
{
};

struct Enum
{
};

}; // namespace rotate
