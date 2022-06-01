#include <iostream>
#include <string_view>
#include <source_location>

inline void log_msg(const std::string_view message, const std::source_location location = std::source_location::current()){
    std::cout << message << " ";
    std::cout << "in file: ";
    std::cout << location.file_name() << "(";
    std::cout << location.line() << ":";
    std::cout << location.column() << ") ";
    std::cout << location.function_name() << "\n";
}
