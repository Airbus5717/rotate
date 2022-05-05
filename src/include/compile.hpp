#include "common.hpp"

namespace rotate
{

struct compile_options
{
    const char *filename = NULL;
    bool debug_info      = false;
    bool debug_symbols   = false;

    compile_options(const char *filename, bool debug_info, bool debug_symbols)
        : filename(filename), debug_info(debug_info), debug_symbols(debug_symbols)
    {
    }

    ~compile_options() = default;
};

//
int compile(compile_options *options) noexcept;

} // namespace rotate
