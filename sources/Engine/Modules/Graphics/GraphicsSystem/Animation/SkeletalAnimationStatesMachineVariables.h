#pragma once

using SkeletalAnimationVariableId = int16_t;

class SkeletalAnimationStatesMachineVariables
{
public:
    SkeletalAnimationStatesMachineVariables();

    SkeletalAnimationVariableId registerVariable(const std::string& name, float initialValue);
    SkeletalAnimationVariableId getVariableId(const std::string& name) const;

    void setVariableValue(const std::string& name, float value);
    void setVariableValue(SkeletalAnimationVariableId id, float value);

    float getVariableValue(const std::string& name) const;
    float getVariableValue(SkeletalAnimationVariableId id) const;

private:
    std::unordered_map<std::string, SkeletalAnimationVariableId> m_variablesNameIdMap;
    std::vector<float> m_variablesValues;
};

