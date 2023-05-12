#include "lexer.hpp"

namespace rotate
{

// file must not be null and lexer owns the file ptr
Lexer::Lexer(const file_t *file)
    : index(0), len(0), line(1), file_length(file ? file->length : 0), file(file),
      error(LexErr::UNKNOWN), tokens(new ArrayList<Token>())
{
    ASSERT_NULL(file, "Lexer File passed is a null pointer");
    ASSERT_NULL(tokens, "Lexer vec of tokens passed is a null pointer");

    tokens->resize(file->length / 4);
}

Lexer::~Lexer() noexcept
{
    delete tokens;
}

void
Lexer::save_log(FILE *output)
{
    for (uint i = 0; i < tokens->count(); i++)
    {
        log_token(output, tokens->at(i), file->contents);
    }
}

u8
Lexer::lex()
{
    for (;;)
    {
        switch (lex_director())
        {
            case SUCCESS: break;
            case DONE: {
                len = 0;
                for (u8 i = 0; i < EXTRA_NULL_TERMINATORS; ++i)
                    add_token(TknType::EOT);
                return SUCCESS;
            }
            case FAILURE: return report_error();
        }
    }
    return FAILURE;
}

inline void
Lexer::skip_whitespace() noexcept
{
    for (;;)
    {
        const char c = current();
        if (c == ' ') { index++; }
        else if (c == '\n')
        {
            index++;
            line++;
        }
        else { break; }
    }
}

u8
Lexer::lex_director()
{
    skip_whitespace();
    len = 0, begin_tok_line = line;
    save_state();
    const char c = current();
    // ints and floats
    if (isdigit(c)) return lex_numbers();

    // chars, and strings
    // TODO: Multiline string
    if (c == '\'') return lex_chars();
    if (c == '"') return lex_strings();

    // NOTE: Idenitifiers, keywords and builtin functions
    if (c == '_' || isalpha(c)) return lex_identifiers();
    if (c == '@') return lex_builtin_funcs();
    // Symbols
    return lex_symbols();
}

ArrayList<Token> *
Lexer::get_tokens() const
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

    // TODO: optimize searching for matching keywords
    switch (len)
    {
        case 2: {
            switch (current())
            {
                case 'a':
                    if (keyword_match("as", 2)) _type = TknType::As;
                    break;
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
                case 'v':
                    if (keyword_match("void", 4)) _type = TknType::Void;
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
        return FAILURE;
    }

    return add_token(_type);
}

u8
Lexer::lex_numbers()
{
    const char c = current();
    const char p = peek();

    // 0x or 0b switch state of lexing to specific radix
    // x -> hexadecimal(r16) | b -> binary(r2)
    if (c == '0')
    {
        switch (p)
        {
            case 'x': return lex_hex_numbers();
            case 'b': return lex_binary_numbers();
            default: break;
        }
    }

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
        return FAILURE;
    }
    index -= len;
    return add_token(reached_dot ? TknType::Integer : TknType::Float);
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
        return FAILURE;
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
        return FAILURE;
    }
    index -= len;
    return add_token(TknType::Integer);
}

u8
Lexer::lex_strings()
{
    advance_len_inc();
    for (;;)
    {
        if (current() == '\0')
        {
            restore_state_for_err();
            error = LexErr::NOT_CLOSED_STRING;
            return FAILURE;
        }

        if (current() == '"')
        {
            if (past() == '\\')
            {
                advance_len_inc();
                continue;
            }
            else
                break;
        }
        advance_len_inc();
    }
    advance_len_inc();

    if (len > (UINT_MAX / 2))
    {
        restore_state_for_err();
        log_error("A string is not allowed to be longer than (UUINT_MAX / 2)");
        error = LexErr::TOO_LONG_STRING;
        return FAILURE;
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
                return FAILURE;
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
            return FAILURE;
        }
    }

    return FAILURE;
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
                return SUCCESS;
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
                return SUCCESS;
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
        case '#': {
            // this is for comments
            while (current() != '\n' && is_not_eof())
                advance();
            advance();
            return SUCCESS;
        }
        default: {
            switch (c)
            {
                case '\0': return DONE;
                case '\t': {
                    this->error = LexErr::TABS;
                    break;
                }
                case '\r': {
                    advance();
                    return SUCCESS;
                }
                default: {
                    this->error = LexErr::LEXER_INVALID_CHAR;
                }
            }
        }
    }
    return FAILURE;
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
    // NOTE(5717): check if needed to specify the type
    return add_token(TknType::BuiltinFunc);
}

inline void
Lexer::advance()
{
    const char c = current();
    index++;
    line += (c == '\n');
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
    line += (c == '\n');
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
Lexer::keyword_match(const char *string, uint length)
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
    uint low = index, col = 0;
    while (file->contents[low] != '\n' && low > 0)
    {
        low--;
        col++;
    }
    low = low > 1 ? low + 1 : 0;

    //
    uint _length = index;
    while (file->contents[_length] != '\n' && _length + 1 < file->length)
        _length++;

    _length -= low;

    // error msg
    fprintf(stderr, " > %s%s%s:%u:%u: %serror: %s%s%s\n", BOLD, WHITE, file->name, line, col, LRED,
            LBLUE, lexer_err_msg(error), RESET);

    // line from source code
    fprintf(stderr, "  %s%u%s | %.*s\n", LYELLOW, line, RESET, _length, (file->contents + low));

    uint num_line_digits = get_digits_from_number(line);

    // arrows pointing to error location
    uint spaces = index - low + 1;

    const uint MAX_ARROW_LEN = 101;
    if (len < 101)
    {
        char arrows[MAX_ARROW_LEN];
        memset(arrows, 0, MAX_ARROW_LEN);
        memset(arrows, '^', len);
        arrows[len] = '\0';

        fprintf(stderr, "  %*c |%*c%s%s%s\n", num_line_digits, ' ', spaces, ' ', LRED, BOLD,
                arrows);
    }
    else
    {
        fprintf(stderr, "  %*c |%*c%s%s^^^---...\n", num_line_digits, ' ', spaces, ' ', LRED, BOLD);
    }
    // error lexer_err_advice
    fprintf(stderr, " > Advice: %s%s\n", RESET, lexer_err_advice(error));
    return FAILURE;
}

u8
Lexer::add_token(const TknType type)
{
    // index at the end of the token
    // NOTE(Airbus5717): emplace_back constructs the token in the vector
    tokens->append(Token(index, len, begin_tok_line, type));
    advance_len_times(); // TODO: Test optimization
    return SUCCESS;
}

} // namespace rotate
