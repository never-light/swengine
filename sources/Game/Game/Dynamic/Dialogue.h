#pragma once

#include <string>
#include <utility>
#include <unordered_map>

class DialoguePhrase {
 public:
  DialoguePhrase(std::string id, std::string content);

  void setId(const std::string& id);
  [[nodiscard]] const std::string& getId() const;

  void setContent(const std::string& content);
  [[nodiscard]] const std::string& getContent() const;

  void addResponse(const std::string& response);
  [[nodiscard]] const std::vector<std::string>& getResponses() const;

 private:
  std::string m_id;
  std::string m_content;

  std::vector<std::string> m_responses;
};

class Dialogue {
 public:
  explicit Dialogue(std::string id);

  void setId(const std::string& id);
  [[nodiscard]] const std::string& getId() const;

  void addPhrase(const DialoguePhrase& phrase);

  [[nodiscard]] const DialoguePhrase& getStartPhrase() const;
  [[nodiscard]] const DialoguePhrase& getPhrase(const std::string& id) const;

  [[nodiscard]] bool hasPhrase(const std::string& id) const;

 private:
  std::string m_id;
  std::unordered_map<std::string, DialoguePhrase> m_phrases;
};
