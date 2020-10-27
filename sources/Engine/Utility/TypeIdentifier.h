#pragma once

template<class TypeFamily>
class TypeIdentifier {
 public:
  template<class SpecificType>
  static size_t getTypeId() {
    static size_t typeId = s_typeId++;

    return typeId;
  }

 private:
  static size_t s_typeId;
};

template <typename TypeFamily>
size_t TypeIdentifier<TypeFamily>::s_typeId = 0;
