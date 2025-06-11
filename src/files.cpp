#include "lgf/files.h"
#include <fstream>
#include <sstream>

std::string LGF::IO::readFile(const char* fileName) {
    std::ifstream file(fileName);
    if (!file) {
        perror("ifstream failed");
        return "";
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}