#pragma once

#include "token.hpp"

namespace rotate
{

enum class BaseType : u8
{
    TInt,
    TUint,
    TFloat,
    TChar, // u8
    TBool,
    TArray, // strings are char arrays
    TStruct,
    TEnum,
};

enum class Attr : u8
{
    // NOTE(5717): mutable
    mutableVariable = 0,
    staticMutable   = 1,
    // NOTE(5717): immutable
    staticConstVar = 2,
    constVar       = 3,
};

static const char *
get_type_modifier_attr_string(Attr a)
{
    switch (a)
    {
    case Attr::staticConstVar: return "static const";
    case Attr::constVar: return "const";
    case Attr::staticMutable: return "static";
    case Attr::mutableVariable: return "mutable";
    default: break;
    }
    TODO("implement get type modifier to string method");
    return "UNKNOWN";
}

struct Type
{
    // NOTE(5717): index in symbol tables [3 tables (structs, enums, arrays)]
    u32 type_index;
    BaseType type;
    Attr modifier;
};

// TODO: more complex types such as multidimention arrays, structures, enums and type aliases
struct SymbolTable
{
};

struct SymbolTableStructs
{
};

struct SymbolTableArrays
{
};

struct SymbolTableEnums
{
};

}; // namespace rotate
