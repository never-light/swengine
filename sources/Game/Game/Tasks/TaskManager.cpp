#include "TaskManager.h"

#include <Engine\assertions.h>

TaskManager::TaskManager(InfoportionsStore * infoportionsStore)
	: m_infoportionsStore(infoportionsStore),
	m_currentTaskIndex(NO_TASK),
	m_startedTasks(0),
	m_changeCurrentTaskCallback(nullptr)
{
	infoportionsStore->registerAddInfoportionCallback(
		std::bind(&TaskManager::onInfoportionAdd, this, std::placeholders::_1));

	infoportionsStore->registerRemoveInfoportionCallback(
		std::bind(&TaskManager::onInfoportionRemove, this, std::placeholders::_1));
}

TaskManager::~TaskManager()
{
}

void TaskManager::addTask(const Task & task)
{
	m_tasks.push_back(task);
}

const Task* TaskManager::getCurrentTask() const
{
	if (m_currentTaskIndex == NO_TASK)
		return nullptr;

	return &m_tasks[m_currentTaskIndex];
}

void TaskManager::startTask(size_t taskIndex)
{
	m_startedTasks++;
	m_tasks[taskIndex].start();
	m_currentTaskIndex = taskIndex;

	_assert(m_startedTasks == 1);

	if (m_changeCurrentTaskCallback != nullptr)
		m_changeCurrentTaskCallback(getCurrentTask());
}

void TaskManager::setChangeCurrentTaskCallback(const ChangeCurrentTaskCallback & callback)
{
	m_changeCurrentTaskCallback = callback;
}

void TaskManager::onInfoportionAdd(const std::string & infoportion)
{
	bool isCurrentTaskChanged = false;

	for (Task& task : m_tasks) {
		if (task.getStartInfoportion() == infoportion) {
			task.start();
			isCurrentTaskChanged = true;
		}

		bool currentObjectiveCompleted = false;
		bool newObjectiveStarted = false;
		size_t completedObjectiveId = 0;

		if (task.isStarted()) {
			for (Objective& objective : task.getObjectives()) {
				if (objective.getCompleteInfoportion() == infoportion) {
					objective.complete();
					currentObjectiveCompleted = true;
					completedObjectiveId = objective.getObjectiveId();
				}

				if (objective.getStartInfoportion() == infoportion) {
					objective.start();
					task.setCurrentObjective(objective);
					newObjectiveStarted = true;
				}
			}
		}

		if (task.isStarted() && task.getCompleteInfoportion() == infoportion) {
			task.complete();
			m_startedTasks--;
		}

		if (currentObjectiveCompleted && !newObjectiveStarted && !task.isCompleted()) {
			if (completedObjectiveId == (task.getObjectives().size() - 1)) {
				task.complete();
				m_startedTasks--;
				
				m_currentTaskIndex = NO_TASK;
			}
			else {
				Objective& nextObjective = task.getObjectives()[completedObjectiveId + 1];
				nextObjective.start();
				task.setCurrentObjective(nextObjective);
			}
		}

		if (currentObjectiveCompleted || newObjectiveStarted)
			isCurrentTaskChanged = true;
	}
	
	for (size_t taskIndex = 0; taskIndex < m_tasks.size(); taskIndex++) {
		if (m_tasks[taskIndex].isStarted()) {
			m_currentTaskIndex = taskIndex;

			break;
		}
	}

	if (isCurrentTaskChanged && m_changeCurrentTaskCallback != nullptr)
		m_changeCurrentTaskCallback(getCurrentTask());
}

void TaskManager::onInfoportionRemove(const std::string & infoportion)
{
}
