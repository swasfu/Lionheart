#include "World.h"
#include "components/ModelComponent.h"

#include "lionheart/graphics/GLShaderProgram.h"
#include "lionheart/graphics/GLVBO.h"
#include "lionheart/graphics/GLVAO.h"
#include "lionheart/graphics/GLEBO.h"
#include "lionheart/graphics/GLTexture.h"
#include "lionheart/graphics/GLCamera.h"
#include "lionheart/ecs/registry/Registry.h"

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

	srand(time(nullptr));

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

	GLShaderProgram shaderProgram("data/shaders/defaultVertexShader.glsl", "data/shaders/basicFragmentShader.glsl");
	shaderProgram.Use();

	float cameraDepth = 2.0f;
	float cameraMoveSpeed = 0.01f;
	float cameraZoomSpeed = 0.01f;
	float fov = 5.0f + cameraDepth * 15.0f;

	GLCamera camera(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, cameraDepth));

	World::GenerateTiles(&registry, 1.0f, 200);

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.position += glm::normalize(camera.up) * cameraMoveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.position -= glm::normalize(camera.up) * cameraMoveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.position += glm::normalize(glm::cross(camera.orientation, camera.up)) * cameraMoveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.position -= glm::normalize(glm::cross(camera.orientation, camera.up)) * cameraMoveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			cameraDepth += cameraZoomSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			cameraDepth -= cameraZoomSpeed;
		}

		fov = 15.0f + cameraDepth * 15.0f;
		camera.position = glm::normalize(camera.position) * cameraDepth;
		camera.orientation = -camera.position;

		/*
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			double mouseX;
			double mouseY;

			glfwGetCursorPos(window, &mouseX, &mouseY);

			float rotX = camera.sensitivity * (float)(mouseY - (camera.width / 2)) / camera.width;
			float rotY = camera.sensitivity * (float)(mouseX - (camera.height / 2)) / camera.height;

			glm::vec3 newOrientation = glm::rotate(camera.orientation, glm::radians(-rotX), glm::normalize(glm::cross(camera.orientation, camera.up)));

			if (abs(glm::angle(newOrientation, camera.up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				camera.orientation = newOrientation;
			}

			camera.orientation = glm::rotate(camera.orientation, glm::radians(-rotY), camera.up);
		}
		*/

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);

		auto modelIDs = registry.ViewIDs<ModelComponent>();

		for (auto modelID : modelIDs)
		{
			shaderProgram.Use();

			auto modelPtr = registry.GetComponent<ModelComponent>(modelID);
			auto& model = modelPtr->model;

			model.mesh.vao.Bind();

			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(model.position);

			GLint modelLocation = glGetUniformLocation(shaderProgram.id, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			GLint viewLocation = glGetUniformLocation(shaderProgram.id, "view");
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.ViewMatrix()));
			GLint projLocation = glGetUniformLocation(shaderProgram.id, "proj");
			glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(camera.ProjectionMatrix(fov, 0.1f, 100.0f)));

			glDrawElements(GL_TRIANGLES, model.mesh.indices.size(), GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}