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

Lexer::~Lexer()
{
    for (usize i = 0; i < tokens.size(); i++)
    {
        if (get_keyword_or_type(tokens[i].type) == NULL)
        {
            free((void *)tokens[i].value);
             tokens[i].value= NULL; 
        }
    }
}

void Lexer::save_log()
{
    TODO("save_log implementation");
}

int Lexer::lex_init()
{
    col = 0, line = 1, index = 0;
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
    while (current() == ' ' || current() == '\n')
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
    TODO("lex identifiers implementation");
    return EXIT_FAILURE;
}

int Lexer::lex_numbers()
{
    const char c = current();
    const char p = peek();
    if (c == '0' && p == 'x') return lex_hex_numbers();
    if (c == '0' && p == 'b') return lex_binary_numbers();

    bool reached_dot = false;
    while ((isdigit(current()) || current() == '.'))
    {
        advance();
        len++;
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

    return add_token(reached_dot ? token_type::TknTypeFloat : token_type::TknTypeInteger);
}

int Lexer::lex_hex_numbers()
{
    TODO("lex hex numbers implementation");
    return EXIT_FAILURE;
}

int Lexer::lex_binary_numbers()
{
    TODO("lex binary numbers");
    return EXIT_FAILURE;
}

int Lexer::lex_strings()
{
    TODO("lex strings implementation");
    return EXIT_FAILURE;
}

int Lexer::lex_chars()
{
    TODO("lex chars implementation");
    return EXIT_FAILURE;
}

int Lexer::lex_symbols()
{
    const char c = current();
    const char p = peek();
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
    index += is_not_eof();
    if (peek() == '\0') error = END_OF_FILE;

    if (current() != '\n')
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

int Lexer::report_error()
{
    fprintf(stderr, "Error: %s\n", err_msgsfunc(error));
    fprintf(stderr, "Advice: %s\n", advice(error));
    return EXIT_FAILURE;
}

int Lexer::add_token(token_type type)
{
    const char *str = strndup(file->contents + (index - len), len);
    if (str == NULL)
    {
        log_error("Memory allocation failure");
        exit(1);
    }
    token tkn = token(type, line, col, index, str);
    tokens.push_back(tkn);
    return EXIT_SUCCESS;
}

int Lexer::add_token_default(token_type type)
{
    token tkn = token(type, line, col, index, get_keyword_or_type(type));
    tokens.push_back(tkn);
    return EXIT_SUCCESS;
}

} // namespace rotate
