#pragma once
#include "Panel.h"
#include "Shader.h"
#include "Camera.h"

class Panel2D : public Panel
{
	float m_vertices[18] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
	};
	unsigned int m_VBO;
	unsigned int m_VAO;

	Shader m_shader;
	Camera* m_camera;

public:
	Panel2D(const std::string& name, const glm::vec3& clearColor);
	void OnResize();
	void HandleInput(const ImGuiIO& io, const glm::vec2& relativeMousePos);
	void Draw();
};