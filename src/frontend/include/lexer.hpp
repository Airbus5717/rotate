#ifndef ROTATE_LEXER
#define ROTATE_LEXER

#include "token.hpp"

namespace rotate
{

class Lexer
{
    // lexer state variables
    usize index = 0;
    file_t *file;
    std::vector<token> tokens{};

    //
    int lex();
    int report_error();
    int lex_chars();
    int lex_numbers();
    int lex_strings();
    int lex_binary_numbers();
    int lex_hex_numbers();
    int lex_symbols();
    int lex_builtin_funcs();
    int lex_identifiers();
    void add_token();

    void skip_whitespace() noexcept;

    //
    void advance();
    char peek() const;
    char past() const;
    char current() const;

  public:
    //
    Lexer(file_t *file);
    ~Lexer() = default;
    std::vector<token> getTokens();
    int lex_init();
};

} // namespace rotate

#endif // ROTATE_LEXER
