#pragma once

enum class Platform {
  Win64,
  Linux
};

#if defined __linux__
constexpr Platform TARGET_PLATFORM = Platform::Linux;
#elif defined _WIN64
constexpr Platform TARGET_PLATFORM = Platform::Win64;
#else
#error "Platform is not supported"
#endif

#ifdef _DEBUG
constexpr bool IS_DEBUG_MODE = true;
#else
constexpr bool IS_DEBUG_MODE = false;
#endif

constexpr bool LOG_RESOURCES_LOADING = true;
constexpr bool LOG_GPU_ADDITIONAL_DEBUG_INFO_MESSAGES = false;
