#include "worldgen/World.h"
#include "worldgen/components/ModelComponent.h"
#include "worldgen/components/CelestialBodyComponent.h"
#include "worldgen/Fractal.h"
#include "math/Constants.h"

#include "lionheart/graphics/GLShaderProgram.h"
#include "lionheart/graphics/GLVBO.h"
#include "lionheart/graphics/GLVAO.h"
#include "lionheart/graphics/GLEBO.h"
#include "lionheart/graphics/GLTexture.h"
#include "lionheart/graphics/GLCamera.h"
#include "lionheart/ecs/Registry.h"

#include <thirdparty/glad/gl.h>
#include <thirdparty/glfw/glfw3.h>
#include <thirdparty/stb/stb_image.h>
#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtc/matrix_transform.hpp>
#include <thirdparty/glm/gtx/rotate_vector.hpp>
#include <thirdparty/glm/gtc/type_ptr.hpp>
#include <thirdparty/glm/gtx/vector_angle.hpp>

#include <iostream>

int windowWidth = 1920;
int windowHeight = 1080;

void DeltaTime(double time, float& last, float& delta)
{
	delta = (float)time - last;
	last = (float)time;
}

int main(void)
{
	Registry registry;

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Lionheart", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGL(glfwGetProcAddress);

	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	GLShaderProgram shaderProgram("data/shaders/defaultVertexShader.glsl", "data/shaders/defaultFragmentShader.glsl");

	float worldSize = 6.371f;

	float cameraDepth = 2 * worldSize;
	float cameraMoveSpeed = 0.005f * worldSize;
	float cameraZoomSpeed = 0.01f * worldSize;
	float fov = 75.0f;

	GLCamera camera(windowWidth, windowHeight);

	shaderProgram.Use();

	glm::vec4 lightColour = glm::vec4(1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

	glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w);
	glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	/*ValueMap test;
	int testSize = pow(2, 5);
	CreateFractal(test, testSize, 1.0f, 1.55f);
	std::cout << "Width: " << test.width << std::endl;
	std::cout << "Height: " << test.height << std::endl;
	std::cout << "X-Y" << std::endl;
	for (int y = 0; y < testSize / 2; y++)
	{
		for (int x = 0; x < testSize; x++)
		{
			std::cout << (int)(test.values[x * test.height + y] * 9.f) << " ";
		}
		std::cout << std::endl;
	}


	std::cout << "LAT-LON" << std::endl;
	for (float y = -Constants::HALF_PI; y < Constants::HALF_PI; y += (Constants::PI / (float)(testSize / 2)))
	{
		for (float x = -Constants::PI; x < Constants::PI; x += (Constants::TWO_PI / (float)testSize))
		{
			std::cout << (int)(test.Value(y, x) * 9.0f) << " ";
		}
		std::cout << std::endl;
	}*/

	GenerateWorld(&registry, worldSize, 50, powf(2, 10));

	float last = 0.0f;
	float delta = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		DeltaTime(glfwGetTime(), last, delta);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.orientation -= glm::normalize(camera.up) * cameraMoveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.orientation += glm::normalize(camera.up) * cameraMoveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.orientation -= glm::normalize(glm::cross(camera.orientation, camera.up)) * cameraMoveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.orientation += glm::normalize(glm::cross(camera.orientation, camera.up)) * cameraMoveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			cameraDepth += cameraZoomSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			cameraDepth -= cameraZoomSpeed;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);

		shaderProgram.Use();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(camera.ViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "projection"), 1, GL_FALSE, glm::value_ptr(camera.ProjectionMatrix(fov, 0.001f, 100.0f)));

		auto bodyIDs = registry.ViewIDs<CelestialBodyComponent>();

		for (auto bodyID : bodyIDs)
		{
			auto body = registry.GetComponent<CelestialBodyComponent>(bodyID);

			body->rotation *= glm::angleAxis(body->rotationSpeed * delta, glm::vec3(0.0f, 0.0f, 1.0f));

			camera.position = body->position;
			camera.position += -glm::normalize(camera.orientation) * cameraDepth;

			auto bodyModel = registry.GetComponent<ModelComponent>(body->bodyModelID);
			bodyModel->model.floatingPosition = body->position;
			bodyModel->model.rotation = body->rotation;
		}

		auto modelIDs = registry.ViewIDs<ModelComponent>();

		for (auto modelID : modelIDs)
		{
			auto modelPtr = registry.GetComponent<ModelComponent>(modelID);
			auto& model = modelPtr->model;

			model.position = model.floatingPosition - camera.position;

			model.mesh.vao.Bind();

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "translation"), 1, GL_FALSE, glm::value_ptr(model.TranslationMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "rotation"), 1, GL_FALSE, glm::value_ptr(model.RotationMatrix()));

			std::cout << model.position.x << ", " << model.position.y << ", " << model.position.z << std::endl;

			glDrawElements(GL_TRIANGLES, model.mesh.indices.size(), GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}