#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 pos)
{
	Camera::width = width;
	Camera::height = height;
	this->position = pos;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::UpdateMatrix(float fov, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

	view = glm::lookAt(position, position + orientation, up);
	proj = glm::perspective(glm::radians(45.0f), (float)(width / height), nearPlane, farPlane);

	cameraMatrix = proj * view;
}

void Camera::Inputs(GLFWwindow* window, bool cursorEnabled)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += speed * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position += speed * -orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position += speed * -glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += speed * glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		position[1] -= 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position[1] += 0.5f;
	}
	if (!cursorEnabled) {
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		orientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));
		orientation = glm::rotate(orientation, glm::radians(-rotY), up);



		glfwSetCursorPos(window, width / 2, height / 2);
	}
}
