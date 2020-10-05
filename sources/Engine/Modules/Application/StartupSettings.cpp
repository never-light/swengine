#include "precompiled.h"

#pragma hdrstop

#include "StartupSettings.h"

#include "Utility/files.h"
#include "Utility/xml.h"

#include "Exceptions/exceptions.h"

StartupOptionScreenMode StartupSettings::getScreenMode() const
{
  return m_screenMode;
}

void StartupSettings::setScreenMode(StartupOptionScreenMode screenMode)
{
  m_screenMode = screenMode;
}

StartupOptionScreenDimension StartupSettings::getScreenDimension() const
{
  return m_screenDimension;
}

void StartupSettings::setScreenDimension(StartupOptionScreenDimension screenDimension)
{
  m_screenDimension = screenDimension;
}

StartupOptionScreenMode StartupSettings::getScreenModeByName(const std::string& modeName)
{
  if (modeName == "windowed") {
    return StartupOptionScreenMode::Windowed;
  }
  else if (modeName == "fullscreen") {
    return StartupOptionScreenMode::Fullscreen;
  }
  else {
    SW_ASSERT(false);
    return {};
  }
}

std::string StartupSettings::getScreenModeName(StartupOptionScreenMode screenMode)
{
  switch (screenMode) {
    case StartupOptionScreenMode::Windowed:
      return "windowed";

    case StartupOptionScreenMode::Fullscreen:
      return "fullscreen";

    default:
      SW_ASSERT(false);
      return {};
  }
}

StartupOptionScreenDimension StartupSettings::getScreenDimensionByName(const std::string& modeName)
{
  if (modeName == "1280x720") {
    return StartupOptionScreenDimension::_1280x720;
  }
  else if (modeName == "1366x768") {
    return StartupOptionScreenDimension::_1366x768;
  }
  else if (modeName == "1440x900") {
    return StartupOptionScreenDimension::_1440x900;
  }
  else {
    SW_ASSERT(false);
    return {};
  }
}


std::string StartupSettings::getScreenDimensionName(StartupOptionScreenDimension dimension)
{
  switch (dimension) {
    case StartupOptionScreenDimension::_1280x720:
      return "1280x720";

    case StartupOptionScreenDimension::_1366x768:
      return "1366x768";

    case StartupOptionScreenDimension::_1440x900:
      return "1440x900";

    default:
      SW_ASSERT(false);
      return {};
  }
}



StartupSettings StartupSettings::loadFromFile()
{
  std::string settingsPath(FileUtils::STARTUP_SETTINGS_PATH);
  spdlog::debug("Load settings from {}", settingsPath);

  auto settingsData = XMLUtils::openDescriptionFile(settingsPath, "settings");
  pugi::xml_node settingsNode = std::get<1>(settingsData);

  StartupSettings settings;

  for (pugi::xml_node optionNode : settingsNode.children("option")) {
    std::string optionName = optionNode.attribute("name").as_string();

    if (optionName == "screen-dimension") {
      settings.setScreenDimension(getScreenDimensionByName(optionNode.child_value()));
    }
    else if (optionName == "screen-mode") {
      settings.setScreenMode(getScreenModeByName(optionNode.child_value()));
    }
    else {
      THROW_EXCEPTION(EngineRuntimeException,
        fmt::format("Unknown startup option {}", optionName));
    }
  }

  return settings;
}

void StartupSettings::saveToFile(StartupSettings settings)
{
  std::string settingsPath(FileUtils::STARTUP_SETTINGS_PATH);
  spdlog::debug("Save settings to {}", settingsPath);

  auto settingsData = XMLUtils::openDescriptionFile(settingsPath, "settings");
  pugi::xml_node settingsNode = std::get<1>(settingsData);

  for (pugi::xml_node optionNode : settingsNode.children("option")) {
    std::string optionName = optionNode.attribute("name").as_string();

    if (optionName == "screen-dimension") {
      optionNode.first_child().set_value(getScreenDimensionName(settings.getScreenDimension()).c_str());
    }
    else if (optionName == "screen-mode") {
      optionNode.first_child().set_value(getScreenModeName(settings.getScreenMode()).c_str());
    }
    else {
      THROW_EXCEPTION(EngineRuntimeException,
        fmt::format("Unknown startup option {}", optionName));
    }
  }

  pugi::xml_document& settingsDoc = std::get<0>(settingsData);
  bool saveResult = settingsDoc.save_file(settingsPath.c_str());

  if (!saveResult) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("It is impossible to save startup options to file {}", settingsPath));
  }
}
