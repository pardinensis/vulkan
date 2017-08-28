#pragma once

#include "common_includes.hpp"

class Projection {
private:
	const float fovyDeg;
	const float nearPlane;
	const float farPlane;

	float width;
	float height;

	glm::mat4 projectionMatrix;

public:
	Projection(float width, float height);
	Projection(const VkExtent2D& extent);

	void update(float width, float height);
	void update(const VkExtent2D& extent);

	const float getNearPlane() const { return nearPlane; }
	const float getFarPlane() const { return farPlane; }
	const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
};