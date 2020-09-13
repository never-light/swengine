#pragma once

#ifdef _DEBUG
constexpr bool IS_DEBUG_MODE = true;
#else
constexpr bool IS_DEBUG_MODE = false;
#endif

constexpr bool LOG_RESOURCES_LOADING = true;