#pragma once

#include "SGLog.h"

#define sgFree(object) if (object) { delete object; object = nullptr; }