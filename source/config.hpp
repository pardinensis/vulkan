#pragma once

#include <glm/glm.hpp>

namespace Config {
	const bool VALIDATION_LAYERS_ENABLED = true;

	const char* const WINDOW_NAME = "";
	// const unsigned int WINDOW_WIDTH = 900;
	// const unsigned int WINDOW_HEIGHT = 600;

	const char* const APPLICATION_NAME = "";
	const unsigned int APPLICATION_VERSION_MAJOR = 1;
	const unsigned int APPLICATION_VERSION_MINOR = 0;
	const unsigned int APPLICATION_VERSION_PATCH = 0;

	const char* const ENGINE_NAME = "No Engine";
	const unsigned int ENGINE_VERSION_MAJOR = 1;
	const unsigned int ENGINE_VERSION_MINOR = 0;
	const unsigned int ENGINE_VERSION_PATCH = 0;

	const float CAMERA_SPEED_LINEAR = 0.5f;
	const float CAMERA_SPEED_ANGULAR = 0.003f;
	const float CAMERA_SPEED_FAST_FACTOR = 5.0f;

	const glm::vec3 CAMERA_INITIAL_POS = {-5.0f, 0.0f, 0.0f };
	const glm::vec3 CAMERA_INITIAL_DIR = { 1.0f, 0.0f, 0.0f };
	const glm::vec3 CAMERA_INITIAL_UP  = { 0.0f, 1.0f, 0.0f };

	const float PROJECTION_FOVY_DEGREES = 60.0f;
	const float PROJECTION_NEAR_PLANE = 0.01f;
	const float PROJECTION_FAR_PLANE = 10.0f;
}