#pragma once

#include "ArrayList.hpp"
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

/*
 *  Utilites
 */

//
void log_stage(cstr);
void log_error(cstr);
void exit_error(cstr);
void log_debug(cstr);
void log_info(cstr);
void log_warn(cstr);

char *strndup(cstr, const usize);
//
uint get_digits_from_number(uint);
// bitwise operations
u8 bit_set(const u8 field, const u8 n);
u8 bit_clear(const u8 field, const u8 n);
u8 bit_is_set(const u8 field, const u8 n);

} // namespace rotate
