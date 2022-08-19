#include "include/compile.hpp"
#include "include/common.hpp"
#include "include/file.hpp"

#include "frontend/include/parser.hpp"

namespace rotate
{

void compile_options::log_error_unknown_flag(const char *str)
{
    fprintf(stderr, "[%sWARN%s] : Ignored flag: `%s`\n", LYELLOW, RESET, str);
}

void log_compilation(FILE *file, Lexer &lexer, Parser &parser)
{
    time_t rawtime;
    time(&rawtime);

    const auto *tokens = lexer.getTokens();

    fprintf(file, "===META===\n");
    fprintf(file, "filename: %s\n", lexer.getFile()->name);
    fprintf(file, "file_len: %llu chars\n", lexer.getFile()->length);
    fprintf(file, "time: %s", asctime(localtime(&rawtime)));
    fprintf(file, "number of tokens: %lu\n", tokens->size());
    fprintf(file, "===FILE===\n");
    fprintf(file, "%s\n", lexer.getFile()->contents);
    fprintf(file, "===TOKENS===\n");
    for (usize i = 0; i < tokens->size(); i++)
    {
        const Token &tkn = tokens->at(i);
        fprintf(file, "[TOKEN]: idx: %u, len: %u, type: %s, val: `%.*s`\n", tkn.index, tkn.length,
                tkn_type_describe(tkn.type), tkn.length, lexer.getFile()->contents + tkn.index);
    }
    fprintf(file, "===TODO: Parser Abstract Syntax Tree===\n");
    parser.save_log(file);
    fprintf(file, "====TODO: TYPECHECKER ====\n");
    log_info("Logging complete");
}

u8 compile(compile_options *options, compilation_state *state) noexcept
{
    ASSERT_NULL(state, "state is null");
    file_t *file;
    // Parser *parser;
    u8 exit = 0;

    // Read file
    *state = cs_file_read;
    file   = file_read(options->filename);
    if (!file) return EXIT_FAILURE;

    // Lexical analysis
    *state      = cs_lexer;
    Lexer lexer = Lexer(file);
    exit        = lexer.lex();
    if (exit == EXIT_FAILURE) return EXIT_FAILURE;

    // parse lexed tokens to Abstract Syntax tree
    *state        = cs_parser;
    Parser parser = Parser(lexer);
    exit          = parser.parse();

    // log compiliation
    if (options->debug_info)
    {
        FILE *output = fopen("output.log", "wb");
        if (output)
        {
            log_compilation(output, lexer, parser);
            fclose(output);
        }
    }

    return exit;
}

} // namespace rotate
