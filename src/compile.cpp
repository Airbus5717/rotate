#include "include/compile.hpp"
#include "include/file.hpp"

#include "frontend/include/parser.hpp"

namespace rotate
{
    
void compile_options::log_error_unknown_flag(const char *str)
{
    fprintf(stderr, "[%sWARN%s] : Ignored Unknown flag: `%s`\n", LYELLOW, RESET, str);
}

int compile(compile_options *options) noexcept
{
    file_t *file;
    Lexer *lexer;
    u8 exit;
    FILE *output = stdout;
    if (options->debug_info) output = fopen("output.log", "wb");
    if (!output) output = stderr;

    file = file_read(options->filename);
    if (!file) return EXIT_FAILURE;

    if (options->debug_info) file->log_head_file(output);

    lexer = new Lexer(file);
    exit  = lexer->lex();
    if (exit == EXIT_FAILURE) return EXIT_FAILURE;

    if (options->debug_info) lexer->save_log(output);

    // Free memory
    if (options->debug_info) fclose(output);
    delete lexer;
    delete file;

    return exit;
}

} // namespace rotate
