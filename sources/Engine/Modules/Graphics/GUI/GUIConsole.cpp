#include "precompiled.h"

#pragma hdrstop

#include "GUIConsole.h"

GUIConsole::GUIConsole(std::shared_ptr<GUIConsoleCommandsExecutor> commandsExecutor, int historySize,
                       std::shared_ptr<BitmapFont> font)
    : m_commandsExecutor(commandsExecutor),
      m_historySize(historySize),
      m_textFontSize(font->getBaseSize()) {
  SW_ASSERT(m_commandsExecutor != nullptr && font != nullptr);
  SW_ASSERT(historySize >= 0);

  for (size_t textLineIndex = 0; textLineIndex < static_cast<size_t>(historySize); textLineIndex++) {
    std::shared_ptr<GUIText> textLine = std::make_shared<GUIText>(font, "");
    m_textLines.push_back(textLine);
  }

  m_commandsTextBox = std::make_shared<GUITextBox>(font);
  m_commandsTextBox
      ->setKeyboardEventCallback(std::bind(&GUIConsole::processConsoleKeyboardEvent, this, std::placeholders::_1));

  recalculateLayout();
}

int GUIConsole::getHistorySize() const {
  return m_historySize;
}

void GUIConsole::setTextFontSize(int size) {
  m_textFontSize = size;

  recalculateLayout();
}

int GUIConsole::getTextFontSize() const {
  return m_textFontSize;
}

void GUIConsole::setTextColor(const glm::vec4& color) {
  m_textColor = color;

  for (auto textLine : m_textLines) {
    textLine->setColor(color);
  }
}

glm::vec4 GUIConsole::getTextColor() const {
  return m_textColor;
}

void GUIConsole::setTextHoverColor(const glm::vec4& color) {
  m_textHoverColor = color;

  for (auto textLine : m_textLines) {
    textLine->setHoverColor(color);
  }
}

glm::vec4 GUIConsole::getTextHoverColor() const {
  return m_textHoverColor;
}

std::shared_ptr<GUITextBox> GUIConsole::getTextBox() const {
  return m_commandsTextBox;
}

void GUIConsole::print(const std::string& text) {
  if (m_historyFreePosition >= m_historySize) {
    std::shared_ptr<GUIText> firstLine = m_textLines.front();
    m_textLines.pop_front();

    firstLine->setText(text);
    m_textLines.push_back(firstLine);
  }
  else {
    auto textLinesIt = m_textLines.begin();
    std::advance(textLinesIt, m_historyFreePosition);

    (*textLinesIt)->setText(text);

    m_historyFreePosition++;
  }

  recalculateLayout();
}

void GUIConsole::recalculateLayout() {
  int textVerticalOffset = 0;

  for (std::shared_ptr<GUIText> textLine : m_textLines) {
    textLine->setOrigin({10, textVerticalOffset});
    textLine->setFontSize(m_textFontSize);

    textVerticalOffset += static_cast<int>(m_textFontSize * 2.0f);
  }

  textVerticalOffset += m_textFontSize;

  m_commandsTextBox->setOrigin({0, textVerticalOffset});
  m_commandsTextBox->setSize({getSize().x, 25});

  setHeight(textVerticalOffset + 25);
}

void GUIConsole::transformationCacheUpdate() {
  if (m_commandsTextBox->getParent() == nullptr) {
    addChildWidget(m_commandsTextBox);

    for (auto textLine : m_textLines) {
      addChildWidget(textLine);
    }
  }
}

void GUIConsole::processConsoleKeyboardEvent(const GUIKeyboardEvent& event) {
  if (event.type == KeyboardEventType::KeyDown) {
    if (event.keyCode == SDLK_RETURN && !m_commandsTextBox->getText().empty()) {
      m_commandsExecutor->executeCommand(m_commandsTextBox->getText(), *this);
      m_commandsTextBox->setText("");
    }
  }
}

void GUIConsoleCommandsBackPrinter::executeCommand(const std::string& command, GUIConsole& console) {
  console.print(command);
}
