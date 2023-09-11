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

int windowWidth = 800;
int windowHeight = 800;

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

	GLShaderProgram shaderProgram("data/shaders/default.vert", "data/shaders/default.frag");
	shaderProgram.Use();

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	GLCamera camera(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));

	GLVertex testRightVertex;
	testRightVertex.position = glm::vec3(1.0f, 0.0f, 0.0f);
	testRightVertex.colour = glm::vec3(1.0f);
	GLVertex testTopVertex;
	testTopVertex.position = glm::vec3(0.0f, 1.0f, 0.0f);
	testTopVertex.colour = glm::vec3(1.0f);
	GLVertex testLeftVertex;
	testLeftVertex.position = glm::vec3(-1.0f, 0.0f, 0.0f);
	testLeftVertex.colour = glm::vec3(1.0f);
	std::vector<GLVertex> testVertices = { testRightVertex, testTopVertex, testLeftVertex };
	std::vector<GLuint> testIndices = { 0, 1, 2 };
	GLTexture testTexture;

	/*
	GLMesh testMesh(testVertices, testIndices, testTexture);
	std::cout << "kms" << std::endl;
	std::cout << testMesh.vao.id << std::endl;

	GLModel testModel = GLModel(testMesh);
	std::cout << "kms3" << std::endl;
	std::cout << testModel.mesh.vao.id << std::endl;
	*/

	World::GenerateTiles(&registry, 1.0f, 1);

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.position += glm::normalize(camera.orientation) * camera.speed;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.position -= glm::normalize(camera.orientation) * camera.speed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.position += glm::normalize(glm::cross(camera.orientation, camera.up)) * camera.speed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.position -= glm::normalize(glm::cross(camera.orientation, camera.up)) * camera.speed;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			camera.position += glm::normalize(camera.up) * camera.speed;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			camera.position -= glm::normalize(camera.up) * camera.speed;
		}

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

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Use();
		//testModel.mesh.vao.Bind();
		GLVBO testVBO(testVertices);
		testVBO.Bind();
		GLEBO testEBO(testIndices);
		testEBO.Bind();
		GLVAO testVAO;
		//testVAO.Bind();

		testVAO.LinkAttrib(testVBO, 0, 3, GL_FLOAT, sizeof(GLVertex), (void*)0);
		testVAO.LinkAttrib(testVBO, 1, 3, GL_FLOAT, sizeof(GLVertex), (void*)(3 * sizeof(float)));
		testVAO.LinkAttrib(testVBO, 2, 3, GL_FLOAT, sizeof(GLVertex), (void*)(6 * sizeof(float)));
		testVAO.LinkAttrib(testVBO, 3, 2, GL_FLOAT, sizeof(GLVertex), (void*)(9 * sizeof(float)));

		glm::mat4 testModelMatrix = glm::mat4(1.0f);
		//testModelMatrix = glm::translate(testModel.position);
		glm::mat4 testViewMatrix = glm::mat4(1.0f);
		glm::mat4 testProjMatrix = glm::mat4(1.0f);

		GLint testModelLocation = glGetUniformLocation(shaderProgram.id, "m");
		glUniformMatrix4fv(testModelLocation, 1, GL_FALSE, glm::value_ptr(testModelMatrix));
		GLint testViewLocation = glGetUniformLocation(shaderProgram.id, "v");
		glUniformMatrix4fv(testViewLocation, 1, GL_FALSE, glm::value_ptr(testViewMatrix));
		GLint testProjLocation = glGetUniformLocation(shaderProgram.id, "p");
		glUniformMatrix4fv(testProjLocation, 1, GL_FALSE, glm::value_ptr(testProjMatrix));
		/*
		testModel.mesh.texture.Uniform(shaderProgram, "tex0", 0);
		testModel.mesh.texture.Bind();

		for (auto& index : testModel.mesh.indices)
		{
			std::cout << "Index: " << index << ", vertex: (" << testModel.mesh.vertices[index].position.x << ", " << testModel.mesh.vertices[index].position.y << ", " << testModel.mesh.vertices[index].position.z << ")" << std::endl;
		}
		*/
		glDrawElements(GL_TRIANGLES, testIndices.size(), GL_UNSIGNED_INT, 0);

		/*
		auto modelIDs = registry.ViewIDs<ModelComponent>();

		for (auto modelID : modelIDs)
		{
			shaderProgram.Use();

			auto modelPtr = registry.GetComponent<ModelComponent>(modelID);
			auto& model = modelPtr->model;
			model.mesh.vao.Bind();

			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(model.position);

			GLint modelLocation = glGetUniformLocation(shaderProgram.id, "m");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			GLint viewLocation = glGetUniformLocation(shaderProgram.id, "v");
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.ViewMatrix()));
			GLint projLocation = glGetUniformLocation(shaderProgram.id, "p");
			glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(camera.ProjectionMatrix(45.0f, 0.1f, 100.0f)));

			model.mesh.texture.Uniform(shaderProgram, "tex0", 0);
			model.mesh.texture.Bind();

			glDrawElements(GL_TRIANGLES, model.mesh.indices.size(), GL_UNSIGNED_INT, 0);
		}
		*/
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}