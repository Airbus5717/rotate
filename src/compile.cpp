#include "include/compile.hpp"
#include "include/common.hpp"
#include "include/defines.hpp"
#include "include/file.hpp"

#include "fe/parser.hpp"

namespace rotate
{

void
log_compilation(FILE *file, file_t *code_file, Lexer *lexer)
{
    time_t rawtime;
    time(&rawtime);
    assert(code_file && lexer);

    const auto *tokens = lexer->getTokens();
    if (tokens->size() > 0x10000000)
    {
        log_warn("Too large file to show log");
        return;
    }
    log_warn("Logging will slow down compiliation and use alot of memory");
    fprintf(file, "#+TITLE: COMPILATION LOG\n");
    fprintf(file, "#+OPTIONS: toc:nil num:nil\n");
    fprintf(file, "#+AUTHOR: Rotate compiler\n");
    fprintf(file, "#+DATE: %s\n", asctime(localtime(&rawtime)));
    fprintf(file, "** Meta\n");
    fprintf(file, "- filename: =%s=\n", code_file->name);
    fprintf(file, "- file length(chars): %u chars\n", code_file->length);
    fprintf(file, "- time: %s", asctime(localtime(&rawtime)));
    fprintf(file, "- number of tokens: %lu\n\n", tokens->size());
    fprintf(file, "** FILE\n");
    fprintf(file, "#+begin_src cpp \n%s\n#+end_src\n\n", code_file->contents);

    fprintf(file, "** TOKENS\n");
    fprintf(file, "#+begin_src\n");
    for (usize i = 0; i < tokens->size(); i++)
    {
        const Token &tkn = tokens->at(i);
        fprintf(file, "[TOKEN]: idx: %u, line: %u, len: %u, type: %s, val: `%.*s`\n", tkn.index,
                tkn.line, tkn.length, tkn_type_describe(tkn.type), tkn.length,
                code_file->contents + tkn.index);
    }
    fprintf(file, "#+end_src\n");
    fprintf(file, "\n** TODO: Parser Abstract Syntax Tree\n");
    // if (nullptr) parser->save_log(file);
    fprintf(file, "\n** TODO: TYPECHECKER\n");
    log_info("Logging complete");
}

u8
handle_err(u8 exit, const char *err_string)
{
    if (exit == 1)
    {
        log_error(err_string);
    }
    return exit;
}

u8
compile(compile_options *options) noexcept
{
    // Parser *parser;
    u8 exit = 0;

    // Read file
    options->st = Stage::file;
    file_t file = file_read(options->filename);
    ASSERT_RET_FAIL(file.valid_code == valid::success, "file read error");

    /*
     *
     * LEXICAL ANALYSIS
     *
     * */
    options->st = Stage::lexer;
    Lexer lexer = Lexer(&file);
    exit        = lexer.lex();
    if (lexer.getTokens()->size() < 2) log_error("file is empty");
    if (exit == EXIT_FAILURE) return EXIT_FAILURE;
    // parse lexed tokens to Abstract Syntax tree
    /*
    **
    ** PARSING
    **
    */
    if (!options->lex_only)
    {
        options->st   = Stage::parser;
        Parser parser = Parser(&file, &lexer);
        exit          = parser.parse_lexer();
        ASSERT_RET_FAIL(exit == EXIT_FAILURE, "Parser error");
    }

    // log compiliation
    if (options->debug_info)
    {
        options->st = Stage::logger;
        if (FILE *output = fopen("output.org", "wb"))
        {
            log_compilation(output, &file, &lexer);
            fclose(output);
        }
    }

    return exit;
}

} // namespace rotate
