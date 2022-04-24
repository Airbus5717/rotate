#include "include/compile.hpp"
#include "include/file.hpp"

#include "frontend/include/lexer.hpp"
#include "frontend/include/token.hpp"

namespace rotate
{

int compile(const char *arg) noexcept
{
    file_t *file;
    Lexer *lexer;
    u8 exit;

    file = file_read(arg);
    if (!file) return EXIT_FAILURE;

    lexer = new Lexer(file);
    exit  = lexer->lex_init();

#if defined(DEBUG_MODE)
    {
        if (exit == EXIT_SUCCESS)
        {
            auto tkns = lexer->getTokens();
            for (usize i = 0; i < tkns.size(); i++)
            {
                log_token(file->contents, tkns[i]);
            }
        }
    }
#endif // DEBUG_MODE

    // Free memory
    delete lexer;
    delete file;

    return exit;
}

} // namespace rotate
