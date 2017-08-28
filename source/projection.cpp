#include "projection.hpp"

#include "config.hpp"

Projection::Projection(float width, float height)
		: fovyDeg(Config::PROJECTION_FOVY_DEGREES), nearPlane(Config::PROJECTION_NEAR_PLANE), farPlane(Config::PROJECTION_FAR_PLANE) {
	update(width, height);
}

Projection::Projection(const VkExtent2D& extent)
		: Projection(extent.width, extent.height) {}

void Projection::update(float width, float height) {
	this->width = width;
	this->height = height;
	projectionMatrix = glm::perspective(glm::radians(fovyDeg), width / height, nearPlane, farPlane);
	projectionMatrix[1][1] *= -1; // compensate for the inverse y coordinate axis (vulkan: y up, opengl: y down)
}

void Projection::update(const VkExtent2D& extent) {
	update(extent.width, extent.height);
}