#pragma once

#include <algorithm>
#include <string>
#include <optional>

using std::optional;

namespace argparser {

bool option_present(char** begin, char** end, std::string const& option);
optional<std::string> option_value(char** begin, char** end, std::string const& option);


} // namespace argparser