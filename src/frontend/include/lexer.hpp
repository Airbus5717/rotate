#ifndef ROTATE_LEXER
#define ROTATE_LEXER

#include "token.hpp"

namespace rotate
{

class Lexer
{
    // lexer state variables
    usize index = 0, col = 0, line = 0, len = 0;
    file_t *file;
    usize file_length;
    bool is_done;
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

    //
    int add_token(token_type type);
    int add_token_default(token_type type);

    //
    void advance();
    void advance(usize i);
    char peek() const;
    char past() const;
    char current() const;
    bool is_eof() const;
    void skip_whitespace() noexcept;

    error_type error;

  public:
    //
    Lexer(file_t *file);
    ~Lexer() = default;
    std::vector<token> getTokens();
    int lex_init();
};
} // namespace rotate

#endif // ROTATE_LEXER
