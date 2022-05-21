#include <iostream>
using std::cout;
using std::endl;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

#define ScreenFactor (2)
#define ScreenWidth  (800 * ScreenFactor)
#define ScreenHeight (600 * ScreenFactor)

float vertices[] = {
	-0.5f, -0.5f,  0.0f,
	 0.5f, -0.5f,  0.0f,
	 0.0f,  0.5f,  0.0f
};


int main() {

	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a Window
	GLFWwindow* window = glfwCreateWindow(
		ScreenWidth, ScreenHeight, "LearnOpenGL_C04", NULL, NULL
	);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set the Viewport
	glViewport(0, 0, ScreenWidth, ScreenHeight);

	// Set the Window Resizing Callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Generate Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Send Vertices to the Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean Up after the Window is Closed
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
