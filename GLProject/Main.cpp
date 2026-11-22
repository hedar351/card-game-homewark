
#include "libs/glad_loader_docs.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

#include "PolygonWithTexture.cpp"
#include "Polygon.cpp"
#include <random>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include "stb_image.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace glm;

// =====================================================
// 1. Window Settings
// =====================================================

int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;

// =====================================================
// 2. Camera Settings
// =====================================================

// Camera camera(vec3(0.0f, 2.5f, 6.5f));

Camera camera(
    vec3(0.0555789f, 1.36426f, 2.23016f),
    vec3(0.0f, 1.0f, 0.0f),
    -89.7999f,
    -23.0f
);

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int cameraMode = 2;

bool cameraControlEnabled = false;

bool oneKeyWasPressed = false;
bool twoKeyWasPressed = false;

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
bool gKeyPressed = false;

void updateCardPosition(
    PolygonWithTexture& card,
    glm::vec3 position,
    float rotationAngle
)
{
    glm::mat4 cardModel = glm::mat4(1.0f);

    cardModel = glm::translate(cardModel, position);

    cardModel = glm::rotate(
        cardModel,
        glm::radians(180.0f),
        glm::vec3(0, 1, 0)
    );

    cardModel = glm::rotate(
        cardModel,
        glm::radians(rotationAngle),
        glm::vec3(1, 0, 0)
    );

    card.transformation(cardModel);
}


int main()
{
	// -------------------------------------------------
	// 4.1 Initialize GLFW and Create Window
	// -------------------------------------------------
	bool gKeyWasPressed = false;
	bool verticalMode = true;
	bool vKeyWasPressed = false;
	bool cKeyWasPressed = false;

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);

	stbi_set_flip_vertically_on_load(true);

	// -------------------------------------------------
	// 4.2 Load Shader
	// -------------------------------------------------

	Shader ourShader(
		"./shaders/vs/L4.vs",
		"./shaders/fs/L4.fs");
	Model manaCrystal("../GLProject/models/mana-crystal-nodes.glb");
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

	// =====================================================
	// Shield Decoration
	// =====================================================

	// Outer shield shape
	vector<vec3> shieldOuterVertices = {
		vec3(-0.65f, 3.2f, -4.80f),
		vec3(0.65f, 3.2f, -4.80f),
		vec3(0.70f, 2.35f, -4.80f),
		vec3(0.00f, 1.55f, -4.80f),
		vec3(-0.70f, 2.35f, -4.80f)};

	// Outer Shield Texture Coordinates
	vector<vec2> shieldOuterTexCoords = {
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.5f),
		vec2(0.5f, 0.0f),
		vec2(0.0f, 0.5f)};

	PolygonWithTexture shieldOuterTextured(
		shieldOuterVertices,
		shieldOuterTexCoords);

	// Inner shield shape
	vector<vec3> shieldInnerVertices = {
		vec3(-0.50f, 3.05f, -4.78f),
		vec3(0.50f, 3.05f, -4.78f),
		vec3(0.54f, 2.35f, -4.78f),
		vec3(0.00f, 1.78f, -4.78f),
		vec3(-0.54f, 2.35f, -4.78f)};

	vector<vec2> shieldInnerTexCoords = {
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.5f),
		vec2(0.5f, 0.0f),
		vec2(0.0f, 0.5f)};

	PolygonWithTexture shieldInnerTextured(
		shieldInnerVertices,
		shieldInnerTexCoords);

	std::vector<vec3> posterVertices = {
		vec3(-4.2f, 3.3f, -4.95f), // Top left
		vec3(-2.0f, 3.3f, -4.95f), // Top right

		// Torn right edge
		vec3(-2.12f, 2.95f, -4.95f),
		vec3(-1.92f, 2.72f, -4.95f),
		vec3(-2.15f, 2.45f, -4.95f),
		vec3(-1.95f, 2.15f, -4.95f),

		// Torn bottom edge
		vec3(-2.0f, 1.2f, -4.95f),
		vec3(-2.4f, 1.32f, -4.95f),
		vec3(-2.75f, 1.13f, -4.95f),
		vec3(-3.1f, 1.29f, -4.95f),
		vec3(-3.45f, 1.10f, -4.95f),
		vec3(-3.8f, 1.27f, -4.95f),
		vec3(-4.2f, 1.2f, -4.95f)};
	std::vector<vec2> posterTexCoords = {
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f),

		// Right edge
		vec2(1.0f, 0.83f),
		vec2(1.0f, 0.72f),
		vec2(1.0f, 0.60f),
		vec2(1.0f, 0.45f),

		// Bottom edge
		vec2(1.0f, 0.0f),
		vec2(0.82f, 0.05f),
		vec2(0.66f, 0.0f),
		vec2(0.50f, 0.04f),
		vec2(0.34f, 0.0f),
		vec2(0.18f, 0.03f),
		vec2(0.0f, 0.0f)};
	PolygonWithTexture poster(posterVertices, posterTexCoords);

	// Table Top
	vector<vec3> tableTopVertices = {
		vec3(-2.0f, 0.5f, -1.5f),
		vec3(2.0f, 0.5f, -1.5f),
		vec3(2.0f, 0.5f, 1.5f),
		vec3(-2.0f, 0.5f, 1.5f)};

	vector<vec2> tableTopTexCoords = {
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f)};

	PolygonWithTexture tableTop(tableTopVertices, tableTopTexCoords); // Tabletop thickness
	vector<vec3> tableBottomVertices = {
		vec3(-2.0f, 0.3f, -1.5f),
		vec3(2.0f, 0.3f, -1.5f),
		vec3(2.0f, 0.3f, 1.5f),
		vec3(-2.0f, 0.3f, 1.5f)};

	// vector<vec3> cardVertices = {
	// 	vec3(-0.15f, 0.51f, -0.20f),
	// 	vec3( 0.15f, 0.51f, -0.20f),
	// 	vec3( 0.15f, 0.51f,  0.20f),
	// 	vec3(-0.15f, 0.51f,  0.20f)
	// };

	// vector<vec2> cardTexCoords = {
	// 	vec2(1.0f, 0.0f),
	// 	vec2(0.0f, 0.0f),
	// 	vec2(0.0f, 1.0f),
	// 	vec2(1.0f, 1.0f)
	// };

	// PolygonWithTexture firstCard(cardVertices, cardTexCoords);

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

	// Load Shield Texture
	unsigned int shieldTexture;

	glGenTextures(1, &shieldTexture);
	glBindTexture(GL_TEXTURE_2D, shieldTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char *shieldData = stbi_load(
		"./textures/shield.jpg",
		&width,
		&height,
		&nrChannels,
		0);

	if (shieldData)
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
			shieldData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load shield texture" << std::endl;
	}

	stbi_image_free(shieldData);

	shieldInnerTextured.setTexture(shieldTexture);
	// Load Shield Frame Texture
	unsigned int shieldFrameTexture;

	glGenTextures(1, &shieldFrameTexture);
	glBindTexture(GL_TEXTURE_2D, shieldFrameTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char *shieldFrameData = stbi_load(
		"./textures/shield_frame.jpg",
		&width,
		&height,
		&nrChannels,
		0);

	if (shieldFrameData)
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
			shieldFrameData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load shield frame texture" << std::endl;
	}

	stbi_image_free(shieldFrameData);

	// Load Poster Texture
	unsigned int posterTexture;

	glGenTextures(1, &posterTexture);
	glBindTexture(GL_TEXTURE_2D, posterTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char *posterData = stbi_load("../GLProject/textures/poster.jpg", &width, &height, &nrChannels, 0);

	if (posterData)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			posterData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load poster texture" << std::endl;
	}

	stbi_image_free(posterData);

	shieldOuterTextured.setTexture(shieldFrameTexture);
	poster.setTexture(posterTexture);

	// Load Table Wood Texture
	unsigned int tableWoodTexture;

	glGenTextures(1, &tableWoodTexture);
	glBindTexture(GL_TEXTURE_2D, tableWoodTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char *tableWoodData = stbi_load(
		"./textures/table_wood.jpg",
		&width,
		&height,
		&nrChannels,
		0);

	if (tableWoodData)
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
			tableWoodData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load table wood texture" << std::endl;
	}

	stbi_image_free(tableWoodData);

	tableTop.setTexture(tableWoodTexture);
		// Load Card Texture
// Card textures
vector<string> cardTexturePaths = {
    "./textures/knight.jpg",
    "./textures/hornt.jpg",
    "./textures/grim.jpg",
    "./textures/quirrel.jpg",
    "./textures/lords.jpg"
};

vector<GLuint> cardTextures(5);

glGenTextures(5, cardTextures.data());

for (int i = 0; i < 5; i++)
{
    glBindTexture(GL_TEXTURE_2D, cardTextures[i]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* cardData = stbi_load(
        cardTexturePaths[i].c_str(),
        &width,
        &height,
        &nrChannels,
        0
    );

    if (cardData)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            cardData
        );

        glGenerateMipmap(GL_TEXTURE_2D);

        cout << "Card texture " << i + 1 << " loaded!" << endl;
    }
    else
    {
        cout << "Failed to load: " << cardTexturePaths[i] << endl;
    }

    stbi_image_free(cardData);
}

	// firstCard.setTexture(cardTexture);
	// Card vertices
	vector<vec3> cardVertices = {
		vec3(-0.15f, 0.0f, -0.20f),
		vec3( 0.15f, 0.0f, -0.20f),
		vec3( 0.15f, 0.0f,  0.20f),
		vec3(-0.15f, 0.0f,  0.20f)
	};

// Card texture coordinates
	vector<vec2> cardTexCoords = {
		vec2(1.0f, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f)
	};

	// Create five cards

// Create five cards
	vector<int> availableCards = { 0, 1, 2, 3, 4 };

	random_device rd;
	mt19937 generator(rd());

	shuffle(availableCards.begin(), availableCards.end(), generator);

	vector<int> playedCards;

	vector<PolygonWithTexture> cards;

	vector<glm::vec3> cardPositions(5);
	vector<glm::vec3> targetPositions(5);

	vector<float> cardAngles(5, -90.0f);
	vector<float> targetAngles(5, -90.0f);
	cards.reserve(5);

	// First: Create all cards and assign textures
	for (int i = 0; i < 5; i++)
	{
		cards.emplace_back(cardVertices, cardTexCoords);
		cards[i].setTexture(cardTextures[i]);
	}

	for (int i = 0; i < 5; i++)
	{
		float offset = i * 0.008f;

		cardPositions[i] = glm::vec3(
			-0.05f + i * 0.025f,
			0.70f + offset,
			0.35f - offset
		);

		targetPositions[i] = cardPositions[i];
	}

	
	for (int i = 0; i < 5; i++)
	{
		int cardIndex = availableCards[i];

		float offset = i * 0.025f;

		// float xPosition = verticalMode
		// 	? 0.05f - offset
		// 	: -0.05f + offset;
		float xPosition = -0.05f + offset;

		float yPosition = verticalMode
			? 0.82f + (i * 0.008f)
			: 0.51f + (i * 0.008f);

		float zPosition = verticalMode
			? 0.35f + offset
			: 0.35f - offset;

		cardPositions[cardIndex] = glm::vec3(
			xPosition,
			yPosition,
			zPosition
		);

		targetPositions[cardIndex] = cardPositions[cardIndex];

		cardAngles[cardIndex] = verticalMode ? -90.0f : 0.0f;
		targetAngles[cardIndex] = cardAngles[cardIndex];

		updateCardPosition(
			cards[cardIndex],
			cardPositions[cardIndex],
			cardAngles[cardIndex]
		);
	}



	// Plant Pot - Four Side Faces
	glm::vec3 potColor(0.55f, 0.22f, 0.10f);

	// Front face
	vector<vec3> potFrontVertices = {
		vec3(4.2f, -1.0f, -4.2f),
		vec3(4.8f, -1.0f, -4.2f),
		vec3(4.72f, -0.35f, -4.2f),
		vec3(4.28f, -0.35f, -4.2f)};
	Polygon potFront(potFrontVertices, potColor);

	// Back face
	vector<vec3> potBackVertices = {
		vec3(4.8f, -1.0f, -4.8f),
		vec3(4.2f, -1.0f, -4.8f),
		vec3(4.28f, -0.35f, -4.8f),
		vec3(4.72f, -0.35f, -4.8f)};
	Polygon potBack(potBackVertices, potColor);

	// Left face
	vector<vec3> potLeftVertices = {
		vec3(4.2f, -1.0f, -4.8f),
		vec3(4.2f, -1.0f, -4.2f),
		vec3(4.28f, -0.35f, -4.2f),
		vec3(4.28f, -0.35f, -4.8f)};
	Polygon potLeft(potLeftVertices, potColor);

	// Right face
	vector<vec3> potRightVertices = {
		vec3(4.8f, -1.0f, -4.2f),
		vec3(4.8f, -1.0f, -4.8f),
		vec3(4.72f, -0.35f, -4.8f),
		vec3(4.72f, -0.35f, -4.2f)};
	Polygon potRight(potRightVertices, potColor);
	// Plant Soil
	vector<vec3> soilVertices = {
		vec3(4.28f, -0.34f, -4.28f),
		vec3(4.72f, -0.34f, -4.28f),
		vec3(4.72f, -0.34f, -4.72f),
		vec3(4.28f, -0.34f, -4.72f)};

	Polygon soil(soilVertices, vec3(0.16f, 0.08f, 0.035f));
	// Plant Stem
	glm::vec3 stemColor(0.35f, 0.85f, 0.18f);
	vector<vec3> stemVertices = {
		vec3(4.48f, -0.34f, -4.50f),
		vec3(4.54f, -0.34f, -4.50f),
		vec3(4.54f, 0.65f, -4.50f),
		vec3(4.48f, 0.65f, -4.50f)};

	Polygon stem(stemVertices, stemColor);
	// Plant Leaves
	glm::vec3 leafColor(0.02f, 0.75f, 0.18f);

	// Left Leaf
	vector<vec3> leftLeafVertices = {
		vec3(4.49f, 0.18f, -4.49f),
		vec3(4.22f, 0.22f, -4.49f),
		vec3(4.02f, 0.52f, -4.49f),
		vec3(4.34f, 0.43f, -4.49f)};
	Polygon leftLeaf(leftLeafVertices, leafColor);

	// Right Leaf
	vector<vec3> rightLeafVertices = {
		vec3(4.53f, 0.32f, -4.49f),
		vec3(4.75f, 0.35f, -4.49f),
		vec3(4.98f, 0.68f, -4.49f),
		vec3(4.70f, 0.55f, -4.49f)};
	Polygon rightLeaf(rightLeafVertices, leafColor);
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

	// vector<int> availableCards = { 0, 1, 2, 3, 4 };
	
	
	while (!glfwWindowShouldClose(window))
	{
		float cardY = verticalMode ? 0.70f : 0.51f;
		// Calculate frame time
		bool cPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;

		if (cPressed && !cKeyWasPressed)
		{
			cameraControlEnabled = !cameraControlEnabled;

			if (cameraControlEnabled)
			{
				// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				cout << "Camera Control: ON" << endl;
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				cout << "Camera Control: OFF" << endl;
			}
		}

		if (cPressed && !cKeyWasPressed)
		{
			firstMouse = true;
		}

		bool onePressed = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
		bool twoPressed = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
		bool cameraChanged = false;

// Camera 1: Top View
if (onePressed && !oneKeyWasPressed)
{
    cameraMode = 1;

	verticalMode = false;
	cameraChanged = true;

    camera = Camera(
        vec3(0.0f, 3.5f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        -90.0f,
        -89.9f
    );

    cameraControlEnabled = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    firstMouse = true;

    cout << "Camera 1: Top View" << endl;
}

// Camera 2: Player View
if (twoPressed && !twoKeyWasPressed)
{
    cameraMode = 2;
	verticalMode = true;
	cameraChanged = true;

    camera = Camera(
        vec3(0.0555789f, 1.36426f, 2.23016f),
        vec3(0.0f, 1.0f, 0.0f),
        -89.7999f,
        -23.0f
    );

    cameraControlEnabled = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    firstMouse = true;

    cout << "Camera 2: Player View" << endl;
}

		oneKeyWasPressed = onePressed;
		twoKeyWasPressed = twoPressed;

		if (cameraChanged)
		{
			// Update cards still in the deck
			for (int i = 0; i < availableCards.size(); i++)
			{
				int cardIndex = availableCards[i];

				float offset = i * 0.025f;

				targetPositions[cardIndex] = glm::vec3(
					-0.05f + offset,
					verticalMode ? 0.82f + (i * 0.008f) : 0.51f + (i * 0.008f),
					verticalMode ? 0.35f + offset : 0.35f - offset
				);
				targetAngles[cardIndex] = verticalMode ? -90.0f : 0.0f;
			}

			// Update cards already played
			for (int i = 0; i < playedCards.size(); i++)
			{
				int cardIndex = playedCards[i];

				targetPositions[cardIndex] = glm::vec3(
					-0.84f + (i * 0.42f),
					verticalMode ? 0.82f : 0.51f,
					0.9f
				);

				targetAngles[cardIndex] = verticalMode ? -90.0f : 0.0f;
			}
		}

		bool gPressed = glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;

		if (gPressed && !gKeyWasPressed && !availableCards.empty())
		{
			// Take the next card from the shuffled deck
			int cardIndex = availableCards.back();
			availableCards.pop_back();

			playedCards.push_back(cardIndex);

			// Position of the played card
			int playedPosition = playedCards.size() - 1;

			targetPositions[cardIndex] = glm::vec3(
				-0.84f + (playedPosition * 0.42f),
				verticalMode ? 0.82f : 0.51f,
				0.9f
			);

			targetAngles[cardIndex] = verticalMode ? -90.0f : 0.0f;

			cout << "Played card: " << cardIndex << endl;
		}

		gKeyWasPressed = gPressed;

		
		// bool vPressed = glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS;

		// if (vPressed && !vKeyWasPressed)
		// {
		// 	verticalMode = !verticalMode;

		// 	// Update cards still in the deck
		// 	for (int i = 0; i < availableCards.size(); i++)
		// 	{
		// 		int cardIndex = availableCards[i];

		// 		float offset = i * 0.025f;

		// 		float xPosition = -0.05f + offset;
		// 		float yPosition = verticalMode
		// 			? 0.70f + (i * 0.008f)
		// 			: 0.51f + (i * 0.008f);
		// 		float zPosition = 0.05f - offset;

		// 		updateCardPosition(
		// 			cards[cardIndex],
		// 			xPosition,
		// 			yPosition,
		// 			zPosition,
		// 			verticalMode
		// 		);
		// 	}

		// 	// Update cards already played
		// 	for (int i = 0; i < playedCards.size(); i++)
		// 	{
		// 		int cardIndex = playedCards[i];

		// 		float xPosition = -0.84f + (i * 0.42f);

		// 		updateCardPosition(
		// 			cards[cardIndex],
		// 			xPosition,
		// 			verticalMode ? 0.70f : 0.51f,
		// 			0.9f,
		// 			verticalMode
		// 		);
		// 	}

		// 	cout << "Vertical Mode: " << verticalMode << endl;
		// }

		// vKeyWasPressed = vPressed;


		float currentFrame = static_cast<float>(glfwGetTime());

		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float animationSpeed = 5.0f;

		float animationStep = glm::min(
			animationSpeed * deltaTime,
			1.0f
		);

		for (int i = 0; i < 5; i++)
		{
			cardPositions[i] = glm::mix(
				cardPositions[i],
				targetPositions[i],
				animationStep
			);

			cardAngles[i] = glm::mix(
				cardAngles[i],
				targetAngles[i],
				animationStep
			);

			updateCardPosition(
				cards[i],
				cardPositions[i],
				cardAngles[i]
			);
		}

		// Handle keyboard input

		if (cameraControlEnabled)
		{
			processInput(window);
		}

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
		glActiveTexture(GL_TEXTURE0);
		ourShader.setInt("texture_diffuse1", 0);

		// firstCard.draw(ourShader);
		for (int i = 0; i < cards.size(); i++)
		{
			cards[i].draw(ourShader);
		}
		tableBottom.draw(ourShader);
		potFront.draw(ourShader);
		potBack.draw(ourShader);
		potLeft.draw(ourShader);
		potRight.draw(ourShader);
		soil.draw(ourShader);
		stem.draw(ourShader);
		leftLeaf.draw(ourShader);
		rightLeaf.draw(ourShader);
		drawTableLeg(ourShader, -2.0f, -1.5f);
		drawTableLeg(ourShader, 1.8f, -1.5f);
		drawTableLeg(ourShader, -2.0f, 1.3f);
		drawTableLeg(ourShader, 1.8f, 1.3f);
		tableRight.draw(ourShader);
		tableLeft.draw(ourShader);
		tableBack.draw(ourShader);
		tableFront.draw(ourShader);
		shieldOuterTextured.draw(ourShader);
		shieldInnerTextured.draw(ourShader);
		poster.draw(ourShader);
		glm::mat4 crystalModel = glm::mat4(1.0f);

		crystalModel = glm::translate(
			crystalModel,
			glm::vec3(-1.5f, 0.5f, -1.0f));

		crystalModel = glm::scale(
			crystalModel,
			glm::vec3(0.1f, 0.1f, 0.1f));

		ourShader.setMat4("model", crystalModel);
		manaCrystal.Draw(ourShader);

		ourShader.setMat4("model", glm::mat4(1.0f));
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
    if (!cameraControlEnabled)
        return;

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