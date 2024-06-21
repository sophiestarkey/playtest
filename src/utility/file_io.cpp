#include <fstream>
#include <sstream>

#include "file_io.h"

std::string ReadFile(const std::string& path)
{
    std::ifstream file(path);
    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}