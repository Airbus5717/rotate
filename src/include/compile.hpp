#pragma once

#include "common.hpp"

namespace rotate
{

static void
print_version_and_exit()
{
    const char *out = " Rotate Compiler \n Version: %s\n"
                      " --lex   for lexical analysis\n"
                      " --log   for dumping compilation info as orgmode format in output.org\n"
                      " https://github.com/Airbus5717/rotate.git"
                      "\n";
    fprintf(stdout, out, RTVERSION);
    exit(0);
}

struct compile_options
{
    const s32 argc;
    char **const argv;
    const char *filename = NULL;
    bool debug_info      = false;
    bool debug_symbols   = false;
    bool timer           = false;
    bool lex_only        = false;
    Stage st             = Stage::unknown;

    compile_options(const s32 argc, char **argv) : argc(argc), argv(argv)
    {
        filename = argv[1];
        for (s32 i = 2; i < argc; i++)
        {
            auto string = argv[i];

            if (strcmp(string, "--log") == 0)
            {
                debug_info = true;
            }
            else if (!strcmp(string, "--version") || !strcmp(string, "-v"))
            {
                print_version_and_exit();
            }
            else if (strcmp(string, "--timer") == 0)
            {
                timer = true;
            }
            else if (strcmp(string, "--lex") == 0)
            {
                lex_only = true;
            }
            else
            {
                log_error_unknown_flag(string);
            }
        }
    }

    ~compile_options() = default;

    void log_error_unknown_flag(const char *str)
    {
        fprintf(stderr, "[%sWARN%s] : Ignored flag: `%s`\n", LYELLOW, RESET, str);
    }
};

u8 compile(compile_options *options) noexcept;

} // namespace rotate
