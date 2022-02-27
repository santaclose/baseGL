#pragma once
#include "DisplayPanel.h"
#include "Shader.h"
#include "Camera.h"

class Panel2D : public DisplayPanel
{
	unsigned int m_fontId;
	Camera* m_camera;

	glm::vec4 m_squareColor = glm::vec4(0.9f, 0.1f, 0.1f, 1.0f);

public:
	const char* textToDrawBuffer;
	Panel2D(const std::string& name, const glm::vec3& clearColor);
	void OnResize();
	void HandleInput(const ImGuiIO& io, const glm::vec2& relativeMousePos);
	void Draw();

	float* GetSquareColorReference();
};