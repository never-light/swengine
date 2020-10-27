#include <catch2/catch.hpp>

#include <Engine/Utility/TypeIdentifier.h>

class TypeStorage1 {

};

class TypeStorage2 {

};

class SpecificType1 {

};

class SpecificType2 {

};

class SpecificType3 {

};

TEST_CASE("type_identifier", "[utility]")
{
  REQUIRE(TypeIdentifier<TypeStorage1>::getTypeId<SpecificType1>() == 0);
  REQUIRE(TypeIdentifier<TypeStorage1>::getTypeId<SpecificType2>() == 1);
  REQUIRE(TypeIdentifier<TypeStorage1>::getTypeId<SpecificType3>() == 2);

  REQUIRE(TypeIdentifier<TypeStorage2>::getTypeId<SpecificType1>() == 0);
  REQUIRE(TypeIdentifier<TypeStorage2>::getTypeId<SpecificType2>() == 1);
  REQUIRE(TypeIdentifier<TypeStorage2>::getTypeId<SpecificType3>() == 2);

  REQUIRE(TypeIdentifier<TypeStorage1>::getTypeId<SpecificType1>() == 0);
  REQUIRE(TypeIdentifier<TypeStorage2>::getTypeId<SpecificType1>() == 0);
}
