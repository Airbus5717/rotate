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
void log_error(const char *);
void exit_error(const char *);
void log_debug(const char *);
void log_info(const char *);
void log_warn(const char *);

//
Uint get_digits_from_number(Uint);

} // namespace rotate
