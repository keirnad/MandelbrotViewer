

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GLuint x_offset, y_offset, mandelbrot_scale;

float x_offset_val = -0.5f;

float y_offset_val = 0.0f;

double scale = 2.000;

int iterations = 250;

float color_multiple = 1.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
		x_offset_val += scale * 0.025;
	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
		x_offset_val -= scale * 0.025;
	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
		y_offset_val += scale * 0.025;
	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
		y_offset_val -= scale * 0.025;

	if (key == GLFW_KEY_EQUAL && (action == GLFW_REPEAT || action == GLFW_PRESS))
		scale -= scale * 0.025;
	if (key == GLFW_KEY_MINUS && (action == GLFW_REPEAT || action == GLFW_PRESS))
		scale += scale * 0.025;
}

void compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infolog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infolog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infolog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
		}
	}
}

// Function for reading file contents (use it for shaders) https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

int main() {
	// Init GLFW library
	glfwInit();

	// Set GL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	// Use only modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Prevent resizing
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLfloat vertices[] =
	{
		-1.0, 1.0, 0.0,
		1.0, 1.0, 0.0,
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0
	};

	GLuint indices[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	// Creating window and handling error
	GLFWwindow* window = glfwCreateWindow(800, 800, "MandelbrotViewer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return - 1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 800);

	GLuint VBO, VAO, EBO;

	// Get fragment and vertex shaders code 
	std::string vertexCode = get_file_contents("mandelbrot.vert");
	std::string fragmentCode = get_file_contents("mandelbrot.frag");

	// Convert to C string
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Creating and compiling shaders

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(vertexShader, "FRAGMENT");

	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	compileErrors(vertexShader, "PROGRAM");

	// Delete shaders because we dont need them anymore

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Gen buffers and vertex array

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	while (!glfwWindowShouldClose(window))
	{
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glfwSetKeyCallback(window, key_callback);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUniform1f(glGetUniformLocation(shaderProgram, "x_offset"), x_offset_val);
		glUniform1f(glGetUniformLocation(shaderProgram, "y_offset"), y_offset_val);
		glUniform1i(glGetUniformLocation(shaderProgram, "iterations"), iterations);
		glUniform1d(glGetUniformLocation(shaderProgram, "scale"), scale);
		glUniform1f(glGetUniformLocation(shaderProgram, "color_multiply"), color_multiple);

		ImGui::Begin("Settings");
		ImGui::SliderInt("Iterations", &iterations, 10, 5000);
		ImGui::SliderFloat("Color", &color_multiple, 0.0f, 3.0f);
		ImGui::End();

		ImGui::Begin("Controls");
		ImGui::Text("Movements:");
		ImGui::Text("Move Up: W");
		ImGui::Text("Move Down: S");
		ImGui::Text("Move Left: A");
		ImGui::Text("Move Right: D\n\n");
		ImGui::Text("Scaling:");
		ImGui::Text("Zoom: +");
		ImGui::Text("Zoom Out: -");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		// Handling events
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete VAO, EBO, VBO and shader program

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}