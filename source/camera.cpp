#include "camera.hpp"

#include "config.hpp"

Camera::Camera(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up)
		: pos(pos), dir(glm::normalize(dir)), up(up),
		  slowSpeed(CAMERA_SPEED_LINEAR), fastSpeed(CAMERA_SPEED_LINEAR * CAMERA_SPEED_FAST_FACTOR), rotSpeed(CAMERA_SPEED_ANGULAR) {

	// assume no keys are pressed
	keyForward = false;
	keyBackward = false;
	keyLeft = false;
	keyRight = false;
	keyFast = false;

	buttonViewDirection = false;

	// TODO
	pitch = 0;
}

Camera::Camera()
		: Camera(CAMERA_INITIAL_POS, CAMERA_INITIAL_DIR, CAMERA_INITIAL_UP) {}

void Camera::keyPressed(int keycode) {
	switch (keycode) {
		case GLFW_KEY_W: keyForward = true; break;
		case GLFW_KEY_A: keyLeft = true; break;
		case GLFW_KEY_S: keyBackward = true; break;
		case GLFW_KEY_D: keyRight = true; break;
		case GLFW_KEY_LEFT_CONTROL: keyFast = true; break;
		default: return;
	}
}

void Camera::keyReleased(int keycode) {
	switch (keycode) {
		case GLFW_KEY_W: keyForward = false; break;
		case GLFW_KEY_A: keyLeft = false; break;
		case GLFW_KEY_S: keyBackward = false; break;
		case GLFW_KEY_D: keyRight = false; break;
		case GLFW_KEY_LEFT_CONTROL: keyFast = false; break;
		default: return;
	}
}

void Camera::mouseButtonPressed(int button, int x, int y) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		lastMousePosX = x;
		lastMousePosY = y;
		buttonViewDirection = true;
	}
}

void Camera::mouseButtonReleased(int button) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		buttonViewDirection = false;
	}
}

void Camera::mouseMoved(int x, int y) {
	if (!buttonViewDirection) {
		return;
	}

	int xrel = x - lastMousePosX;
	int yrel = y - lastMousePosY;
	lastMousePosX = x;
	lastMousePosY = y;

	glm::vec3 right = glm::normalize(glm::cross(dir, up));
	glm::quat quatYaw = glm::angleAxis(-rotSpeed * xrel, up);

	float newpitch = pitch - rotSpeed * yrel;
	if (newpitch > M_PI/2 - 0.01)
		newpitch = M_PI/2 - 0.01;
	if (newpitch < -M_PI/2 + 0.01)
		newpitch = -M_PI/2 + 0.01;

	glm::quat quatPitch = glm::angleAxis(newpitch - pitch, right);
	glm::quat rotation = quatYaw * quatPitch;
	dir = rotation * dir;

	pitch = newpitch;
}

void Camera::update() {
	double seconds = getTimeSinceLastUpdate();

	glm::vec3 right = glm::normalize(glm::cross(dir, up));
	float speed = (keyFast) ? fastSpeed : slowSpeed;
	float forwardSpeed = (static_cast<int>(keyForward) - static_cast<int>(keyBackward)) * speed * seconds;
	float sidewardSpeed = (static_cast<int>(keyRight) - static_cast<int>(keyLeft)) * speed * seconds;

	pos += forwardSpeed * dir;
	pos += sidewardSpeed * right;

	viewMatrix = glm::lookAt(pos, pos + dir, up);
}

double Camera::getTimeSinceLastUpdate() {
	using namespace std::chrono;

	steady_clock::time_point now = steady_clock::now();
	
	duration<double> timeSpan = duration_cast<duration<double>>(now - lastUpdate);
	double seconds = timeSpan.count();
	if (seconds < 0 || seconds > 10) {
		seconds = 0;
	}

	lastUpdate = now;
	return seconds;
}