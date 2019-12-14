#include "strings.h"

#include <sstream>
#include <algorithm>

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter)
{
   std::vector<std::string> entries;
   std::string entry;

   std::istringstream stringStream(str);

   while (std::getline(stringStream, entry, delimiter)) {
      entries.push_back(entry);
   }

   return entries;
}

std::string StringUtils::toLowerCase(const std::string& str)
{
    std::string result;

    for (auto ch : str) {
        result.push_back(static_cast<char>(std::tolower(ch)));
    }

    return result;
}
