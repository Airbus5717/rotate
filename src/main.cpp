#include "include/compile.hpp"

void print_version_and_exit()
{
    fprintf(stdout, "Rotate Compiler \nVersion: %s\n", VERSION);
}

int main(const int argc, char **const argv)
{
    try
    {
        if (argc > 1)
        {
            return rotate::compile(argv[1]);
        }
        else
        {
            print_version_and_exit();
        }
    }
    catch (std::exception &e)
    {
        rotate::log_error(e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
