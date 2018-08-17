#pragma once

#include "Objective.h"
#include <vector>

class Task {
public:
	enum class State {
		NotStarted, Started, Completed, Failed
	};

public:
	Task(const std::string& title, const std::vector<Objective>& objectives);
	~Task();

	const std::string& getStartInfoportion() const;
	void setStartInfoportion(const std::string& infoportion);

	const std::string& getCompleteInfoportion() const;
	void setCompleteInfoportion(const std::string& infoportion);

	const std::vector<Objective>& getObjectives() const;
	std::vector<Objective>& getObjectives();

	void setCurrentObjective(const Objective& objective);

	void start();
	bool isStarted() const;

	void complete();
	bool isCompleted() const;

	void fail();
	bool isFailed() const;

	const Objective* getCurrentObjective() const;

	const std::string& getTitle() const;

private:
	std::string m_title;

	std::string m_startInfoportion;
	std::string m_completeInfoportion;

	std::vector<Objective> m_objectives;
	size_t m_currentObjectiveIndex;

	State m_state;
};