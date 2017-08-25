#pragma once

#include "common_includes.hpp"

class Camera {
private:
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 up;

	float pitch;

	glm::mat4 viewMatrix;

	const float slowSpeed;
	const float fastSpeed;
	const float rotSpeed;

	bool keyForward;
	bool keyBackward;
	bool keyLeft;
	bool keyRight;
	bool keyFast;

	bool buttonViewDirection;

	int lastMousePosX;
	int lastMousePosY;

	std::chrono::steady_clock::time_point lastUpdate;

public:
	Camera(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up);
	Camera();

	void keyPressed(int keycode);
	void keyReleased(int keycode);
	void mouseButtonPressed(int button, int x, int y);
	void mouseButtonReleased(int button);
	void mouseMoved(int x, int y);

	void update();

	const glm::mat4& getViewMatrix() const { return viewMatrix; }

private:
	double getTimeSinceLastUpdate();
};