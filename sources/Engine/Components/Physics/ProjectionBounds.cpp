#include "ProjectionBounds.h"

#include <algorithm>

float ProjectionBounds::getOverlapLength(const ProjectionBounds & first, const ProjectionBounds & second)
{
	if (!(first.min <= second.max && first.max >= second.min)) {
		return 0.0f;
	}

	return std::min(first.max, second.max) - std::max(first.min, second.min);
}
