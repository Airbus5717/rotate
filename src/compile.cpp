#include "include/compile.hpp"
#include "include/file.hpp"

#include "frontend/include/lexer.hpp"

namespace rotate
{

int compile(const char *arg)
{
    file_t *file;
    Lexer *lexer;

    file = file_read(arg);
    if (!file) return EXIT_FAILURE;

    lexer = new Lexer(file);
    lexer->lex_init();

#if defined(DEBUG_MODE)
    {
        auto tkns = lexer->getTokens();
        for (token tkn : tkns)
        {
            fprintf(stderr, "%s\n", tkn_type_describe(tkn.type));
        }
    }
#endif // DEBUG_MODE

    delete lexer;
    delete file;

    return EXIT_SUCCESS;
}

} // namespace rotate
