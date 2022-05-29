#ifndef ROTATE_LEXER
#define ROTATE_LEXER

#include "token.hpp"

namespace rotate
{

class Lexer
{
    // lexer state variables
    u32 index, len, line;
    file_t *file; // not owned by the lexer
    u32 file_length;
    bool is_done;
    error_type error;
    std::vector<token> *tokens;

    //
    u8 lex_director();
    u8 lex_chars();
    u8 lex_numbers();
    u8 lex_strings();
    u8 lex_binary_numbers();
    u8 lex_hex_numbers();
    u8 lex_symbols();
    u8 lex_builtin_funcs();
    u8 lex_identifiers();

    //
    u8 report_error();
    u8 reverse_len_for_error();

    //
    u8 add_token(token_type type);

    //
    void advance();
    void advance_len_inc();
    void advance_len_times();
    void advance_len_times_plus_one();
    void advance_len_times_minus_one();
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
    std::vector<token> *getTokens();
    file_t *getFile();
    u32 get_num_of_lines();
    u8 lex();
    void save_log(FILE *output);
};

void log_token(FILE *output, const rotate::token tkn, const char *str);

} // namespace rotate

#endif // ROTATE_LEXER
