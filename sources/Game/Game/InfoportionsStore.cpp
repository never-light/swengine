#include "InfoportionsStore.h"

InfoportionsStore::InfoportionsStore()
{
}

InfoportionsStore::~InfoportionsStore()
{
}

void InfoportionsStore::registerAddInfoportionCallback(const AddInfoportionCallback & callback)
{
	m_addInfoportionCallbacks.push_back(callback);
}

void InfoportionsStore::registerRemoveInfoportionCallback(const RemoveInfoportionCallback & callback)
{
	m_removeInfoportionCallbacks.push_back(callback);
}

void InfoportionsStore::addInfoportion(const std::string & infoportion)
{
	m_infoportions.insert(infoportion);

	for (const AddInfoportionCallback& callback : m_addInfoportionCallbacks)
		callback(infoportion);
}

void InfoportionsStore::removeInfoportion(const std::string & infoportion)
{
	m_infoportions.erase(infoportion);

	for (const RemoveInfoportionCallback& callback : m_removeInfoportionCallbacks)
		callback(infoportion);
}

bool InfoportionsStore::hasInfoportion(const std::string & infoportion) const
{
	return m_infoportions.find(infoportion) != m_infoportions.end();
}
