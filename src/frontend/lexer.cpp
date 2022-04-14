#include "include/lexer.hpp"

namespace rotate
{

Lexer::Lexer(file_t *file)
{
    this->file = file;
}

int Lexer::lex_init()
{
    for (usize i = 0; i < file->contents.size(); i++)
    {
        if (lex() == EXIT_FAILURE) return report_error();
    }
    return EXIT_SUCCESS;
}

void Lexer::skip_whitespace() noexcept
{
    while (is_space_rotate(current()))
    {
        advance();
    }
}

int Lexer::lex()
{
    skip_whitespace();
    const char c = current();
    if (isdigit(c))
    {
        if (c == '0' and peek() == 'x')
            return lex_hex_numbers();
        else if (c == '0' and peek() == 'b')
            return lex_binary_numbers();
        else
            return lex_numbers();
    }
    if (c == '\'') return lex_chars();
    if (c == '"') return lex_strings();

    if (c == '_' || isalpha(c)) return lex_identifiers();
    if (c == '@') return lex_builtin_funcs();

    return lex_symbols();
}

std::vector<token> Lexer::getTokens()
{
    return tokens;
}

int Lexer::lex_identifiers()
{
    TODO("");
    return EXIT_FAILURE;
}

int Lexer::lex_numbers()
{
    TODO("");
    return EXIT_FAILURE;
}

int Lexer::lex_hex_numbers()
{
    TODO("");
    return EXIT_FAILURE;
}

int Lexer::lex_binary_numbers()
{
    TODO("");
    return EXIT_FAILURE;
}

int Lexer::lex_strings()
{
    TODO("");
    return EXIT_FAILURE;
}

int Lexer::lex_chars()
{
    TODO("");
    return EXIT_FAILURE;
}

int Lexer::lex_symbols()
{
    TODO("lexer lex symbols");
    switch (current())
    {
        case '.':
            break;
        case '!':
            break;
        default:
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int Lexer::lex_builtin_funcs()
{
    TODO("");
    return EXIT_FAILURE;
}

void Lexer::advance()
{
    index += (index < file->contents.size());
}

char Lexer::peek() const
{
    return file->contents.at(index + 1);
}

char Lexer::current() const
{
    return file->contents.at(index);
}

char Lexer::past() const
{
    return file->contents.at(index - 1);
}

int Lexer::report_error()
{
    TODO("");
    return EXIT_FAILURE;
}

} // namespace rotate