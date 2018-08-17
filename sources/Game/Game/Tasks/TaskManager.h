#pragma once

#include <Game\Game\InfoportionsStore.h>
#include "Task.h"

class TaskManager {
public:
	using ChangeCurrentTaskCallback = std::function<void(const Task*)>;

public:
	TaskManager(InfoportionsStore* infoportionsStore);
	~TaskManager();

	void addTask(const Task& task);
	const Task* getCurrentTask() const;

	void startTask(size_t taskIndex);

	void setChangeCurrentTaskCallback(const ChangeCurrentTaskCallback& callback);

private:
	void onInfoportionAdd(const std::string& infoportion);
	void onInfoportionRemove(const std::string& infoportion);
	
private:
	InfoportionsStore * m_infoportionsStore;

	std::vector<Task> m_tasks;
	size_t m_currentTaskIndex;

	size_t m_startedTasks;

	ChangeCurrentTaskCallback m_changeCurrentTaskCallback;
private:
	const static int NO_TASK = 99999999;
};