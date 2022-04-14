#include "include/compile.hpp"
#include "include/file.hpp"

#include "frontend/include/lexer.hpp"

namespace rotate
{

int compile(const char *arg)
{

    file_t *file = file_read(arg);
    Lexer *lexer = new Lexer(file);
    lexer->lex_init();

    delete lexer;
    delete file;
    return EXIT_FAILURE;
}

} // namespace rotate
