#pragma once


class SkeletalAnimationStatesMachineVariables
{
public:
    SkeletalAnimationStatesMachineVariables();

    size_t registerVaraible(const std::string& name, float initialValue);
    size_t getVariableId(const std::string& name) const;

    void setVariableValue(const std::string& name, float value);
    void setVariableValue(size_t id, float value);

    float getVariableValue(const std::string& name) const;
    float getVariableValue(size_t id) const;

private:
    std::unordered_map<std::string, size_t> m_variablesNameIdMap;
    std::vector<float> m_variablesValues;
};

