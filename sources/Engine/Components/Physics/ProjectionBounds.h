#pragma once

struct ProjectionBounds {
public:
public:
	static float getOverlapLength(const ProjectionBounds& first, const ProjectionBounds& second);

public:
	ProjectionBounds() : min(0.0f), max(0.0f) {

	}

	float min;
	float max;
};