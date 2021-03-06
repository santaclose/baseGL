#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Entity
{
protected:
	glm::vec3 m_position = glm::vec3(0.0, 0.0, 0.0);
	glm::fquat m_rotation = glm::fquat(1.0, 0.0, 0.0, 0.0);

public:
	void SetPosition(const glm::vec3& newPosition);
	void SetPosition(float x, float y, float z);
	void SetRotation(const glm::fquat& newRotation);
	void SetRotation(const glm::vec3& newRotation); // from euler angles
	void SetRotation(float x, float y, float z); // from euler angles
	void LookAt(const glm::vec3& target, const glm::vec3& up);
	const glm::vec3& GetPosition();
	const glm::fquat& GetRotation();
	glm::vec3 Forward();
	glm::vec3 Right();
	glm::vec3 Up();
};