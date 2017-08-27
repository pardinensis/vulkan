#include "main_window.hpp"

#include "config.hpp"

MainWindow::MainWindow(const Instance& instance)
		: alive(true) {
	// create window
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	width = mode->width;
	height = mode->height;

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(width, height, Config::WINDOW_NAME, monitor, nullptr);

	setupCallbacks();

	if(glfwCreateWindowSurface(instance.getVkInstance(), window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create surface");
	}

	registerKeyPressedHandler([this](KeyCode keyCode) -> bool {
		if (keyCode == GLFW_KEY_ESCAPE) {
			this->alive = false;
			return true;
		}
		return false;
	});
}

MainWindow::~MainWindow() {
	glfwDestroyWindow(window);
}

void MainWindow::setupCallbacks() {
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		MainWindow* mainWindow = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
		if (action == GLFW_PRESS) {
			for (auto& handler : mainWindow->keyPressedHandlers) {
				if (handler(key))
					break;
			}
		}
		else if (action == GLFW_RELEASE) {
			for (auto& handler : mainWindow->keyReleasedHandlers) {
				if (handler(key))
					break;
			}
		}
	});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
		MainWindow* mainWindow = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		if (action == GLFW_PRESS) {
			for (auto& handler : mainWindow->mousePressedHandlers) {
				if (handler(button, static_cast<int>(xpos), static_cast<int>(ypos)))
					break;
			}
		}
		else if (action == GLFW_RELEASE) {
			for (auto& handler : mainWindow->mouseReleasedHandlers) {
				if (handler(button, static_cast<int>(xpos), static_cast<int>(ypos)))
					break;
			}
		}
	});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		MainWindow* mainWindow = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
		for (auto& handler : mainWindow->mouseMovedHandlers) {
			if (handler(static_cast<int>(xpos), static_cast<int>(ypos)))
				break;
		}
	});
}

void MainWindow::registerKeyPressedHandler(const std::function<bool (KeyCode)> &handler) {
	keyPressedHandlers.push_back(handler);
}

void MainWindow::registerKeyReleasedHandler(const std::function<bool (KeyCode)> &handler) {
	keyReleasedHandlers.push_back(handler);
}

void MainWindow::registerMousePressedHandler(const std::function<bool (MouseButton, int, int)> &handler) {
	mousePressedHandlers.push_back(handler);
}

void MainWindow::registerMouseReleasedHandler(const std::function<bool (MouseButton, int, int)> &handler) {
	mouseReleasedHandlers.push_back(handler);
}

void MainWindow::registerMouseMovedHandler(const std::function<bool (int, int)> &handler) {
	mouseMovedHandlers.push_back(handler);
}