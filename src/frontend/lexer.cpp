#include "include/lexer.hpp"
#include "include/token.hpp"

namespace rotate
{

// file must not be null and lexer owns the ile ptr
Lexer::Lexer(file_t *file)
    : index(0), len(0), line(1), file(file), file_length(file ? file->length : 0), is_done(false),
      error(UNKNOWN), tokens(new std::vector<token>())
{
    ASSERT_NULL(file, "Lexer File passed is a null pointer");
    ASSERT_NULL(tokens, "Lexer vec of tokens passed is a null pointer");
}

Lexer::~Lexer() noexcept
{
    delete tokens;
}

void Lexer::save_log(FILE *output)
{
    for (usize i = 0; i < tokens->size(); i++)
    {
        log_token(output, tokens->at(i), file->contents);
    }
}

u8 Lexer::lex()
{
    do
    {
        switch (lex_director())
        {
            case EXIT_SUCCESS:
                break;
            case EXIT_DONE:
                len = 0;
                add_token(token_type::EOT);
                return EXIT_SUCCESS;
            case EXIT_FAILURE:
                return report_error();
        }
    } while (true);
    return EXIT_SUCCESS;
}

inline void Lexer::skip_whitespace() noexcept
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

u8 Lexer::lex_director()
{
    skip_whitespace();
    len = 0;

    const char c = current();

    if (isdigit(c)) return lex_numbers();

    //
    if (c == '\'') return lex_chars();
    if (c == '"') return lex_strings();

    //
    if (c == '_' || isalpha(c)) return lex_identifiers();
    if (c == '@') return lex_builtin_funcs();

    return lex_symbols();
}

std::vector<token> *Lexer::getTokens()
{
    return tokens;
}

file_t *Lexer::getFile()
{
    return file;
}

u8 Lexer::lex_identifiers()
{
    advance_len_inc();
    while (isalnum(current()) || current() == '_')
    {
        advance_len_inc();
    }
    index -= len;
    token_type _type = Identifier;
    switch (len)
    {
        case 2: {
            if (keyword_match("fn", 2))
                _type = Function;
            else if (keyword_match("if", 2))
                _type = If;
            else if (keyword_match("or", 2))
                _type = Or;
            //
            else if (keyword_match("u8", 2))
                _type = INT_U8;
            else if (keyword_match("s8", 2))
                _type = INT_S8;
            break;
        }
        case 3: {
            if (keyword_match("for", 3))
                _type = For;
            else if (keyword_match("let", 3))
                _type = Let;
            else if (keyword_match("pub", 3))
                _type = Public;
            else if (keyword_match("str", 3))
                _type = StringKeyword;
            else if (keyword_match("int", 3))
                _type = IntKeyword;
            else if (keyword_match("ref", 3))
                _type = Ref;
            else if (keyword_match("and", 3))
                _type = And;
            else if (keyword_match("nil", 3))
                _type = Nil;
            else if (keyword_match("var", 3))
                _type = Var;
            //
            else if (keyword_match("u16", 3))
                _type = INT_U16;
            else if (keyword_match("u32", 3))
                _type = INT_U32;
            else if (keyword_match("u64", 3))
                _type = INT_U64;
            else if (keyword_match("s16", 3))
                _type = INT_S16;
            else if (keyword_match("s32", 3))
                _type = INT_S32;
            else if (keyword_match("s64", 3))
                _type = INT_S64;
            break;
        }
        case 4: {
            if (keyword_match("else", 4))
                _type = Else;
            else if (keyword_match("true", 4))
                _type = True;
            else if (keyword_match("enum", 4))
                _type = Enum;
            else if (keyword_match("char", 4))
                _type = CharKeyword;
            else if (keyword_match("bool", 4))
                _type = BoolKeyword;
            else if (keyword_match("void", 4))
                _type = Void;
            break;
        }
        case 5: {
            if (keyword_match("while", 5))
                _type = While;
            else if (keyword_match("false", 5))
                _type = False;
            else if (keyword_match("match", 5))
                _type = Match;
            else if (keyword_match("break", 5))
                _type = Break;
            else if (keyword_match("const", 5))
                _type = Const;
            else if (keyword_match("float", 5))
                _type = FloatKeyword;
            break;
        }
        case 6: {
            if (keyword_match("return", 6))
                _type = Return;
            else if (keyword_match("import", 6))
                _type = Import;
            else if (keyword_match("struct", 6))
                _type = Struct;
            break;
        }
        case 7: {
            if (keyword_match("include", 7)) _type = Include;
            break;
        }
        default:
            break;
    }

    if (len > 128)
    {
        log_error("identifier length is more than 128 chars");
        return EXIT_FAILURE;
    }

    return add_token(_type);
}

u8 Lexer::lex_numbers()
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
        return EXIT_FAILURE;
    }
    return add_token(reached_dot ? Float : Integer);
}

u8 Lexer::lex_hex_numbers()
{
    // skip '0x'
    advance();
    advance();
    while (isxdigit(current()))
    {
        advance_len_inc();
    }

    if (len > 32)
    {
        log_error("hex number digits length is above 32");
    }
    index -= len;
    return add_token(token_type::HexInteger);
}

u8 Lexer::lex_binary_numbers()
{
    // skip '0b'
    advance();
    advance();
    while (current() == '0' || current() == '1')
    {
        advance_len_inc();
    }

    if (len > 128)
    {
        log_error("binary number digits length is above 128");
    }
    index -= len;
    return add_token(token_type::BinaryInteger);
}

u8 Lexer::lex_strings()
{
    advance_len_inc();
    while (current() != '"' && past() != '\\')
    {
        if (current() == '\0')
        {
            error = error_type::NOT_CLOSED_STRING;
            return EXIT_FAILURE;
        }
        advance_len_inc();
    }

    advance_len_inc();

    if (len > UINT16_MAX)
    {
        log_error("Too long string");
    }
    index -= len;
    return add_token(String);
}

u8 Lexer::lex_chars()
{
    advance_len_inc();
    if (current() != '\\' && peek() == '\'')
    {
        advance_len_inc();
        advance_len_inc();
        index -= len;
        return add_token(Char);
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
            case '\'':
                advance_len_inc();
                break;
            default:
                error = error_type::NOT_VALID_ESCAPE_CHAR;
                return EXIT_FAILURE;
        }
        if (current() == '\'')
        {
            advance_len_inc();
            index -= len;
            return add_token(EscapedChar);
        }
        else
        {
            error = error_type::LEXER_INVALID_CHAR;
            return reverse_len_for_error();
        }
    }

    return EXIT_FAILURE;
}

u8 Lexer::lex_symbols()
{
    const char c = current();
    const char p = peek();
    len++;
    switch (c)
    {
        // clang-format off
        case '{': return add_token(token_type::OpenCurly);
        case '}': return add_token(token_type::CloseCurly);
        case '(': return add_token(token_type::OpenParen);
        case ')': return add_token(token_type::CloseParen);
        case '[': return add_token(token_type::OpenSQRBrackets);
        case ']': return add_token(token_type::CloseSQRBrackets);
        case ';': return add_token(token_type::SemiColon);
        // TODO(5717) bug below needs to check an eql during peeking
        case '>': return add_token(token_type::Greater);
        case '<': return add_token(token_type::Less);
        case '.': return add_token(token_type::Dot);
        case ',': return add_token(token_type::Comma);
        case ':': return add_token(token_type::Colon);
        // clang-format on
        case '=': {
            if (p == '=')
            {
                len++;
                return add_token(token_type::EqualEqual);
            }
            else
                return add_token(token_type::Equal);
        }
        case '+': {
            if (p == '=')
            {
                len++;
                return add_token(token_type::AddEqual);
            }
            else
                return add_token(token_type::PLUS);
        }
        case '-': {
            if (p == '=')
            {
                len++;
                return add_token(token_type::SubEqual);
            }
            else
                return add_token(token_type::MINUS);
        }
        case '*': {
            if (p == '=')
            {
                len++;
                return add_token(token_type::MultEqual);
            }
            else
                return add_token(token_type::Star);
        }
        case '/': {
            if (p == '=')
            {
                len++;
                return add_token(token_type::DivEqual);
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
                // TODO: Allow nested comments
                bool end_comment = false;
                while (is_not_eof() && !end_comment)
                {
                    if ((past() != '/' && current() == '*' && peek() == '/'))
                    {
                        advance();
                        end_comment = true;
                    }
                    advance();
                }
                return EXIT_SUCCESS;
            }
            else
            {
                return add_token(token_type::DIV);
            }
            break;
        }
        case '!': {
            if (p == '=')
            {
                len++;
                return add_token(token_type::NotEqual);
            }
            return add_token(token_type::Not);
        }
        default: {
            switch (c)
            {
                case '\0':
                    is_done = true;
                    return EXIT_DONE;
                case '\t':
                    this->error = TABS;
                    break;
                case '\r':
                    this->error = WINDOWS_CRAP;
                    break;
                default:
                    this->error = LEXER_INVALID_CHAR;
            }
        }
    }
    return EXIT_FAILURE;
}

u8 Lexer::lex_builtin_funcs()
{
    advance(); // skip '@'
    while (isalpha(current()))
    {
        advance_len_inc();
    }
    index -= len;

    switch (len)
    {
        case 3: {
            if (keyword_match("col", 3))
            {

                return add_token(BuiltinFunc);
            }
            break;
        }
        case 4: {
            if (keyword_match("line", 4))
            {

                return add_token(BuiltinFunc);
            }
            else if (keyword_match("file", 4))
            {

                return add_token(BuiltinFunc);
            }
            break;
        }
        case 7: {
            if (keyword_match("println", 7))
            {

                return add_token(BuiltinFunc);
            }
            break;
        }
        default: {
            break;
        }
    }

    error = error_type::LEXER_INVALID_BUILTN_FN;
    return reverse_len_for_error();
}

inline void Lexer::advance()
{
    const char c = current();
    index++;
    if (c == '\n') line++;
}

inline void Lexer::advance_len_times()
{
    index += len;
}

inline void Lexer::advance_len_inc()
{
    const char c = current();
    index++;
    len++;
    if (c == '\n') line++;
}

inline char Lexer::peek() const
{
    return file->contents[index + 1];
}

inline char Lexer::current() const
{
    return file->contents[index];
}

inline char Lexer::past() const
{
    return file->contents[index - 1];
}

inline bool Lexer::is_not_eof() const
{
    return index < file_length;
}

inline bool Lexer::keyword_match(const char *string, u32 length)
{
    return strncmp(file->contents + index, string, length) == 0;
}

u8 Lexer::reverse_len_for_error()
{
    index -= len;
    return EXIT_FAILURE;
}

u8 Lexer::report_error()
{
    //
    u32 low = index, col = 0;
    while (file->contents[low] != '\n' && low > 0)
    {
        low--;
        col++;
    }
    low++;

    //
    u32 _length = index;
    while (file->contents[_length] != '\n' && _length + 1 < file->length)
        _length++;

    _length -= low;

    // error msg
    fprintf(stderr, "> %s%s%s:%u:%u: %serror: %s%s%s\n", BOLD, WHITE, file->name, line, col, LRED,
            LBLUE, err_msgsfunc(error), RESET);

    // line from source code
    fprintf(stderr, " %s%u%s | %.*s\n", LYELLOW, line, RESET, _length, (file->contents + low));

    u32 num_line_digits = get_digits_from_number(line);

    // arrows pointing to error location
    u32 spaces = index - low + 1;
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
    // error advice
    fprintf(stderr, "> Advice: %s%s\n", RESET, advice(error));
    return EXIT_FAILURE;
}

u8 Lexer::add_token(token_type type)
{
    // index at the end of the token
    tokens->push_back(token(type, index, len));

    for (u32 i = 0; i < len; i++)
        advance();

    return EXIT_SUCCESS;
}

} // namespace rotate
