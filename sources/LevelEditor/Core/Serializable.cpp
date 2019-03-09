#include "Serializable.h"

QDataStream & operator<<(QDataStream & out, const Serializable & object)
{
	object.serialize(out);
	return out;
}

QDataStream & operator>>(QDataStream & in, Serializable & object)
{
	object.deserialize(in);
	return in;
}
