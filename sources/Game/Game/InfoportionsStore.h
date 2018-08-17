#pragma once

#include <string>
#include <unordered_set>
#include <functional>

class InfoportionsStore {
public:
	using AddInfoportionCallback = std::function<void(const std::string& string)>;
	using RemoveInfoportionCallback = std::function<void(const std::string& string)>;
public:
	InfoportionsStore();
	~InfoportionsStore();

	void registerAddInfoportionCallback(const AddInfoportionCallback& callback);
	void registerRemoveInfoportionCallback(const RemoveInfoportionCallback& callback);

	void addInfoportion(const std::string& infoportion);
	void removeInfoportion(const std::string& infoportion);

	bool hasInfoportion(const std::string& infoportion) const;
protected:
	std::unordered_set<std::string> m_infoportions;

	std::vector<AddInfoportionCallback> m_addInfoportionCallbacks;
	std::vector<RemoveInfoportionCallback> m_removeInfoportionCallbacks;

};