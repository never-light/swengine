#pragma once

#include <unordered_map>
#include <string>
#include <vector>

using SkeletalAnimationVariableId = int16_t;

class SkeletalAnimationStatesMachineVariables {
 public:
  SkeletalAnimationStatesMachineVariables();

  [[nodiscard]] SkeletalAnimationVariableId registerVariable(const std::string& name, float initialValue);
  [[nodiscard]] SkeletalAnimationVariableId getVariableId(const std::string& name) const;

  void setVariableValue(const std::string& name, float value);
  void setVariableValue(SkeletalAnimationVariableId id, float value);

  [[nodiscard]] float getVariableValue(const std::string& name) const;
  [[nodiscard]] float getVariableValue(SkeletalAnimationVariableId id) const;

 private:
  std::unordered_map<std::string, SkeletalAnimationVariableId> m_variablesNameIdMap;
  std::vector<float> m_variablesValues;
};

