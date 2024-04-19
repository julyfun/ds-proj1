#include <string>

namespace log {

void log(const std::string& node, const std::string& msg);
std::string indent_of_depth(const int depth);

} // namespace log
