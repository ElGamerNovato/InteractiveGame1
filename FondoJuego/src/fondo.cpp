#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLFWwindow* initializeWindow(int width, int height, const char* title) {
	if (!glfwInit()) {
		std::cerr << "Fallo al inicializar GLFW" << std::endl;
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr,nullptr);
	if (!window) {
		std::cerr << "Fallo al crear la ventana GLFW" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (glewInit() != GLEW_OK) {
		std::cerr << "Fallo al inicializar GLEW" << std::endl;
		return nullptr;
	}
	return window;
}

void setup2DScene(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawStars(int width, int height, int numStars) {
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_POINTS);
	for (int i = 0; i < numStars; ++i) {
		float x = static_cast<float>(rand() % width);
		float y = static_cast<float>(rand() % height);
		glVertex2f(x, y);
	}
	glEnd();
}

int main() {
	int width = 800, height = 600;
	int numStars = 100;

	std::srand(static_cast<unsigned int>(std::time(0)));

	GLFWwindow* window = initializeWindow(width, height, "Night Background");
	if (!window) {
		return -1;
	}

	setup2DScene(width, height);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		drawStars(width, height, numStars);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}