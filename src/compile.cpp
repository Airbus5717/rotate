#include "include/compile.hpp"
#include "include/common.hpp"
#include "include/file.hpp"
#include "include/log.hpp"


namespace rotate
{

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
    if (lexer.get_tokens()->count() < 2u) log_error("file is empty");
    if (exit == FAILURE) return FAILURE;
    // parse lexed tokens to Abstract Syntax tree

    /*
     *
     * PARSING
     *
     * */
    // TODO

    // log compiliation
    if (options->debug_info)
    {
        // LOGS ONLY DURING SUCCESS OF THE PREVIOUS STAGES
        options->st = Stage::logger;
        if (FILE *output = fopen("output.org", "wb"))
        {
            log_compilation(output, &file, &lexer);
            fclose(output);
        }
        else
        {
            log_error("Log failed");
        }
    }

    return exit;
}

} // namespace rotate
