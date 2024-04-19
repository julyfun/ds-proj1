#include "log.h"

#include <iostream>
#include <string>

namespace log {

void log(const std::string& node, const std::string& msg) {
    std::cerr << "[" << node << "] " << msg << std::endl;
}

std::string indent_of_depth(const int depth) {
    return std::string(depth * 4, ' ');
}

} // namespace log
