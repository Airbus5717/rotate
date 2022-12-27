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
    TInvalid,
    TVoid, // NOTE(5717): ONLY ALLOWED FOR FUNCTIONS
    TInt,
    TUInt,
    TFloat,
    TChar, // u8
    TBool,
    TArray, // strings are char arrays
    TStruct,
    TEnum, // a member

    TId_Struct_or_Enum, // must be converted to struct or enum during type prep
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
    // TODO: optimize representation
    TypeIndex idx;
    BaseType type;
    bool is_pointer;
    bool is_const;
};

struct SymbolTable
{
    std::vector<Type> types;
    std::vector<EnumType> enums_types;
    std::vector<StructType> structs_types;
    std::vector<ArrayType> array_types;
};

const char *get_base_type_string(Type);
// const char *get_type_modifier_attr_string(TypeAttr);

}; // namespace rotate
