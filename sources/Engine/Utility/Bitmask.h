#pragma once

#include <bitset>

template<class T>
class Bitmask {
 public:
  Bitmask() = default;
  ~Bitmask() = default;

  [[nodiscard]] bool test(T bitIndex)
  {
    return m_bits.test(static_cast<size_t>(bitIndex));
  }

  void reset(T bitIndex)
  {
    m_bits.reset(static_cast<size_t>(bitIndex));
  }

  void set(T bitIndex)
  {
    m_bits.set(static_cast<size_t>(bitIndex));
  }

 private:
  std::bitset<sizeof(T)> m_bits;
};

using Bitmask8 = Bitmask<uint8_t>;
using Bitmask16 = Bitmask<uint16_t>;
using Bitmask32 = Bitmask<uint32_t>;
using Bitmask64 = Bitmask<uint64_t>;