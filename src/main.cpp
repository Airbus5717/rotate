#include "include/common.hpp"
#include "include/compile.hpp"
#include "include/defines.hpp"

rotate::cstr
main_err(rotate::Stage s)
{
    using namespace rotate;
    switch (s)
    {
        case Stage::file: return "FILE READ";
        case Stage::lexer: return "LEXER";
        case Stage::parser: return "PARSER";
        case Stage::tchecker: return "TYPE CHECKER";
        case Stage::logger: return "LOGGER";
        default: return "UNKNOWN";
    }
    return "UNKNOWN";
}

int
main(const int argc, char **const argv)
{
    using namespace rotate;

    if (argc > 1)
    {
        // parse program arguments
        auto comp_opt = compile_options(argc, argv);

        // setup timer stuff
        clock_t start_t, end_t;
        f128 total_t;
        start_t = clock();

        // compile
        u8 _exit = compile(&comp_opt);
        if (_exit == FAILURE)
            log_stage(main_err(comp_opt.st));
        else if (_exit == SUCCESS)
            log_info("SUCCESS");

        // print comptime
        end_t   = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("[%sTIME%s] : %.5Lf sec\n", LMAGENTA, RESET, total_t);
    }
    else { print_version_and_exit(); }
    return SUCCESS;
}
