#pragma once

#include "../fe/lexer.hpp"
#include "../fe/parser.hpp"
#include "common.hpp"

namespace rotate
{
void log_compilation(FILE *, file_t *, Lexer *, Parser *);
};
