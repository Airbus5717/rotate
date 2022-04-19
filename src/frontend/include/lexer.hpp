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
    error_type error;

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
    int add_token_identifiers(token_type type);
    int add_token_default(token_type type);

    //
    void advance();
    void advance(usize i);
    void advance_len_inc();
    char peek() const;
    char past() const;
    char current() const;
    bool is_not_eof() const;
    void skip_whitespace() noexcept;
    bool keyword_match(const char *string, usize length);

  public:
    //
    Lexer(file_t *file);
    ~Lexer();
    std::vector<token> getTokens();
    int lex_init();
    void save_log();
};

void log_token(const char *str, const rotate::token &tkn);

} // namespace rotate

#endif // ROTATE_LEXER
