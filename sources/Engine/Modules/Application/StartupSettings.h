#pragma once

enum class StartupOptionScreenMode {
  Windowed,
  Fullscreen
};

enum class StartupOptionScreenDimension {
  _1280x720,
  _1366x768,
  _1440x900
};

// TODO: replace this stub with generic settings storage

class StartupSettings {
 public:
  StartupSettings() = default;

  void setScreenMode(StartupOptionScreenMode screenMode);
  [[nodiscard]] StartupOptionScreenMode getScreenMode() const;

  [[nodiscard]] StartupOptionScreenDimension getScreenDimension() const;
  void setScreenDimension(StartupOptionScreenDimension screenDimension);

 public:
  [[nodiscard]] static StartupOptionScreenMode getScreenModeByName(const std::string& modeName);
  [[nodiscard]] static std::string getScreenModeName(StartupOptionScreenMode screenMode);

  [[nodiscard]] static StartupOptionScreenDimension getScreenDimensionByName(const std::string& dimensionName);
  [[nodiscard]] static std::string getScreenDimensionName(StartupOptionScreenDimension dimension);

 public:
  [[nodiscard]] static StartupSettings loadFromFile();
  static void saveToFile(StartupSettings settings);

 private:
  StartupOptionScreenMode m_screenMode = StartupOptionScreenMode::Windowed;
  StartupOptionScreenDimension m_screenDimension = StartupOptionScreenDimension::_1280x720;
};

