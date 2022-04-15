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
    auto tkns = lexer->getTokens();
    for (usize i = 0; i < tkns.size(); i++)
    {
        std::cout << tkns[i].value << " " << tkns[i].type << std::endl;
    }
    delete lexer;
    delete file;
    return EXIT_SUCCESS;
}

} // namespace rotate
