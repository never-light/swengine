#include <iostream>
#include <stb_image.h>
#include <spdlog/spdlog.h>

int main() {
  spdlog::info("Info message");
  spdlog::warn("Warning message");
  spdlog::error("Error message");

  return 0;
}