#include "files.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Exceptions/EngineRuntimeException.h"

bool FileUtills::isFileExists(const std::string& path)
{
    std::ifstream file;
    file.open(path);
    file.close();

    return static_cast<bool>(file);
}

