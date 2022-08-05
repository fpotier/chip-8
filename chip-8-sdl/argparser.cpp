#include <algorithm>

#include "argparser.h"

namespace argparser {

bool option_present(char** begin, char** end, std::string const& option)
{
    return std::find(begin, end, option) != end;
}

optional<std::string> option_value(char** begin, char** end, std::string const& option)
{
    char** it = std::find(begin, end, option);
    if (it != end && ++it != end)
        return std::string(*it);

    return std::nullopt;
}

} // namespace argparser