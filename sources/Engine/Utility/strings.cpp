#include "strings.h"

#include <sstream>

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
