#include "precompiled.h"

#pragma hdrstop

#include "numbers.h"

#include "Exceptions/exceptions.h"

std::mt19937& Random::getMt19937()
{
  static std::random_device randomDevice;
  static std::mt19937 mt19937(randomDevice());

  return mt19937;
}
