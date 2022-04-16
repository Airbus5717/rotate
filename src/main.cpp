#include "include/compile.hpp"

void print_version_and_exit()
{
    fprintf(stdout, "Rotate Compiler \nVersion: %s\n", VERSION);
}

int main(const int argc, char **const argv) noexcept
{
    if (argc > 1)
    {
        if (rotate::compile(argv[1]) == EXIT_FAILURE)
        {
            rotate::log_error("FAILURE");
        }
        else
        {
            rotate::log_info("SUCCESS");
        }
    }
    else
    {
        print_version_and_exit();
    }
    return EXIT_SUCCESS;
}
