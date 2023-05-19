#pragma once

#include "token.hpp"

namespace rotate
{

class Lexer
{
    // lexer state variables
    uint index, len, line, begin_tok_line, file_length;
    const file_t *file; // not owned by the lexer
    LexErr error    = LexErr::UNKNOWN;
    uint save_index = 0, save_line = 0;
    Array<Token> *tokens;

    //
    u8 lex_director();
    u8 lex_chars();
    u8 lex_numbers();
    u8 lex_strings();
    u8 lex_multiline_strings();
    u8 lex_binary_numbers();
    u8 lex_hex_numbers();
    u8 lex_symbols();
    u8 lex_builtin_funcs();
    u8 lex_identifiers();

    //
    u8 report_error();
    void save_state();
    void restore_state_for_err();

    //
    u8 add_token(const TknType);

    //
    void advance();
    void advance_len_inc();
    void advance_len_times();
    char peek() const;
    char past() const;
    char current() const;
    bool is_not_eof() const;
    void skip_whitespace() noexcept;
    bool keyword_match(cstr, uint);

    public:
    //
    Lexer(const file_t *);
    ~Lexer() noexcept;
    Array<Token> *get_tokens() const;
    uint get_num_of_lines();
    u8 lex();
    void save_log(FILE *);
}; // class Lexer

void log_token(FILE *, const Token, cstr);

} // namespace rotate
