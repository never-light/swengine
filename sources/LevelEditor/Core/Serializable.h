#pragma once

#include <QDataStream>

class Serializable {
public:
	virtual void serialize(QDataStream& stream) const = 0;
	virtual void deserialize(QDataStream& stream) = 0;
};

QDataStream& operator<<(QDataStream &out, const Serializable& object);
QDataStream& operator>>(QDataStream &in, Serializable& object);