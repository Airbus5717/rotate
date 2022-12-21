#include "type.hpp"

namespace rotate
{
const char *
get_type_string(Type t)
{
    switch (t.type)
    {
    default: break;
    }
    TODO("IMPLEMENT GET TYPE STRING");
    return nullptr;
}

const char *
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

} // namespace rotate
