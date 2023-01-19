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
    ImportId, // alias
    // functions
    FnId,
    OpenCurly,
    CloseCurly,
    IncorrectType,
    // global variables
    GlobalMutableVar,
    GlobalVarType,
    GlobalLongVarExpr,
    GlobalVarSemiColon,
    GlobalVarColon,
    GlobalVarEql,
    // global structs
    // global enums
    // types
    ParseType,
    NUM_OF_PARSER_ERRORS, // NOTE: do not use it for parsing errors
                          // this is only to get the count of the enum
};

struct ParserErr
{
    PrsErr err;
    const char *msg;
    const char *advice;
    ParserErr(PrsErr err, const char *msg, const char *advice) : err(err), msg(msg), advice(advice)
    {
    }
};

static const ParserErr parser_errors[] = {
    ParserErr(PrsErr::Unknown, "Unknown Error Message", "Unknown Error Advice"),
    ParserErr(PrsErr::SemicolonExpect, "Statement requires a SemiColon';'",
              "Add a Semicolon at the end of the statement"),
    ParserErr(PrsErr::GlobalScopeNotAllowed, "Found a non global statement",
              "This Token is not allowed in global scope"),
    ParserErr(PrsErr::ImportStringExpect, "Import statement requires a string path",
              "Add a path as a string literal in the import statement"),
    ParserErr(PrsErr::OpenParents, "Missing open parentheses (", "Add an open parentheses '('"),
    ParserErr(PrsErr::CloseParents, "Missing close parentheses )", "Add an close parentheses ')'"),
    ParserErr(PrsErr::OpenCurly, "Expected open curly brackets {",
              "Add an open curly brackets '{'"),
    ParserErr(PrsErr::CloseCurly, "Expected close curly brackets }",
              "Add an open curly brackets '}'"),
    ParserErr(PrsErr::ImportId, "Import aliased string requires an Id as alias",
              "Add an identifier after 'as' keyword"),
    ParserErr(PrsErr::GlobalVarColon, "Expected a colon after Global Variable identifier",
              "Add a colon ':' for the global variable Identifier"),
    ParserErr(PrsErr::GlobalVarSemiColon, "Expected a semicolon",
              "Add a Semicolon ';' after the global variable statement"),
    ParserErr(PrsErr::GlobalVarEql, "Expected an Equal",
              "Add an Equal sign '=' for assigning to the global variable"),
};

} // namespace rotate
