#include "include/lexer.hpp"

namespace rotate
{

#define EXIT_DONE 3

// file must not be null
Lexer::Lexer(file_t *file)
{
    ASSERT_NULL(file, "Lexer File init failure");
    this->file        = file;
    this->is_done     = false;
    this->error       = error_type::UNKNOWN;
    this->file_length = file->length;
    this->tokens      = new std::vector<token>();
    ASSERT_NULL(tokens, "Tokens vector initialization failure");
}

Lexer::~Lexer()
{
    delete tokens;
}

void Lexer::save_log(FILE *output)
{
    ASSERT_NULL(tokens, "Tokens var is null in save_log");
    ASSERT(output, "output is NULL");
    if (tokens->size() > 0)
    {
        auto size = tokens->size();
        if (size > 128)
        {
            log_info("Too many Tokens to display");
        }
        else
        {
            for (usize i = 0; i < size; i++)
            {
                log_token(output, file->contents, tokens->at(i));
            }
        }
    }
}

u8 Lexer::lex()
{
    index = 0;
    do
    {
        skip_whitespace();
        switch (lex_director())
        {
            case EXIT_SUCCESS:
                break;
            case EXIT_DONE:
                add_token_fixed_length(token_type::TknTypeEOT);
                return EXIT_SUCCESS;
            case EXIT_FAILURE:
                return report_error();
        }
        advance();
    } while (true);
    return EXIT_SUCCESS;
}

void Lexer::skip_whitespace() noexcept
{
    while (is_space_rotate(current()))
    {
        advance();
    }
}

u8 Lexer::lex_director()
{
    len = 0;
    skip_whitespace();
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

u8 Lexer::lex_identifiers()
{
    advance_len_inc();
    while (isalnum(current()) || current() == '_')
    {
        advance_len_inc();
    }

    switch (len)
    {
        case 2: {
            if (keyword_match("as", 2))
                return add_token_variant_length(TknTypeAs);
            else if (keyword_match("fn", 2))
                return add_token_variant_length(TknTypeFunction);
            else if (keyword_match("if", 2))
                return add_token_variant_length(TknTypeIf);
            else if (keyword_match("or", 2))
                return add_token_variant_length(TknTypeOr);
            break;
        }
        case 3: {
            if (keyword_match("for", 3))
                return add_token_variant_length(TknTypeFor);
            else if (keyword_match("let", 3))
                return add_token_variant_length(TknTypeLet);
            else if (keyword_match("pub", 3))
                return add_token_variant_length(TknTypePublic);
            else if (keyword_match("str", 3))
                return add_token_variant_length(TknTypeStringKeyword);
            else if (keyword_match("int", 3))
                return add_token_variant_length(TknTypeIntKeyword);
            else if (keyword_match("ref", 3))
                return add_token_variant_length(TknTypeRef);
            else if (keyword_match("and", 3))
                return add_token_variant_length(TknTypeAnd);
            else if (keyword_match("nil", 3))
                return add_token_variant_length(TknTypeNil);
            else if (keyword_match("var", 3))
                return add_token_variant_length(TknTypeVar);
            break;
        }
        case 4: {
            if (keyword_match("else", 4))
                return add_token_variant_length(TknTypeElse);
            else if (keyword_match("true", 4))
                return add_token_variant_length(TknTypeTrue);
            else if (keyword_match("enum", 4))
                return add_token_variant_length(TknTypeEnum);
            else if (keyword_match("char", 4))
                return add_token_variant_length(TknTypeCharKeyword);
            else if (keyword_match("bool", 4))
                return add_token_variant_length(TknTypeBoolKeyword);
            else if (keyword_match("void", 4))
                return add_token_variant_length(TknTypeVoid);
            break;
        }
        case 5: {
            if (keyword_match("while", 5))
                return add_token_variant_length(TknTypeWhile);
            else if (keyword_match("false", 5))
                return add_token_variant_length(TknTypeFalse);
            else if (keyword_match("match", 5))
                return add_token_variant_length(TknTypeMatch);
            else if (keyword_match("break", 5))
                return add_token_variant_length(TknTypeBreak);
            else if (keyword_match("const", 5))
                return add_token_variant_length(TknTypeConst);
            else if (keyword_match("float", 5))
                return add_token_variant_length(TknTypeFloatKeyword);
            break;
        }
        case 6: {
            if (keyword_match("return", 6))
                return add_token_variant_length(TknTypeReturn);
            else if (keyword_match("import", 6))
                return add_token_variant_length(TknTypeImport);
            else if (keyword_match("struct", 6))
                return add_token_variant_length(TknTypeStruct);
            break;
        }
        case 7: {
            if (keyword_match("include", 7)) return add_token_variant_length(TknTypeInclude);
            break;
        }
        default:
            break;
    }

    if (len > 100)
    {
        log_error("identifier length is more than 100 chars");
        return EXIT_FAILURE;
    }

    return add_token_variant_length(TknTypeIdentifier);
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

    return add_token_variant_length(reached_dot ? TknTypeFloat : TknTypeInteger);
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

    if (len > 64)
    {
        log_error("hex number digits length is above 64");
    }

    return add_token_variant_length(token_type::TknTypeHexInteger);
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
    return add_token_variant_length(token_type::TknTypeBinaryInteger);
}

u8 Lexer::lex_strings()
{
    advance_len_inc();
    while (current() != '"' && past() != '\\')
    {
        if (current() == '\0' || current() == '\n')
        {
            error = error_type::NOT_CLOSED_STRING;
            index -= len;
            return EXIT_FAILURE;
        }
        advance_len_inc();
    }
    advance_len_inc();
    if (len > UINT32_MAX)
    {
        log_error("Too long string");
    }
    return add_token_variant_length(TknTypeString);
}

u8 Lexer::lex_chars()
{
    advance_len_inc();
    if (current() != '\\' && peek() == '\'')
    {
        advance_len_inc();
        advance_len_inc();
        return add_token_variant_length(TknTypeChar);
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
            return add_token_variant_length(TknTypeEscapedChar);
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
        case '{': return add_token_fixed_length(token_type::TknTypeLeftCurly);
        case '}': return add_token_fixed_length(token_type::TknTypeRightCurly);
        case '(': return add_token_fixed_length(token_type::TknTypeLeftParen);
        case ')': return add_token_fixed_length(token_type::TknTypeRightParen);
        case '[': return add_token_fixed_length(token_type::TknTypeLeftSQRBrackets);
        case ']': return add_token_fixed_length(token_type::TknTypeRightSQRBrackets);
        case ';': return add_token_fixed_length(token_type::TknTypeSemiColon);
        case '>': return add_token_fixed_length(token_type::TknTypeGreater);
        case '<': return add_token_fixed_length(token_type::TknTypeLess);
        case '.': return add_token_fixed_length(token_type::TknTypeDot);
        case ',': return add_token_fixed_length(token_type::TknTypeComma);
        case ':': return add_token_fixed_length(token_type::TknTypeColon);
        // clang-format on
        case '=': {
            if (p == '=')
            {
                len++;
                return add_token_fixed_length(token_type::TknTypeEqualEqual);
            }
            else
                return add_token_fixed_length(token_type::TknTypeEqual);
        }
        case '+': {
            if (p == '=')
            {
                len++;
                return add_token_fixed_length(token_type::TknTypeAddEqual);
            }
            else
                return add_token_fixed_length(token_type::TknTypePLUS);
        }
        case '-': {
            if (p == '=')
            {
                len++;
                return add_token_fixed_length(token_type::TknTypeSubEqual);
            }
            else
                return add_token_fixed_length(token_type::TknTypeMINUS);
        }
        case '*': {
            if (p == '=')
            {
                len++;
                return add_token_fixed_length(token_type::TknTypeMultEqual);
            }
            else
                return add_token_fixed_length(token_type::TknTypeStar);
        }
        case '/': {
            if (p == '=')
            {
                len++;
                return add_token_fixed_length(token_type::TknTypeDivEqual);
            }
            else if (p == '/')
            {
                while (is_not_eof() && current() != '\n')
                {
                    advance();
                }
            }
            else if (p == '*')
            {
                advance();
                // TODO: Allow nested comments
                u16 nest = 1;
                do
                {
                    if (current() == '/' && peek() == '*') nest++;
                    if ((past() == '*' && current() == '/')) nest--;

                    advance();
                    if (current() == '\0')
                    {
                        error = error_type::NOT_CLOSED_COMMENT;
                        return EXIT_FAILURE;
                    }

                } while (is_not_eof() && nest > 0);
            }
            else
            {
                return add_token_fixed_length(token_type::TknTypeDIV);
            }
            break;
        }
        case '!': {
            if (p == '=')
            {
                len++;
                return add_token_fixed_length(token_type::TknTypeNotEqual);
            }
            return add_token_fixed_length(token_type::TknTypeNot);
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
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

u8 Lexer::lex_builtin_funcs()
{
    advance();
    while (isalpha(current()) || current() == '_')
    {
        advance_len_inc();
    }

    switch (len)
    {
        case 3: {
            if (keyword_match("col", 3)) return add_token_variant_length(TknTypeBuiltinFunc);
            break;
        }
        case 4: {
            if (keyword_match("line", 4))
                return add_token_variant_length(TknTypeBuiltinFunc);
            else if (keyword_match("file", 4))
                return add_token_variant_length(TknTypeBuiltinFunc);
            break;
        }
        case 7: {
            if (keyword_match("println", 7)) return add_token_variant_length(TknTypeBuiltinFunc);
            break;
        }
        default: {
        }
    }
    error = error_type::LEXER_INVALID_BUILTN_FN;
    return reverse_len_for_error();
}

void Lexer::advance()
{
    const char c = current();
    index++;
    if (c != '\n')
    {
        col++;
    }
    else
    {
        col = 1;
        line++;
    }
}

void Lexer::advance_len_inc()
{
    const char c = current();
    index++;
    len++;
    if (c != '\n')
    {
        col++;
    }
    else
    {
        col = 1;
        line++;
    }
}

char Lexer::peek() const
{
    return (index + 1 < file_length) ? file->contents[index + 1] : '\0';
}

char Lexer::current() const
{
    return (is_not_eof()) ? file->contents[index] : '\0';
}

char Lexer::past() const
{
    return file->contents[index - 1];
}

bool Lexer::is_not_eof() const
{
    return index < file_length;
}

bool Lexer::keyword_match(const char *string, u32 length)
{
    return strncmp(file->contents + (index - length), string, length) == 0;
}

u8 Lexer::reverse_len_for_error()
{
    index -= len;
    return EXIT_FAILURE;
}

u8 Lexer::report_error()
{
    //
    u32 low = index;
    while (file->contents[low] != '\n' && low > 0)
        low--;

    low++;

    //
    u32 _length = index;
    while (file->contents[_length] != '\n' && _length + 1 < file->length)
        _length++;

    _length -= low;

    // error msg
    fprintf(stderr, " %s%s%s:%u:%u: %serror: %s %s%s\n", BOLD, WHITE, file->name, line, col, LRED,
            LBLUE, err_msgsfunc(error), RESET);

    // line from source code
    fprintf(stderr, " %s%u%s | %.*s\n", LYELLOW, line, RESET, _length, (file->contents + low));

    u32 num_lin_digits = get_digits_from_number(line);

    // arrows pointing to error location
    u32 spaces = index - low + 1;
    if (len < 101)
    {
        char *arrows = (char *)malloc(len + 1);
        if (!arrows) exit_error("couldn't display error message");
        memset(arrows, '^', len);
        arrows[len] = '\0';

        fprintf(stderr, " %*c |%*c%s%s%s\n", num_lin_digits, ' ', spaces, ' ', LRED, BOLD, arrows);
        free(arrows);
    }
    else
    {
        fprintf(stderr, " %*c |%*c%s%s^^^---...\n", num_lin_digits, ' ', spaces, ' ', LRED, BOLD);
    }
    // error advice
    fprintf(stderr, " Advice: %s%s\n", RESET, advice(error));
    return EXIT_FAILURE;
}

u8 Lexer::add_token_variant_length(token_type type)
{
    tokens->push_back(token(type, index - len, len, line, col - len));
    index--;
    return EXIT_SUCCESS;
}

u8 Lexer::add_token_fixed_length(token_type type)
{
    tokens->push_back(token(type, index, len, line, col));
    return EXIT_SUCCESS;
}

} // namespace rotate
