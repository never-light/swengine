#pragma once

namespace sw
{
enum class Platform {
	Windows
};

enum class GraphicsAPI {
	OpenGL
};

enum class DebugLevel {
	FullDebugging
};

static constexpr Platform TARGET_PLATFORM = Platform::Windows;
static constexpr GraphicsAPI TARGET_GRAPHICS_API = GraphicsAPI::OpenGL;
static constexpr DebugLevel DEBUG_LEVEL = DebugLevel::FullDebugging;
}