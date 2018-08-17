#include "Objective.h"

Objective::Objective(const std::string & title)
	: m_title(title),
	m_state(State::NotStarted)
{
}

Objective::~Objective()
{

}

const std::string & Objective::getStartInfoportion() const
{
	return m_startInfoportion;
}

void Objective::setStartInfoportion(const std::string & infoportion)
{
	m_startInfoportion = infoportion;
}

const std::string & Objective::getCompleteInfoportion() const
{
	return m_completeInfoportion;
}

void Objective::setCompleteInfoportion(const std::string & infoportion)
{
	m_completeInfoportion = infoportion;
}

void Objective::start()
{
	m_state = State::Started;
}

bool Objective::isStarted() const
{
	return m_state == State::Started;
}

void Objective::complete()
{
	m_state = State::Completed;
}

bool Objective::isCompleted() const
{
	return m_state == State::Completed;
}

void Objective::fail()
{
	m_state = State::Failed;
}

bool Objective::isFailed() const
{
	return m_state == State::Failed;
}

size_t Objective::getObjectiveId() const
{
	return m_objectiveId;
}

void Objective::setObjectiveId(size_t id)
{
	m_objectiveId = id;
}

const std::string & Objective::getTitle() const
{
	return m_title;
}
