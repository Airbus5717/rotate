#pragma once

#include "defines.hpp"

namespace rotate
{

enum class Stage : u8
{
    unknown = 0,
    file,
    lexer,
    parser,
    tchecker,
    logger,
};
const char *main_err(Stage);
/*
 *  Utilites
 */

//
void log_stage(const char *);
void log_error(const char *);
void exit_error(const char *);
void log_debug(const char *);
void log_info(const char *);
void log_warn(const char *);

//
UINT get_digits_from_number(UINT);
// bitwise operations
u8 bit_set(const u8 field, const u8 n);
u8 bit_clear(const u8 field, const u8 n);
u8 bit_is_set(const u8 field, const u8 n);

} // namespace rotate
