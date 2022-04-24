#include "include/common.hpp"
#include "include/compile.hpp"

void print_version_and_exit()
{
    fprintf(stdout, "Rotate Compiler \nVersion: %s\n", VERSION);
}

int main(const int argc, char **const argv) noexcept
{
    using namespace rotate;
    if (argc > 1)
    {
        u8 exit = compile(argv[1]);
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
