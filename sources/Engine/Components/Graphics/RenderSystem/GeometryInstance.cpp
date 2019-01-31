#include "GeometryInstance.h"

bool GeometryInstance::hasAttribute(CommonAttrs attribute) const
{
	return hasAttribute(static_cast<int>(attribute));
}

void GeometryInstance::setAttributeDesc(CommonAttrs attributeId, const GeometryAttributeDesc & description)
{
	return setAttributeDesc(static_cast<int>(attributeId), description);
}

GeometryAttributeDesc GeometryInstance::getAttributeDesc(CommonAttrs attributeId) const
{
	return getAttributeDesc(static_cast<int>(attributeId));
}
