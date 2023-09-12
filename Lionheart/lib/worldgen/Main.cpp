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

	glEnable(GL_DEPTH_TEST);

	GLShaderProgram shaderProgram("data/shaders/defaultVertexShader.glsl", "data/shaders/basicFragmentShader.glsl");
	shaderProgram.Use();

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

	GLMesh testMesh(testVertices, testIndices, testTexture);

	GLModel testModel = GLModel(testMesh);

	World::GenerateTiles(&registry, 1.0f, 20);

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

		/*
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, testVertices.size() * sizeof(GLVertex), testVertices.data(), GL_STATIC_DRAW);

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, testIndices.size() * sizeof(GLuint), testIndices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (void*)(1 * sizeof(glm::vec3)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (void*)(2 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		shaderProgram.Use();

		glm::mat4 testModelMatrix = glm::mat4(1.0f);
		glm::mat4 testViewMatrix = glm::mat4(1.0f);
		glm::mat4 testProjMatrix = glm::mat4(1.0f);

		GLint testModelLocation = glGetUniformLocation(shaderProgram.id, "model");
		glUniformMatrix4fv(testModelLocation, 1, GL_FALSE, glm::value_ptr(testModelMatrix));
		GLint testViewLocation = glGetUniformLocation(shaderProgram.id, "view");
		glUniformMatrix4fv(testViewLocation, 1, GL_FALSE, glm::value_ptr(testViewMatrix));
		GLint testProjLocation = glGetUniformLocation(shaderProgram.id, "proj");
		glUniformMatrix4fv(testProjLocation, 1, GL_FALSE, glm::value_ptr(testProjMatrix));

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, testIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		*/

		/*
		testModel.mesh.vao.Bind();

		glm::mat4 testModelMatrix = glm::mat4(1.0f);
		testModelMatrix = glm::translate(testModel.position);
		glm::mat4 testViewMatrix = glm::mat4(1.0f);
		glm::mat4 testProjMatrix = glm::mat4(1.0f);

		GLint testModelLocation = glGetUniformLocation(shaderProgram.id, "model");
		glUniformMatrix4fv(testModelLocation, 1, GL_FALSE, glm::value_ptr(testModelMatrix));
		GLint testViewLocation = glGetUniformLocation(shaderProgram.id, "view");
		glUniformMatrix4fv(testViewLocation, 1, GL_FALSE, glm::value_ptr(camera.ViewMatrix()));
		GLint testProjLocation = glGetUniformLocation(shaderProgram.id, "proj");
		glUniformMatrix4fv(testProjLocation, 1, GL_FALSE, glm::value_ptr(camera.ProjectionMatrix(45.0f, 0.01f, 100.0f)));

		testModel.mesh.texture.Uniform(shaderProgram, "tex0", 0);
		testModel.mesh.texture.Bind();

		glDrawElements(GL_TRIANGLES, testIndices.size(), GL_UNSIGNED_INT, 0);
		*/

		auto modelIDs = registry.ViewIDs<ModelComponent>();

		for (auto modelID : modelIDs)
		{
			shaderProgram.Use();

			auto modelPtr = registry.GetComponent<ModelComponent>(modelID);
			auto& model = modelPtr->model;

			/*
			std::cout << model.mesh.indices.size() << std::endl;
			for (auto& index : model.mesh.indices)
			{
				std::cout << "Index " << index << ": (" << model.mesh.vertices[index].position.x << ", " << model.mesh.vertices[index].position.y << ", " << model.mesh.vertices[index].position.z << ")" << std::endl;
			}
			*/

			model.mesh.vao.Bind();

			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(model.position);

			GLint modelLocation = glGetUniformLocation(shaderProgram.id, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			GLint viewLocation = glGetUniformLocation(shaderProgram.id, "view");
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.ViewMatrix()));
			GLint projLocation = glGetUniformLocation(shaderProgram.id, "proj");
			glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(camera.ProjectionMatrix(45.0f, 0.1f, 100.0f)));

			model.mesh.texture.Uniform(shaderProgram, "tex0", 0);
			model.mesh.texture.Bind();

			glDrawElements(GL_TRIANGLES, model.mesh.indices.size(), GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}