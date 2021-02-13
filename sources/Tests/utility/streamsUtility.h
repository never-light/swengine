#pragma once

#include <fstream>

class NullStreamBuffer : public std::streambuf {
 protected:
  int overflow(int c) override
  {
    setp(stubBuffer, stubBuffer + sizeof(stubBuffer));
    return (c == traits_type::eof()) ? '\0' : c;
  }

 private:
  char stubBuffer[64]{};
};

class NullOStream : private NullStreamBuffer, public std::ostream {
 public:
  NullOStream() : std::ostream(this)
  {}
};
