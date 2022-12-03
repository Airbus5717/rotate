#pragma once

#include "token.hpp"

namespace rotate
{
enum class PrsErr : u16
{
    // USES INDEX SYSTEM
    // DO NOT PUT DIFFERENT INDEXS IN
    Unknown = 0,
    // global stmts
    GlobalScopeNotAllowed,
    SemicolonExpect,
    // global import
    OpenParents,
    CloseParents,
    ImportStringExpect,

    // global variables
    GlobalMutableVar,
    GlobalVarType,
    GlobalLongVarExpr,
    GlobalVarSemiColon,
    // global structs
    // global enums
};

struct ParserErr
{
    PrsErr err;
    const char *msg;
    const char *advice;
};

const static ParserErr parser_errors[] = {
    {
        .err    = PrsErr::Unknown,
        .msg    = "Unknown Error Message",
        .advice = "Unknown Error Advice",
    },
    {
        .err    = PrsErr::SemicolonExpect,
        .msg    = "Statement requires a SemiColon",
        .advice = "Add a SemiColon at the end of the statement ",
    },
    {
        .err    = PrsErr::GlobalScopeNotAllowed,
        .msg    = "Found a non Global Statement",
        .advice = "This Token is not Allowed in global scope",
    },
    {
        .err    = PrsErr::ImportStringExpect,
        .msg    = "Import statement requires a path",
        .advice = "Add a path as string in the import statement",
    },
    {
        .err    = PrsErr::OpenParents,
        .msg    = "Missing open parentheses",
        .advice = "Add an open parentheses '('",
    },
    {
        .err    = PrsErr::CloseParents,
        .msg    = "Missing close parentheses",
        .advice = "Add a close parenthese ')'",
    },
};

} // namespace rotate
