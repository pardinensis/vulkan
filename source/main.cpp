#include "app.hpp"

int main() {
	App app;

	try {
		app.init("");
		app.run();
		app.cleanup();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}