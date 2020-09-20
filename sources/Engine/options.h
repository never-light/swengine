#pragma once

#ifdef _DEBUG
constexpr bool IS_DEBUG_MODE = true;
constexpr bool LOG_RESOURCES_LOADING = true;
#else
constexpr bool IS_DEBUG_MODE = false;
constexpr bool LOG_RESOURCES_LOADING = true;
#endif
