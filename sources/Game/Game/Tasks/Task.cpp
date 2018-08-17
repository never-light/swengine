#include "Task.h"

#include <Engine\assertions.h>

Task::Task(const std::string & title, const std::vector<Objective>& objectives)
	: m_title(title),
	m_state(State::NotStarted),
	m_objectives(objectives),
	m_currentObjectiveIndex(0)
{
	_assert(objectives.size() > 0);

	for (size_t objectiveIndex = 0; objectiveIndex < m_objectives.size(); objectiveIndex++)
		m_objectives[objectiveIndex].setObjectiveId(objectiveIndex);
}

Task::~Task()
{

}

const std::string & Task::getStartInfoportion() const
{
	return m_startInfoportion;
}

void Task::setStartInfoportion(const std::string & infoportion)
{
	m_startInfoportion = infoportion;
}

const std::string & Task::getCompleteInfoportion() const
{
	return m_completeInfoportion;
}

void Task::setCompleteInfoportion(const std::string & infoportion)
{
	m_completeInfoportion = infoportion;
}


const std::vector<Objective>& Task::getObjectives() const
{
	return m_objectives;
}

std::vector<Objective>& Task::getObjectives()
{
	return m_objectives;
}

void Task::setCurrentObjective(const Objective & objective)
{
	m_currentObjectiveIndex = objective.getObjectiveId();
}

void Task::start()
{
	m_state = State::Started;
	m_currentObjectiveIndex = 0;
}

bool Task::isStarted() const
{
	return m_state == State::Started;
}

void Task::complete()
{
	m_state = State::Completed;
}

bool Task::isCompleted() const
{
	return m_state == State::Completed;
}

void Task::fail()
{
	m_state = State::Failed;
}

bool Task::isFailed() const
{
	return m_state == State::Failed;
}

const Objective * Task::getCurrentObjective() const
{
	if (!isStarted())
		return nullptr;

	return &m_objectives[m_currentObjectiveIndex];
}

const std::string & Task::getTitle() const
{
	return m_title;
}
