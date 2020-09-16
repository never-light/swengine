//#ifndef USE_PRECOMPILED_HEADERS
//#pragma error
//#endif

#if defined __cplusplus

// Utility header
#include "Utility/helpers.h"

// Standard library
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <cmath>
#include <array>
#include <type_traits>
#include <typeindex>
#include <fstream>
#include <sstream>
#include <utility>
#include <numbers>
#include <numeric>
#include <span>
#include <tuple>

// Third-party dependencies
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <SDL.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/extended_min_max.hpp>

#include <pugixml.hpp>
#include <stb_image.h>
#include <spdlog/spdlog.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>

// Engine headers to precompile
#include "swdebug.h"

#endif
