#pragma once

#include "token.hpp"

namespace rotate
{
// NOTE(5717): not to be confused with TknIdx
// Type Index will pointer multiple symbol tables and will differtiate
// by the type
typedef UINT TypeIndex;  // Type index
typedef UINT ExprIdx;    // Expression index
typedef UINT ExprLocIdx; // Expression location index

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
    TId,   // TO BE VALIDATED
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
    u8 attributes;
    u8 array_level; // must be > 0
    // NOTE:
    // do not use 0 and 8
    // 1 => ptr
    // 2 => const
    void set_pointer(bool is_ptr)
    {
        bit_set(attributes, is_ptr);
    }

    void set_const(bool is_c)
    {
        bit_set(attributes, is_c);
    }
};

struct SymbolTable
{
    std::vector<Type> types;
    std::vector<EnumType> enums_types;
    std::vector<StructType> structs_types;
    // std::vector<ArrayType> array_types;
};

const char *get_base_type_string(Type);
// const char *get_type_modifier_attr_string(TypeAttr);

}; // namespace rotate
