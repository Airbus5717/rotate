#include "include/compile.hpp"
#include "include/common.hpp"
#include "include/defines.hpp"
#include "include/file.hpp"

#include "frontend/lexer.hpp"

namespace rotate
{

void log_compilation(FILE *file, Lexer &lexer)
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
    // if (nullptr) parser->save_log(file);
    fprintf(file, "\n** TODO: TYPECHECKER\n");
    log_info("Logging complete");
}

u8 handle_err(u8 exit, const char *err_string)
{
    if (exit == 1)
    {
        log_error(err_string);
    }
    return exit;
}

u8 compile(compile_options *options) noexcept
{
    // Parser *parser;
    u8 exit = 0;

    // Read file
    file_t file = file_read(options->filename);
    ASSERT_RET_FAIL(file.valid_code == valid::success, "file read error");

    // Lexical analysis
    Lexer lexer = Lexer(&file);
    exit        = lexer.lex();
    ASSERT_RET_FAIL(exit != EXIT_DONE, "Lexer error");
    if (options->lex_only)
    {
        log_compilation(fopen("output.log", "wb"), lexer);
        return exit;
    }
    // parse lexed tokens to Abstract Syntax tree

    // log compiliation
    if (options->debug_info)
    {
        FILE *output = fopen("output.org", "wb");
        if (output)
        {
            log_compilation(output, lexer);
            fclose(output);
        }
    }

    return exit;
}

} // namespace rotate
