
#include "libs/glad_loader_docs.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

#include "PolygonWithTexture.cpp"
#include "Polygon.cpp"

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include "stb_image.h"

using namespace glm;

// =====================================================
// 1. Window Settings
// =====================================================

int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;

// =====================================================
// 2. Camera Settings
// =====================================================

Camera camera(vec3(0.0f, 0.0f, 4.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// =====================================================
// 3. Function Declarations
// =====================================================

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void drawTableLeg(Shader &shader, float x, float z);
// =====================================================
// 4. Main Function
// =====================================================

int main()
{
	// -------------------------------------------------
	// 4.1 Initialize GLFW and Create Window
	// -------------------------------------------------

	glfwInit();

	GLFWwindow *window = glfwCreateWindow(
		SCR_WIDTH,
		SCR_HEIGHT,
		"Lecture 4",
		NULL,
		NULL);

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);

	stbi_set_flip_vertically_on_load(true);

	// -------------------------------------------------
	// 4.2 Load Shader
	// -------------------------------------------------

	Shader ourShader(
		"./shaders/vs/L4.vs",
		"./shaders/fs/L4.fs");

	// -------------------------------------------------
	// 4.3 Create Floor
	// -------------------------------------------------

	vector<vec3> floorVertices = {
		vec3(-5.0f, -1.0f, -5.0f),
		vec3(5.0f, -1.0f, -5.0f),
		vec3(5.0f, -1.0f, 5.0f),
		vec3(-5.0f, -1.0f, 5.0f)};

	vector<vec2> floorTexCoords = {
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f)};

	PolygonWithTexture floor(floorVertices, floorTexCoords);

	// -------------------------------------------------
	// 4.4 Create Ceiling
	// -------------------------------------------------

	vector<vec3> roofVertices = {
		vec3(-5.0f, 4.0f, -5.0f),
		vec3(5.0f, 4.0f, -5.0f),
		vec3(5.0f, 4.0f, 5.0f),
		vec3(-5.0f, 4.0f, 5.0f)};
	vector<vec2> roofTexCoords = {
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f)};
	PolygonWithTexture roof(roofVertices, roofTexCoords);

	// -------------------------------------------------
	// 4.5 Create Back Wall
	// -------------------------------------------------

	vector<vec3> backWallVertices = {
		vec3(-5.0f, -1.0f, -5.0f),
		vec3(5.0f, -1.0f, -5.0f),
		vec3(5.0f, 4.0f, -5.0f),
		vec3(-5.0f, 4.0f, -5.0f)};

	vector<vec2> backWallTexCoords = {
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f)};

	PolygonWithTexture backWall(backWallVertices, backWallTexCoords);

	// -------------------------------------------------
	// 4.6 Create Right Wall
	// -------------------------------------------------

	vector<vec3> rightWallVertices = {
		vec3(5.0f, -1.0f, -5.0f),
		vec3(5.0f, -1.0f, 5.0f),
		vec3(5.0f, 4.0f, 5.0f),
		vec3(5.0f, 4.0f, -5.0f)};

	vector<vec2> rightWallTexCoords = {
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f)};

	PolygonWithTexture rightWall(rightWallVertices, rightWallTexCoords);

	// -------------------------------------------------
	// 4.7 Create Left Wall
	// -------------------------------------------------

	vector<vec3> leftWallVertices = {
		vec3(-5.0f, -1.0f, -5.0f),
		vec3(-5.0f, -1.0f, 5.0f),
		vec3(-5.0f, 4.0f, 5.0f),
		vec3(-5.0f, 4.0f, -5.0f)};

	vector<vec2> leftWallTexCoords = {
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f)};

	PolygonWithTexture leftWall(leftWallVertices, leftWallTexCoords);

	// -------------------------------------------------
	// 4.8 Create Front Wall
	// -------------------------------------------------

	vector<vec3> frontWallVertices = {
		vec3(-5.0f, -1.0f, 5.0f),
		vec3(5.0f, -1.0f, 5.0f),
		vec3(5.0f, 4.0f, 5.0f),
		vec3(-5.0f, 4.0f, 5.0f)};

	vector<vec2> frontWallTexCoords = {
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f)};

	PolygonWithTexture frontWall(frontWallVertices, frontWallTexCoords);
	// Table Top
	vector<vec3> tableTopVertices = {
		vec3(-2.0f, 0.5f, -1.5f),
		vec3(2.0f, 0.5f, -1.5f),
		vec3(2.0f, 0.5f, 1.5f),
		vec3(-2.0f, 0.5f, 1.5f)};

	Polygon tableTop(tableTopVertices, vec3(0.30f, 0.15f, 0.07f));
	// Tabletop thickness
	vector<vec3> tableBottomVertices = {
		vec3(-2.0f, 0.3f, -1.5f),
		vec3(2.0f, 0.3f, -1.5f),
		vec3(2.0f, 0.3f, 1.5f),
		vec3(-2.0f, 0.3f, 1.5f)};

	Polygon tableBottom(tableBottomVertices, vec3(0.30f, 0.15f, 0.07f));

	// Front side
	vector<vec3> tableFrontVertices = {
		vec3(-2.0f, 0.3f, -1.5f),
		vec3(2.0f, 0.3f, -1.5f),
		vec3(2.0f, 0.5f, -1.5f),
		vec3(-2.0f, 0.5f, -1.5f)};
	Polygon tableFront(tableFrontVertices, vec3(0.30f, 0.15f, 0.07f));

	// Back side
	vector<vec3> tableBackVertices = {
		vec3(-2.0f, 0.3f, 1.5f),
		vec3(2.0f, 0.3f, 1.5f),
		vec3(2.0f, 0.5f, 1.5f),
		vec3(-2.0f, 0.5f, 1.5f)};
	Polygon tableBack(tableBackVertices, vec3(0.30f, 0.15f, 0.07f));

	// Left side
	vector<vec3> tableLeftVertices = {
		vec3(-2.0f, 0.3f, -1.5f),
		vec3(-2.0f, 0.3f, 1.5f),
		vec3(-2.0f, 0.5f, 1.5f),
		vec3(-2.0f, 0.5f, -1.5f)};
	Polygon tableLeft(tableLeftVertices, vec3(0.30f, 0.15f, 0.07f));

	// Right side
	vector<vec3> tableRightVertices = {
		vec3(2.0f, 0.3f, -1.5f),
		vec3(2.0f, 0.3f, 1.5f),
		vec3(2.0f, 0.5f, 1.5f),
		vec3(2.0f, 0.5f, -1.5f)};
	Polygon tableRight(tableRightVertices, vec3(0.30f, 0.15f, 0.07f));

	// Table Legs

		// =====================================================
	// 5. Load Floor Texture
	// =====================================================

	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;

	unsigned char *data = stbi_load(
		"./textures/floor_wood.jpg",
		&width,
		&height,
		&nrChannels,
		0);

	if (data)
	{
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load floor texture" << std::endl;
	}

	stbi_image_free(data);
	// Load Ceiling Texture
	unsigned int ceilingTexture;

	glGenTextures(1, &ceilingTexture);
	glBindTexture(GL_TEXTURE_2D, ceilingTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char *ceilingData = stbi_load(
		"./textures/ceiling.jpg",
		&width,
		&height,
		&nrChannels,
		0);

	if (ceilingData)
	{
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			ceilingData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load ceiling texture" << std::endl;
	}

	stbi_image_free(ceilingData);
	floor.setTexture(texture);
	roof.setTexture(ceilingTexture);
	// =====================================================
	// 6. Load Wallpaper Texture
	// =====================================================

	unsigned int wallpaperTexture;
	glGenTextures(1, &wallpaperTexture);
	glBindTexture(GL_TEXTURE_2D, wallpaperTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char *wallpaperData = stbi_load(
		"./textures/wallpaper.jpg",
		&width,
		&height,
		&nrChannels,
		0);

	if (wallpaperData)
	{
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			wallpaperData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load wallpaper texture" << std::endl;
	}

	stbi_image_free(wallpaperData);

	// Apply the same wallpaper texture to all four walls

	backWall.setTexture(wallpaperTexture);
	rightWall.setTexture(wallpaperTexture);
	leftWall.setTexture(wallpaperTexture);
	frontWall.setTexture(wallpaperTexture);

	// =====================================================
	// 7. Projection Setup
	// =====================================================

	ourShader.use();

	mat4 projection = mat4(1.0f);

	projection = perspective(
		radians(45.0f),
		(float)SCR_WIDTH / (float)SCR_HEIGHT,
		0.1f,
		100.0f);

	ourShader.setMat4("projection", projection);

	// =====================================================
	// 8. Render Loop
	// =====================================================

	while (!glfwWindowShouldClose(window))
	{
		// Calculate frame time

		float currentFrame = static_cast<float>(glfwGetTime());

		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Handle keyboard input

		processInput(window);

		// Clear screen

		glClearColor(0.12f, 0.12f, 0.12f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update camera view

		mat4 view = camera.GetViewMatrix();

		ourShader.setMat4("view", view);

		// Draw room objects

		floor.draw(ourShader);

		roof.draw(ourShader);

		backWall.draw(ourShader);
		rightWall.draw(ourShader);
		leftWall.draw(ourShader);
		frontWall.draw(ourShader);
		tableTop.draw(ourShader);
		tableBottom.draw(ourShader);
		drawTableLeg(ourShader, -2.0f, -1.5f);
		drawTableLeg(ourShader, 1.8f, -1.5f);
		drawTableLeg(ourShader, -2.0f, 1.3f);
		drawTableLeg(ourShader, 1.8f, 1.3f);
		tableRight.draw(ourShader);
		tableLeft.draw(ourShader);
		tableBack.draw(ourShader);
		tableFront.draw(ourShader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// =====================================================
	// 9. Cleanup
	// =====================================================

	glfwTerminate();

	return 0;
}

// =====================================================
// 10. Keyboard Input
// =====================================================

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// =====================================================
// 11. Mouse Movement
// =====================================================

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// =====================================================
// 12. Window Resize
// =====================================================

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void drawTableLeg(Shader &shader, float x, float z)
{
	// 8 corners of the table leg
	vec3 p0(x, -1.0f, z);
	vec3 p1(x + 0.2f, -1.0f, z);
	vec3 p2(x + 0.2f, -1.0f, z + 0.2f);
	vec3 p3(x, -1.0f, z + 0.2f);

	vec3 p4(x, 0.5f, z);
	vec3 p5(x + 0.2f, 0.5f, z);
	vec3 p6(x + 0.2f, 0.5f, z + 0.2f);
	vec3 p7(x, 0.5f, z + 0.2f);
	// Front face
	vector<vec3> frontFace = {p0, p1, p5, p4};
	Polygon frontLeg(frontFace, vec3(0.30f, 0.15f, 0.07f));
	frontLeg.draw(shader);

	// Back face
	vector<vec3> backFace = {p3, p2, p6, p7};
	Polygon backLeg(backFace, vec3(0.30f, 0.15f, 0.07f));
	backLeg.draw(shader);
	// Right face
	vector<vec3> rightFace = {p1, p2, p6, p5};
	Polygon rightLeg(rightFace, vec3(0.30f, 0.15f, 0.07f));
	rightLeg.draw(shader);

	// Left face
	vector<vec3> leftFace = {p0, p3, p7, p4};
	Polygon leftLeg(leftFace, vec3(0.30f, 0.15f, 0.07f));
	leftLeg.draw(shader);
}