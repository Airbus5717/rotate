#include "include/compile.hpp"
#include "include/common.hpp"
#include "include/file.hpp"

#include "frontend/include/parser.hpp"

namespace rotate
{

void log_compilation(FILE *file, Lexer &lexer, Parser *parser)
{
    time_t rawtime;
    time(&rawtime);

    const auto *tokens = lexer.getTokens();
    if (tokens->size() > 1000)
    {
        log_warn("Too large file to show log (above 1k tokens)");
        return;
    }
    log_warn("Logging will slow down compiliation and use alot of memory");
    fprintf(file, "#+TITLE: COMPILATION LOG\n");
    fprintf(file, "#+OPTIONS: toc:nil num:nil\n");
    fprintf(file, "#+DATE: %s\n", asctime(localtime(&rawtime)));
    fprintf(file, "** Meta\n");
    fprintf(file, "- filename: %s\n", lexer.getFile()->name);
    fprintf(file, "- file_len: %llu chars\n", lexer.getFile()->length);
    fprintf(file, "- time: %s", asctime(localtime(&rawtime)));
    fprintf(file, "- number of tokens: %lu\n\n", tokens->size());
    fprintf(file, "** FILE\n");
    fprintf(file, "#+begin_src cpp \n%s\n#+end_src\n\n", lexer.getFile()->contents);

    fprintf(file, "** TOKENS\n");
    fprintf(file, "#+begin_src\n");
    for (usize i = 0; i < tokens->size(); i++)
    {
        const Token &tkn = tokens->at(i);
        fprintf(file, "[TOKEN]: idx: %u, line: %u, len: %u, type: %s, val: `%.*s`\n", tkn.index,
                tkn.line, tkn.length, tkn_type_describe(tkn.type), tkn.length,
                lexer.getFile()->contents + tkn.index);
    }
    fprintf(file, "#+end_src\n");
    fprintf(file, "\n** TODO: Parser Abstract Syntax Tree\n");
    if (parser) parser->save_log(file);
    fprintf(file, "\n** TODO: TYPECHECKER\n");
    log_info("Logging complete");
}

u8 compile(compile_options *options, compilation_state *state) noexcept
{
    ASSERT_NULL(state, "state is null");
    // Parser *parser;
    u8 exit = 0;

    // Read file
    *state      = cs_file_read;
    file_t file = file_read(options->filename);
    if (file.valid_code == valid::failure) return EXIT_FAILURE;

    // Lexical analysis
    *state      = cs_lexer;
    Lexer lexer = Lexer(&file);
    exit        = lexer.lex();
    if (exit == EXIT_FAILURE) return EXIT_FAILURE;
    if (options->lex_only)
    {
        log_compilation(fopen("output.log", "wb"), lexer, nullptr);
        return exit;
    }
    // parse lexed tokens to Abstract Syntax tree
    *state        = cs_parser;
    Parser parser = Parser(&lexer); // parser does not own lexer pointer
    exit          = parser.parse();

    // log compiliation
    if (options->debug_info)
    {
        FILE *output = fopen("output.org", "wb");
        if (output)
        {
            log_compilation(output, lexer, &parser);
            fclose(output);
        }
    }

    return exit;
}

} // namespace rotate
