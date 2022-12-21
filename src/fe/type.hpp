#pragma once

#include "token.hpp"

namespace rotate
{

enum class BaseType : u8
{
    TInt = 0,
    TUINT,
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

struct Type
{
    BaseType type;
    TypeAttr attr;
};

const char *get_type_string(Type);
const char *get_type_modifier_attr_string(TypeAttr);

}; // namespace rotate
