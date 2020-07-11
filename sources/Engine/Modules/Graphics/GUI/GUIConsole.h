#pragma once

#include <string>
#include <list>
#include <memory>

#include "BitmapFont.h"
#include "GUIWidget.h"
#include "GUITextBox.h"
#include "GUIText.h"

class GUIConsole;

struct GameConsoleChangeVisibilityEvent {
  bool isVisible;
};

class GUIConsoleCommandsExecutor {
 public:
  GUIConsoleCommandsExecutor() = default;
  virtual ~GUIConsoleCommandsExecutor() = default;

  virtual void executeCommand(const std::string& command, GUIConsole& console) = 0;
};

class GUIConsoleCommandsBackPrinter : public GUIConsoleCommandsExecutor {
 public:
  void executeCommand(const std::string& command, GUIConsole& console) override;
};

class GUIConsole : public GUIWidget {
 public:
  GUIConsole(std::shared_ptr<GUIConsoleCommandsExecutor> commandsExecutor, int historySize,
    std::shared_ptr<BitmapFont> font);

  [[nodiscard]] int getHistorySize() const;

  void setTextFontSize(int size);
  [[nodiscard]] int getTextFontSize() const;

  void setTextColor(const glm::vec4& color);
  [[nodiscard]] glm::vec4 getTextColor() const;

  void setTextHoverColor(const glm::vec4& color);
  [[nodiscard]] glm::vec4 getTextHoverColor() const;

  [[nodiscard]] std::shared_ptr<GUITextBox> getTextBox() const;

  void print(const std::string& text);

 protected:
  void recalculateLayout();
  void transformationCacheUpdate() override;

  void processConsoleKeyboardEvent(const GUIKeyboardEvent& event);

 private:
  std::shared_ptr<GUIConsoleCommandsExecutor> m_commandsExecutor;

  int m_historySize;
  int m_historyFreePosition = 0;

  int m_textFontSize;

  glm::vec4 m_textColor = {0.0f, 0.0f, 0.0f, 1.0f};
  glm::vec4 m_textHoverColor = {0.0f, 0.0f, 0.0f, 1.0f};

  std::shared_ptr<GUITextBox> m_commandsTextBox;
  std::list<std::shared_ptr<GUIText>> m_textLines;
};
