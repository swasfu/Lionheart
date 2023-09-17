#include "worldgen/World.h"
#include "worldgen/components/ModelComponent.h"

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

	float worldSize = 10.0f;

	float cameraDepth = 12.0f;
	float cameraMoveSpeed = 0.005f * worldSize;
	float cameraZoomSpeed = 0.01f * worldSize;
	float fov = 75.0f;

	GLCamera camera(windowWidth, windowHeight);

	shaderProgram.Use();

	glm::vec4 lightColour = glm::vec4(1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

	glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w);
	glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	World::GenerateTiles(&registry, worldSize, 50);

	while (!glfwWindowShouldClose(window))
	{
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

		camera.position += -glm::normalize(camera.orientation) * cameraDepth;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);

		shaderProgram.Use();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(camera.ViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "projection"), 1, GL_FALSE, glm::value_ptr(camera.ProjectionMatrix(fov, 0.001f, 100.0f)));
		glUniform3f(glGetUniformLocation(shaderProgram.id, "cameraPos"), camera.position.x, camera.position.y, camera.position.z);

		auto modelIDs = registry.ViewIDs<ModelComponent>();
		std::cout << "test" << std::endl;
		for (auto modelID : modelIDs)
		{
			auto modelPtr = registry.GetComponent<ModelComponent>(modelID);
			auto& model = modelPtr->model;

			model.mesh.vao.Bind();

			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(model.position);

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "translation"), 1, GL_FALSE, glm::value_ptr(model.TranslationMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "rotation"), 1, GL_FALSE, glm::value_ptr(model.RotationMatrix()));

			glDrawElements(GL_TRIANGLES, model.mesh.indices.size(), GL_UNSIGNED_INT, 0);

			model.rotation.y += 0.001f;

			camera.position = model.position;

			std::cout << "(" << model.position.x << ", " << model.position.y << ", " << model.position.z << ")" << std::endl;
			std::cout << "(" << model.rotation.x << ", " << model.rotation.y << ", " << model.rotation.z << ")" << std::endl;
		}
		std::cout << "(" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << ")" << std::endl;

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}