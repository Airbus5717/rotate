#include "include/common.hpp"
#include "include/compile.hpp"

using namespace rotate;


s32 main(const s32 argc, char **const argv)
{
    if (argc > 1)
    {
        auto comp_opt           = compile_options(argc, argv);
        compilation_state state = cs_begin;

        u8 _exit = compile(&comp_opt, &state);
        if (_exit == EXIT_FAILURE)
        {
            switch (state)
            {
                case cs_begin:
                    log_error("compilation could not begin");
                    break;
                case cs_file_read:
                    log_error("Error during reading file");
                    break;
                case cs_lexer:
                    log_error("Error during lexical analysis");
                    break;
                case cs_parser:
                    log_error("Error during parsing tokens");
                    break;
                default: {
                    TODO("implement error state catch");
                }
            }
            log_error("FAILURE");
        }
        else if (_exit == EXIT_SUCCESS)
        {
            log_info("SUCCESS");
        }
    }
    else
    {
        print_version_and_exit();
    }
    return EXIT_SUCCESS;
}
