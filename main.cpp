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
#define OnlyRenderLines false

float vertices[] = {
	-0.5f, -0.5f,  0.0f,
	 0.5f, -0.5f,  0.0f,
	 0.0f,  0.5f,  0.0f
};

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor; \n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
	"}\0";


int main() {

	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a Window
	GLFWwindow* window = glfwCreateWindow(
		ScreenWidth, ScreenHeight, "LearnOpenGL_C05", NULL, NULL
	);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// Set the Viewport
	glViewport(0, 0, ScreenWidth, ScreenHeight);

	// Create and Compile the Vertex Shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check if Vertex Shader Compiled Successfully
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << endl;
	}

	// Create and Compile the Fragment Shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check if Fragment Shader Compiled Successfully
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << endl;
	}

	// Create a Shader Program to Link the Vertex and Fragment Shader together
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check if Shader Program Compiled Successfully
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << endl;
	}

	// Use the Shader Program
	glUseProgram(shaderProgram);

	// Delete the Compiled Shaders since they are already Linked together
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Generate VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Generate Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Bind:
	// 1.VAOs (Which stores the Vertex Arrtibute)
	// 2.VBOs
	// 3.Set VBO Data
	// 3.Set Vertex Attributes Data

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Send Vertices to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Tell OpenGL how our vertex buffer (vertices[]) was arranged
	glVertexAttribPointer(
		0,                  // Pass the Vertices to Shader at the (location = 0)
		3,                  // Specifies the size of the vertex attribute (vec3)
		GL_FLOAT,           // Specifies the data type (which is float)
		GL_FALSE,           // Specifies if we want the int data to be normalized
		3 * sizeof(float),  // Specifies stride, which is the space
							//     between consecutive vertex attributes.
		(void*)0            // Specifies the offset (where the position begins)
							//     Type is void*, so the cast is needed.
	);

	// Allow Vertex Shader to Access Vertex Attribute at (location = 0)
	glEnableVertexAttribArray(0);

	// Unbind the VBO is allowed,
	// since the VAO ONLY stores Vertex Attribute, NOT the VBO,
	// And the Vertex Attribute includes the VBO,
	// So Unbind the VBO won't make any impact on the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the VAO
	glBindVertexArray(0);

	// Set the rendering mode
	if (OnlyRenderLines) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate Resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

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
