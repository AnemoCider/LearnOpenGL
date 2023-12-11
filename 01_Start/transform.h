#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 getModelTransformMatrix(float angle_in_degree = -55.0f, 
	glm::vec3 axis = { 1.0f, 0.0f, 0.0f }) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), 
		glm::vec3(0.5f, 1.0f, 0.0f));
	return model;
}

glm::mat4 getViewTransformMatrix(glm::vec3 camera_pos = { 0.0f, 0.0f, 3.0f }) {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, -camera_pos);
	return view;
}

// near and far are positive.
glm::mat4 getProjectionTransformMatrix(float fov_in_degree = 45.0f, 
	float width = 800.0f, float height = 600.0f, float near = 0.1f, float far = 100.0f) {
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov_in_degree), width / height, near, far);
	return projection;
}

glm::mat4 getMVPMatrix() {
	return getProjectionTransformMatrix() *
		getViewTransformMatrix() *
		getModelTransformMatrix();
}