#pragma once

#include "common_includes.hpp"
#include "swapchain.hpp"

typedef int KeyCode;
typedef int MouseButton;

class MainWindow {
private:
	GLFWwindow* window;
	VkSurfaceKHR surface;

	unsigned int width;
	unsigned int height;

	bool alive;

private:
	std::list<std::function<bool (KeyCode)>> keyPressedHandlers;
	std::list<std::function<bool (KeyCode)>> keyReleasedHandlers;
	std::list<std::function<bool (MouseButton, int, int)>> mousePressedHandlers;
	std::list<std::function<bool (MouseButton, int, int)>> mouseReleasedHandlers;
	std::list<std::function<bool (int, int)>> mouseMovedHandlers;

public:
	MainWindow(const Instance& instance);
	~MainWindow();

private:
	void setupCallbacks();

public:
	void registerKeyPressedHandler(const std::function<bool (KeyCode)>& handler);
	void registerKeyReleasedHandler(const std::function<bool (KeyCode)>& handler);
	void registerMousePressedHandler(const std::function<bool (MouseButton, int, int)>& handler);
	void registerMouseReleasedHandler(const std::function<bool (MouseButton, int, int)>& handler);
	void registerMouseMovedHandler(const std::function<bool (int, int)>& handler);

	const VkSurfaceKHR& getVkSurface() const { return surface; }
	unsigned int getWidth() const { return width; }
	unsigned int getHeight() const { return height; }

	bool isAlive() const { return alive; }
};