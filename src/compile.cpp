#include "include/compile.hpp"
#include "include/file.hpp"

#include "frontend/include/lexer.hpp"
#include "frontend/include/parser.hpp"
#include "frontend/include/token.hpp"

namespace rotate
{

int compile(compile_options *options) noexcept
{
    file_t *file;
    Lexer *lexer;
    u8 exit;

    file = file_read(options->filename);
    if (!file) return EXIT_FAILURE;

    lexer = new Lexer(file);
    exit  = lexer->lex();
    if (exit == EXIT_FAILURE) return EXIT_FAILURE;

    if (options->debug_info)
    {
        lexer->save_log(stderr);
    }

    // Free memory
    delete lexer;
    delete file;

    return exit;
}

} // namespace rotate
