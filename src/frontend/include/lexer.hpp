#ifndef ROTATE_LEXER
#define ROTATE_LEXER

#include "token.hpp"

namespace rotate
{

class Lexer
{
    // lexer state variables
    u32 index = 0, len = 0;
    file_t *file;
    u32 file_length;
    bool is_done;
    std::vector<token> tokens{};
    error_type error;

    //
    u8 lex();
    u8 report_error();
    u8 lex_chars();
    u8 lex_numbers();
    u8 lex_strings();
    u8 lex_binary_numbers();
    u8 lex_hex_numbers();
    u8 lex_symbols();
    u8 lex_builtin_funcs();
    u8 lex_identifiers();

    //
    u8 add_token_identifiers(token_type type);
    u8 add_token_default(token_type type);

    //
    void advance();
    void advance_len_inc();
    char peek() const;
    char past() const;
    char current() const;
    bool is_not_eof() const;
    void skip_whitespace() noexcept;
    bool keyword_match(const char *string, u32 length);

  public:
    //
    Lexer(file_t *file);
    ~Lexer();
    std::vector<token> getTokens();
    u8 lex_init();
    void save_log();
};

void log_token(const char *str, const rotate::token &tkn);

} // namespace rotate

#endif // ROTATE_LEXER
