#pragma once

#include "defines.hpp"

namespace rotate
{

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
