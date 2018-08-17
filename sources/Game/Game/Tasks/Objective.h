#pragma once

#include <string>

class Objective {
public:
	enum class State {
		NotStarted, Started, Completed, Failed
	};

public:
	Objective(const std::string& title);
	~Objective();

	const std::string& getStartInfoportion() const;
	void setStartInfoportion(const std::string& infoportion);

	const std::string& getCompleteInfoportion() const;
	void setCompleteInfoportion(const std::string& infoportion);

	void start();
	bool isStarted() const;

	void complete();
	bool isCompleted() const;

	void fail();
	bool isFailed() const;

	size_t getObjectiveId() const;
	void setObjectiveId(size_t id);

	const std::string& getTitle() const;

protected:
	std::string m_title;

	std::string m_startInfoportion;
	std::string m_completeInfoportion;

	State m_state;

	size_t m_objectiveId;
};