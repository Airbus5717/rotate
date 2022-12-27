#include "type.hpp"

namespace rotate
{

const char *
get_base_type_string(Type t)
{
    switch (t.type)
    {
    case BaseType::TVoid: return "VOID";
    case BaseType::TInt: return "INT";
    case BaseType::TBool: return "BOOL";
    case BaseType::TChar: return "CHAR";
    case BaseType::TUInt: return "UINT";
    case BaseType::TFloat: return "FLOAT";
    case BaseType::TArray: return "ARRAY";
    case BaseType::TEnum: return "ENUM";
    case BaseType::TStruct: return "STRUCT";
    case BaseType::TInvalid: return "INVALID";
    case BaseType::TId_Struct_or_Enum: return "ID";
    }
    TODO("IMPLEMENT GET TYPE STRING");
    return nullptr;
}
/*
const char *
get_type_modifier_attr_string(TypeAttr a)
{
    switch (a)
    {
    case TypeAttr::staticConstVar: return "static const";
    case TypeAttr::constVar: return "const";
    case TypeAttr::staticMutable: return "static";
    case TypeAttr::mutableVariable: return "mutable";
    }
    TODO("implement get type modifier as string method");
    return "UNKNOWN";
} */

} // namespace rotate
