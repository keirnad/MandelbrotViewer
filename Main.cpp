#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GLuint x_offset, y_offset, mandelbrot_scale;

// Vars for settings of mandelbrot set shader
float x_offset_val = -0.5f;

float y_offset_val = 0.0f;

float scale = 2.0f;

int iterations = 250;

float color_multiple = 1.0f;


//Controls
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	// Movement
	if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
		x_offset_val += scale * 0.025;
	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
		x_offset_val -= scale * 0.025;
	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
		y_offset_val += scale * 0.025;
	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
		y_offset_val -= scale * 0.025;

	// Scaling
	if (key == GLFW_KEY_EQUAL && (action == GLFW_REPEAT || action == GLFW_PRESS))
		scale -= scale * 0.025;
	if (key == GLFW_KEY_MINUS && (action == GLFW_REPEAT || action == GLFW_PRESS))
		scale += scale * 0.025;
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

	// Connecting shaders
	Shader shaderProrgam("mandelbrot.vert", "mandelbrot.frag");

	// Binding and creating VAO, EBO
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	// Link buffer
	VAO1.LinkVBO(VBO1, 0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Set up imgui
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

		// Handle controls
		glfwSetKeyCallback(window, key_callback);

		// Activating shader
		shaderProrgam.Activate();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Apply settings of shader mandelbrot set
		glUniform1f(glGetUniformLocation(shaderProrgam.ID, "x_offset"), x_offset_val);
		glUniform1f(glGetUniformLocation(shaderProrgam.ID, "y_offset"), y_offset_val);
		glUniform1i(glGetUniformLocation(shaderProrgam.ID, "iterations"), iterations);
		glUniform1f(glGetUniformLocation(shaderProrgam.ID, "scale"), scale);
		glUniform1f(glGetUniformLocation(shaderProrgam.ID, "color_multiply"), color_multiple);

		//Settings window
		ImGui::Begin("Settings");
		ImGui::SliderInt("Iterations", &iterations, 10, 5000);
		ImGui::SliderFloat("Color", &color_multiple, 0.0f, 5.0f);
		ImGui::End();

		// Controls window
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

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProrgam.Delete();
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}