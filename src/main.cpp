#include "include/common.hpp"
#include "include/compile.hpp"

void print_version_and_exit()
{
    fprintf(stdout, "Rotate Compiler \nVersion: %s\n", VERSION);
}

using namespace rotate;

s32 main(const s32 argc, char **const argv)
{
    if (argc > 1)
    {
        auto comp_opt = compile_options(argc, argv);

        u8 exit = compile(&comp_opt);
        if (exit == EXIT_FAILURE)
        {
            log_error("FAILURE");
        }
        else if (exit == EXIT_SUCCESS)
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
