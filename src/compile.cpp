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
        std::vector<token> tkns = lexer->getTokens();
        for (usize i = 0; i < tkns.size(); i++)
        {
            fprintf(stderr, "%s\n", tkn_type_describe(tkns[i].type));
        }
    }
#endif // DEBUG_MODE

    delete lexer;
    delete file;

    return EXIT_SUCCESS;
}

} // namespace rotate
