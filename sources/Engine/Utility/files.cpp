#include "files.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Exceptions/EngineRuntimeException.h"

bool FileUtills::isFileExists(const std::string& str)
{
    std::ifstream file;
    file.open(str);
    file.close();
    if (!file){
        ENGINE_RUNTIME_ERROR("Resourse file does not exist " + str);
        return false;
    }
    return true;
}

