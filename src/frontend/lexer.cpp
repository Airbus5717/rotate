#include "include/lexer.hpp"

namespace rotate
{

#define EXIT_DONE 3

Lexer::Lexer(file_t *file)
{
    this->file        = file;
    this->is_done     = false;
    this->error       = error_type::UNKNOWN;
    this->file_length = file->length;
}

Lexer::~Lexer() = default;

void Lexer::save_log()
{
    TODO("save_log implementation");
}

int Lexer::lex_init()
{
    index = 0;
    while (!is_done)
    {
        skip_whitespace();
        switch (lex())
        {
            case EXIT_SUCCESS:
                break;
            case EXIT_DONE:
                add_token_default(token_type::TknTypeEOT);
                return EXIT_SUCCESS;
            case EXIT_FAILURE:
                return report_error();
        }
        advance();
    }
    return EXIT_SUCCESS;
}

void Lexer::skip_whitespace() noexcept
{
    while (is_space_rotate(current()))
    {
        advance();
    }
}

int Lexer::lex()
{
    len = 0;
    skip_whitespace();
    const char c = current();

    //
    if (isdigit(c)) return lex_numbers();

    if (c == '\'') return lex_chars();
    if (c == '"') return lex_strings();

    if (c == '_' || isalpha(c)) return lex_identifiers();
    if (c == '@') return lex_builtin_funcs();

    return lex_symbols();
}

std::vector<token> Lexer::getTokens()
{
    return tokens;
}

int Lexer::lex_identifiers()
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
                return add_token_identifiers(TknTypeAs);
            else if (keyword_match("fn", 2))
                return add_token_identifiers(TknTypeFunction);
            else if (keyword_match("if", 2))
                return add_token_identifiers(TknTypeIf);
            else if (keyword_match("or", 2))
                return add_token_identifiers(TknTypeOr);
            break;
        }
        case 3: {
            if (keyword_match("for", 3))
                return add_token_identifiers(TknTypeFor);
            else if (keyword_match("let", 3))
                return add_token_identifiers(TknTypeLet);
            else if (keyword_match("pub", 3))
                return add_token_identifiers(TknTypePublic);
            else if (keyword_match("mut", 3))
                return add_token_identifiers(TknTypeMutable);
            else if (keyword_match("str", 3))
                return add_token_identifiers(TknTypeStringKeyword);
            else if (keyword_match("int", 3))
                return add_token_identifiers(TknTypeIntKeyword);
            else if (keyword_match("ref", 3))
                return add_token_identifiers(TknTypeRef);
            else if (keyword_match("and", 3))
                return add_token_identifiers(TknTypeAnd);
            else if (keyword_match("nil", 3))
                return add_token_identifiers(TknTypeNil);
            else if (keyword_match("var", 3))
                return add_token_identifiers(TknTypeVar);
            break;
        }
        case 4: {
            if (keyword_match("else", 4))
                return add_token_identifiers(TknTypeElse);
            else if (keyword_match("true", 4))
                return add_token_identifiers(TknTypeTrue);
            else if (keyword_match("enum", 4))
                return add_token_identifiers(TknTypeEnum);
            else if (keyword_match("char", 4))
                return add_token_identifiers(TknTypeCharKeyword);
            else if (keyword_match("bool", 4))
                return add_token_identifiers(TknTypeBoolKeyword);
            else if (keyword_match("void", 4))
                return add_token_identifiers(TknTypeVoid);
            break;
        }
        case 5: {
            if (keyword_match("while", 5))
                return add_token_identifiers(TknTypeWhile);
            else if (keyword_match("false", 5))
                return add_token_identifiers(TknTypeFalse);
            else if (keyword_match("match", 5))
                return add_token_identifiers(TknTypeMatch);
            else if (keyword_match("break", 5))
                return add_token_identifiers(TknTypeBreak);
            else if (keyword_match("float", 5))
                return add_token_identifiers(TknTypeFloatKeyword);
            break;
        }
        case 6: {
            if (keyword_match("return", 6))
                return add_token_identifiers(TknTypeReturn);
            else if (keyword_match("import", 6))
                return add_token_identifiers(TknTypeImport);
            else if (keyword_match("struct", 6))
                return add_token_identifiers(TknTypeStruct);
            break;
        }
        case 7: {
            if (keyword_match("include", 7)) return add_token_identifiers(TknTypeInclude);
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

    return add_token_identifiers(TknTypeIdentifier);
}

int Lexer::lex_numbers()
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

    return add_token_identifiers(reached_dot ? TknTypeFloat : TknTypeInteger);
}

int Lexer::lex_hex_numbers()
{
    advance();
    advance();
    while (isxdigit(current()) || isdigit(current()))
    {
        advance_len_inc();
    }

    if (len > 64)
    {
        log_error("hex number digits length is above 64");
    }

    return add_token_identifiers(token_type::TknTypeHexInteger);
}

int Lexer::lex_binary_numbers()
{
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
    return add_token_identifiers(token_type::TknTypeBinaryInteger);
}

int Lexer::lex_strings()
{
    advance_len_inc();
    while (current() != '"' && past() != '\\')
    {
        if (current() == '\0' || current() == '\n')
        {
            error = error_type::NOT_CLOSED_STRING;
            return EXIT_FAILURE;
        }
        advance_len_inc();
    }
    advance_len_inc();
    if (len > UINT_MAX)
    {
        log_error("Too long string");
    }
    return add_token_identifiers(TknTypeString);
}

int Lexer::lex_chars()
{
    // TODO("Lex chars implementation");
    advance_len_inc();
    if (current() != '\\' && peek() == '\'')
    {
        advance_len_inc();
        advance_len_inc();
        return add_token_identifiers(TknTypeChar);
    }
    else if (current() == '\\')
    {
        advance_len_inc();
        switch (current())
        {
            case 'n':
            case 't':
            case 'r':
            case 'v':
                advance_len_inc();
                break;
            default:
                error = error_type::NOT_VALID_ESCAPE_CHAR;
                return EXIT_FAILURE;
        }
        if (current() == '\'')
        {
            advance_len_inc();
            return add_token_identifiers(TknTypeEscapedChar);
        }
        else
        {
            return EXIT_FAILURE;
        }
    }

    return EXIT_FAILURE;
}

int Lexer::lex_symbols()
{
    const char c = current();
    const char p = peek();
    len++;
    switch (c)
    {
            // clang-format off
        case '{': return add_token_default(token_type::TknTypeLeftCurly);
        case '}': return add_token_default(token_type::TknTypeRightCurly);
        case '(': return add_token_default(token_type::TknTypeLeftParen);
        case ')': return add_token_default(token_type::TknTypeRightParen);
        case '[': return add_token_default(token_type::TknTypeLeftSQRBrackets);
        case ']': return add_token_default(token_type::TknTypeRightSQRBrackets);
        case ';': return add_token_default(token_type::TknTypeSemiColon);
        case '>': return add_token_default(token_type::TknTypeGreater);
        case '<': return add_token_default(token_type::TknTypeLess);
        case '.': return add_token_default(token_type::TknTypeDot);
        case ',': return add_token_default(token_type::TknTypeComma);
        case ':': return add_token_default(token_type::TknTypeColon);
        // clang-format on
        case '=': {
            if (p == '=')
            {
                len++;
                return add_token_default(token_type::TknTypeEqualEqual);
            }
            else
                return add_token_default(token_type::TknTypeEqual);
        }
        case '+': {
            if (p == '=')
            {
                len++;
                return add_token_default(token_type::TknTypeAddEqual);
            }
            else
                return add_token_default(token_type::TknTypePLUS);
        }
        case '-': {
            if (p == '=')
            {
                len++;
                return add_token_default(token_type::TknTypeSubEqual);
            }
            else
                return add_token_default(token_type::TknTypeMINUS);
        }
        case '*': {
            if (p == '=')
            {
                len++;
                return add_token_default(token_type::TknTypeMultEqual);
            }
            else
                return add_token_default(token_type::TknTypeStar);
        }
        case '/': {
            if (p == '=')
            {
                len++;
                return add_token_default(token_type::TknTypeDivEqual);
            }
            else if (p == '/')
            {
                while (is_not_eof() && current() != '\n')
                {
                    advance();
                }
                break;
            }
            else if (p == '*')
            {
                bool end_comment = false;
                while (!is_not_eof() && current() != '\0' && !end_comment)
                {
                    // end_comment will not break
                    // because advancing is needed
                    if ((past() == '*' && current() == '/')) end_comment = true;
                    advance();
                }
                break;
            }
            else
                return add_token_default(token_type::TknTypeDIV);
            break;
        }
        case '!': {
            if (p == '=')
            {
                len++;
                return add_token_default(token_type::TknTypeNotEqual);
            }
            return add_token_default(token_type::TknTypeNot);
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

int Lexer::lex_builtin_funcs()
{
    TODO("lex builtin funcs implementation");
    return EXIT_FAILURE;
}

void Lexer::advance()
{
    index++;
}

void Lexer::advance_len_inc()
{
    index++;
    len++;
}

char Lexer::peek() const
{
    return (index + 1 < file_length) ? file->contents[index + 1] : 0;
}

char Lexer::current() const
{
    return (is_not_eof()) ? file->contents[index] : 0;
}

char Lexer::past() const
{
    return file->contents[index - 1];
}

bool Lexer::is_not_eof() const
{
    return index < file_length;
}

bool Lexer::keyword_match(const char *string, usize length)
{
    return strncmp(file->contents + (index - length), string, length) == 0;
}

int Lexer::report_error()
{
    fprintf(stderr, "Error: %s\n", err_msgsfunc(error));
    fprintf(stderr, "Advice: %s\n", advice(error));
    return EXIT_FAILURE;
}

int Lexer::add_token_identifiers(token_type type)
{
    tokens.push_back(token(type, index - len, len));
    index--;
    return EXIT_SUCCESS;
}

int Lexer::add_token_default(token_type type)
{
    tokens.push_back(token(type, index, len));
    return EXIT_SUCCESS;
}

} // namespace rotate
