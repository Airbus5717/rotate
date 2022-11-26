#include "lexer.hpp"
#include "token.hpp"

namespace rotate
{

// file must not be null and lexer owns the file ptr
Lexer::Lexer(file_t *file)
    : index(0), len(0), line(1), file_length(file ? file->length : 0), file(file),
      error(LexErr::UNKNOWN), tokens(new std::vector<Token>())
{
    ASSERT_NULL(file, "Lexer File passed is a null pointer");
    ASSERT_NULL(tokens, "Lexer vec of tokens passed is a null pointer");
    tokens->reserve(file->length / 2);
}

Lexer::~Lexer() noexcept
{
    delete tokens;
}

void
Lexer::save_log(FILE *output)
{
    for (usize i = 0; i < tokens->size(); i++)
    {
        log_token(output, tokens->at(i), file->contents);
    }
}

u8
Lexer::lex()
{
    do
    {
        switch (lex_director())
        {
        case EXIT_SUCCESS: break;
        case EXIT_DONE:
            len = 0;
            add_token(TknType::EOT);
            tokens->shrink_to_fit();
            return EXIT_SUCCESS;
        case EXIT_FAILURE: return report_error();
        }
    } while (true);
    return EXIT_FAILURE;
}

inline void
Lexer::skip_whitespace() noexcept
{
    while (true)
    {
        const char c = current();
        if (c == '\n')
        {
            index++;
            line++;
        }
        else if (c == ' ')
        {
            index++;
        }
        else
            break;
    }
}

u8
Lexer::lex_director()
{
    skip_whitespace();
    len = 0, begin_tok_line = line;
    save_state();
    const char c = current();

    if (isdigit(c)) return lex_numbers();

    //
    if (c == '\'') return lex_chars();
    if (c == '"') return lex_strings();
    if (c == '`') return lex_multiline_strings();

    //
    if (c == '_' || isalpha(c)) return lex_identifiers();
    if (c == '@') return lex_builtin_funcs();

    return lex_symbols();
}

std::vector<Token> *
Lexer::getTokens()
{
    return tokens;
}

u8
Lexer::lex_identifiers()
{
    advance_len_inc();
    while (isalnum(current()) || current() == '_')
    {
        advance_len_inc();
    }
    index -= len;
    TknType _type = TknType::Identifier;

    // TODO: optimize matching keywords
    switch (len)
    {
    case 2: {
        switch (current())
        {
        case 'f':
            if (keyword_match("fn", 2)) _type = TknType::Function;
            break;
        case 'i':
            if (keyword_match("if", 2))
                _type = TknType::If;
            else if (keyword_match("in", 2))
                _type = TknType::In;
            break;
        case 'o':
            if (keyword_match("or", 2)) _type = TknType::Or;
            break;
        default: break;
        }

        break;
    }
    case 3: {
        switch (current())
        {
        case 'f':
            if (keyword_match("for", 3)) _type = TknType::For;
            break;
        case 'p':
            if (keyword_match("pub", 3)) _type = TknType::Public;
            break;
        case 'i':
            if (keyword_match("int", 3)) _type = TknType::IntKeyword;
            break;
        case 'r':
            if (keyword_match("ref", 3)) _type = TknType::Ref;
            break;
        case 'a':
            if (keyword_match("and", 3)) _type = TknType::And;
            break;
        case 'n':
            if (keyword_match("nil", 3)) _type = TknType::Nil;
            break;
        }
        break;
    }
    case 4: {
        switch (current())
        {
        case 'e': {
            if (keyword_match("else", 4))
                _type = TknType::Else;
            else if (keyword_match("enum", 4))
                _type = TknType::Enum;
            break;
        }
        case 't':
            if (keyword_match("true", 4)) _type = TknType::True;
            break;
        case 'c':
            if (keyword_match("char", 4)) _type = TknType::CharKeyword;
            break;
        case 'b':
            if (keyword_match("bool", 4)) _type = TknType::BoolKeyword;
            break;
        case 'u':
            if (keyword_match("uint", 4)) _type = TknType::UintKeyword;
            break;
        }
        break;
    }
    case 5: {
        switch (current())
        {
        case 'w':
            if (keyword_match("while", 5)) _type = TknType::While;
            break;
        case 'f': {
            if (keyword_match("false", 5))
                _type = TknType::False;
            else if (keyword_match("float", 5))
                _type = TknType::FloatKeyword;
            break;
        }
        case 'b':
            if (keyword_match("break", 5)) _type = TknType::Break;
            break;
        }
        break;
    }
    case 6: {
        switch (current())
        {
        case 'r':
            if (keyword_match("return", 6)) _type = TknType::Return;
            break;
        case 'i':
            if (keyword_match("import", 6)) _type = TknType::Import;
            break;
        case 'd':
            if (keyword_match("delete", 6)) _type = TknType::Delete;
            break;
        case 's': {
            if (keyword_match("struct", 6))
                _type = TknType::Struct;
            else if (keyword_match("switch", 6))
                _type = TknType::Switch;
            break;
        }
        }
        break;
    }
    default: break;
    }

    if (len > 100)
    {
        // log_error("identifier length is more than 128 chars");
        error = LexErr::TOO_LONG_IDENTIFIER;
        restore_state_for_err();
        return EXIT_FAILURE;
    }

    return add_token(_type);
}

u8
Lexer::lex_numbers()
{
    const char c = current();
    const char p = peek();
    if (c == '0' && p == 'x') return lex_hex_numbers();
    if (c == '0' && p == 'b') return lex_binary_numbers();

    bool reached_dot = false;
    while (isdigit(current()) || current() == '.')
    {
        advance_len_inc();
        if (current() == '.')
        {
            if (reached_dot) break;
            reached_dot = true;
        }
    }

    if (len > 100)
    {
        log_error("number digits length is above 100");
        restore_state_for_err();
        return EXIT_FAILURE;
    }
    index -= len;
    return add_token(reached_dot ? TknType::Float : TknType::Integer);
}

u8
Lexer::lex_hex_numbers()
{
    // skip '0x'
    advance_len_inc();
    advance_len_inc();
    while (isxdigit(current()))
    {
        advance_len_inc();
    }

    if (len > 0x20)
    {
        log_error("hex number digits length is above 32");
        restore_state_for_err();
        return EXIT_FAILURE;
    }
    index -= len;
    return add_token(TknType::Integer);
}

u8
Lexer::lex_binary_numbers()
{
    // skip '0b'
    advance_len_inc();
    advance_len_inc();
    while (current() == '0' || current() == '1')
    {
        advance_len_inc();
    }

    if (len > 0x80)
    {
        log_error("binary number digits length is above 128");
        restore_state_for_err();
        return EXIT_FAILURE;
    }
    index -= len;
    return add_token(TknType::Integer);
}

u8
Lexer::lex_strings()
{
    advance_len_inc();
    while (current() != '"' && past() != '\\')
    {
        if (current() == '\0')
        {
            restore_state_for_err();
            error = LexErr::NOT_CLOSED_STRING;
            return EXIT_FAILURE;
        }
        if (current() == '\n')
        {
            restore_state_for_err();
            error = LexErr::NOT_CLOSED_STRING;
            return EXIT_FAILURE;
        }
        advance_len_inc();
    }
    advance_len_inc();

    if (len > UINT16_MAX)
    {
        log_error("Too long string");
        return EXIT_FAILURE;
    }
    index -= len;

    return add_token(TknType::String);
}

u8
Lexer::lex_multiline_strings()
{
    advance_len_inc();
    while (current() != '`' && past() != '\\')
    {
        if (current() == '\0')
        {
            restore_state_for_err();
            error = LexErr::NOT_CLOSED_STRING;
            return EXIT_FAILURE;
        }
        advance_len_inc();
    }
    advance_len_inc();

    if (len > UINT32_MAX)
    {
        log_error("Too long multiline string");
        return EXIT_FAILURE;
    }
    index -= len;

    return add_token(TknType::String);
}

u8
Lexer::lex_chars()
{
    advance_len_inc();
    if (current() != '\\' && peek() == '\'')
    {
        advance_len_inc();
        advance_len_inc();
        index -= len;
        return add_token(TknType::Char);
    }
    else if (current() == '\\')
    {
        advance_len_inc();
        switch (current())
        {
        case 'n':
        case 't':
        case 'r':
        case 'b':
        case 'f':
        case '\\':
        case '\'': advance_len_inc(); break;
        default:
            error = LexErr::NOT_VALID_ESCAPE_CHAR;
            restore_state_for_err();
            return EXIT_FAILURE;
        }
        if (current() == '\'')
        {
            advance_len_inc();
            index -= len;
            return add_token(TknType::Char);
        }
        else
        {
            error = LexErr::LEXER_INVALID_CHAR;
            restore_state_for_err();
            return EXIT_FAILURE;
        }
    }

    return EXIT_FAILURE;
}

u8
Lexer::lex_symbols()
{
    const char c = current();
    const char p = peek();
    len          = 1;
    switch (c)
    {
    case '{': return add_token(TknType::OpenCurly);
    case '}': return add_token(TknType::CloseCurly);
    case '(': return add_token(TknType::OpenParen);
    case ')': return add_token(TknType::CloseParen);
    case '[': return add_token(TknType::OpenSQRBrackets);
    case ']': return add_token(TknType::CloseSQRBrackets);
    case ';': return add_token(TknType::SemiColon);
    case ',': return add_token(TknType::Comma);
    // TODO(5717) bug below needs to check an eql during peeking

    // more than one length char
    case '.': {
        if (p == '.')
        {
            len++;
            return add_token(TknType::To);
        }
        return add_token(TknType::Dot);
    }
    case ':': {
        if (p == ':')
        {
            len++;
            return add_token(TknType::ColonColon);
        }
        return add_token(TknType::Colon);
    }
    case '>': {
        if (p == '=')
        {
            len++;
            return add_token(TknType::GreaterEql);
        }
        return add_token(TknType::Greater);
    }
    case '<': {
        if (p == '=')
        {
            len++;
            return add_token(TknType::LessEql);
        }
        return add_token(TknType::Less);
    }
    case '=': {
        if (p == '=')
        {
            len++;
            return add_token(TknType::EqualEqual);
        }
        return add_token(TknType::Equal);
    }
    case '+': {
        if (p == '=')
        {
            len++;
            return add_token(TknType::AddEqual);
        }
        return add_token(TknType::PLUS);
    }
    case '-': {
        if (p == '=')
        {
            len++;
            return add_token(TknType::SubEqual);
        }
        return add_token(TknType::MINUS);
    }
    case '*': {
        if (p == '=')
        {
            len++;
            return add_token(TknType::MultEqual);
        }
        return add_token(TknType::Star);
    }
    case '/': {
        if (p == '=')
        {
            len++;
            return add_token(TknType::DivEqual);
        }
        else if (p == '/')
        {
            //
            while (is_not_eof() && current() != '\n')
                advance();
            return EXIT_SUCCESS;
        }
        else if (p == '*')
        {
            advance();
            advance();
            // TODO: Allow nested comments
            bool end_comment = false;
            while (is_not_eof() && !end_comment)
            {
                if (current() == '*' && peek() == '/')
                {
                    advance();
                    end_comment = true;
                }
                advance();
            }
            return EXIT_SUCCESS;
        }
        return add_token(TknType::DIV);
    }
    case '!': {
        if (p == '=')
        {
            len++;
            return add_token(TknType::NotEqual);
        }
        return add_token(TknType::Not);
    }
    default: {
        switch (c)
        {
        case '\0': return EXIT_DONE;
        case '\t': this->error = LexErr::TABS; break;
        case '\r': this->error = LexErr::WINDOWS_CRAP; break;
        case '#': {
            while (current() != '\n')
                advance();
            break;
        }
        default: this->error = LexErr::LEXER_INVALID_CHAR;
        }
    }
    }
    return EXIT_FAILURE;
}

u8
Lexer::lex_builtin_funcs()
{
    advance(); // skip '@'

    // NOTE(5717): ONLY ALPHABET CHARS ARE ALLOWED
    while (isalpha(current()))
    {
        advance_len_inc();
    }
    index -= len;

    return add_token(TknType::BuiltinFunc);
}

inline void
Lexer::advance()
{
    const char c = current();
    index++;
    line += (c == '\n'); // if (c == '\n') line++;
}

inline void
Lexer::advance_len_times()
{
    index += len;
}

inline void
Lexer::advance_len_inc()
{
    const char c = current();
    index++;
    len++;
    line += (c == '\n'); // if (c == '\n') line++;
}

inline char
Lexer::peek() const
{
    return file->contents[index + 1];
}

inline char
Lexer::current() const
{
    return file->contents[index];
}

inline char
Lexer::past() const
{
    return file->contents[index - 1];
}

inline bool
Lexer::is_not_eof() const
{
    return index < file_length;
}

inline bool
Lexer::keyword_match(const char *string, Uint length)
{
    return strncmp(file->contents + index, string, length) == 0;
}

void
Lexer::save_state()
{
    save_index = index;
    save_line  = line;
}

void
Lexer::restore_state_for_err()
{
    index = save_index;
    line  = save_line;
}

u8
Lexer::report_error()
{
    //
    Uint low = index, col = 0;
    while (file->contents[low] != '\n' && low > 0)
    {
        low--;
        col++;
    }
    low = low > 1 ? low + 1 : 0;

    //
    Uint _length = index;
    while (file->contents[_length] != '\n' && _length + 1 < file->length)
        _length++;

    _length -= low;

    // error msg
    fprintf(stderr, "> %s%s%s:%u:%u: %serror: %s%s%s\n", BOLD, WHITE, file->name, line, col, LRED,
            LBLUE, lexer_err_msg(error), RESET);

    // line from source code
    fprintf(stderr, " %s%u%s | %.*s\n", LYELLOW, line, RESET, _length, (file->contents + low));

    Uint num_line_digits = get_digits_from_number(line);

    // arrows pointing to error location
    Uint spaces = index - low + 1;
    if (len < 101)
    {
        char *arrows = (char *)alloca(len + 1);
        memset(arrows, '^', len);
        arrows[len] = '\0';

        fprintf(stderr, " %*c |%*c%s%s%s\n", num_line_digits, ' ', spaces, ' ', LRED, BOLD, arrows);
    }
    else
    {
        fprintf(stderr, " %*c |%*c%s%s^^^---...\n", num_line_digits, ' ', spaces, ' ', LRED, BOLD);
    }
    // error lexer_err_advice
    fprintf(stderr, "> Advice: %s%s\n", RESET, lexer_err_advice(error));
    return EXIT_FAILURE;
}

u8
Lexer::add_token(const TknType type)
{
    // index at the end of the token
    // NOTE(Airbus5717): emplace_back constructs the token in the vector
    tokens->emplace_back(type, index, len, begin_tok_line);

    for (Uint i = 0; i < len; i++)
        advance();

    return EXIT_SUCCESS;
}

} // namespace rotate
