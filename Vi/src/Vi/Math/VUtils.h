#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct VTransform {
//	VTransform() {}

	VTransform(glm::vec3 location = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) :
		Location(location), Rotation(rotation), Scale(scale) {
	}
	glm::vec3 Location = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale    = glm::vec3(1.0f);

};