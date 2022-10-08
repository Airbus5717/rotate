#include "include/common.hpp"
#include "include/compile.hpp"

int main(const int argc, char **const argv)
{
    using namespace rotate;
    if (argc > 1)
    {
        auto comp_opt           = compile_options(argc, argv);

        u8 _exit = compile(&comp_opt);
        if (_exit == EXIT_FAILURE)
            log_error("FAILURE");
        else if (_exit == EXIT_SUCCESS)
            log_info("SUCCESS");
    }
    else
    {
        print_version_and_exit();
    }
    return EXIT_SUCCESS;
}
