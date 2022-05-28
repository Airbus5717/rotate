#include "include/compile.hpp"
#include "include/file.hpp"

#include "frontend/include/parser.hpp"

namespace rotate
{

void compile_options::log_error_unknown_flag(const char *str)
{
    fprintf(stderr, "[%sWARN%s] : Ignored Unknown flag: `%s`\n", LYELLOW, RESET, str);
}

int compile(compile_options *options, compilation_state *state) noexcept
{
    ASSERT_NULL(state, "state is null");
    file_t *file;
    Lexer *lexer;
    // Parser *parser;
    u8 exit      = 0;
    FILE *output = stdout;
    if (options->debug_info) output = fopen("output.log", "wb");
    if (!output) output = stderr;

    // Read file
    *state = cs_file_read;
    file   = file_read(options->filename);
    if (!file) return EXIT_FAILURE;

    // log file read
    if (options->debug_info) file->log_head_file(output);

    // Lexical analysis
    *state = cs_lexer;
    lexer  = new Lexer(file);
    exit   = lexer->lex();
    if (exit == EXIT_FAILURE && !lexer) return EXIT_FAILURE;

    // log lexical analysis
    if (options->debug_info) lexer->save_log(output);

    // parse lexed tokens to Abstract Syntax tree
    // *state = cs_parser;
    // parser = new Parser(lexer);
    // if (exit == EXIT_FAILURE && !parser) return EXIT_FAILURE;
    // exit = parser->parse();

    // Free memory
    if (options->debug_info) fclose(output);
    // delete parser;
    delete lexer;
    delete file;

    //
    return exit;
}

} // namespace rotate
