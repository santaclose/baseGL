#pragma once
#include "DisplayPanel.h"
#include "Shader.h"
#include "Camera.h"

class Panel3D : public DisplayPanel
{
    //cube
	float m_vertices[18*6] = {
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f
    };

	unsigned int m_VBO;
	unsigned int m_VAO;

    Entity m_gimbal;
    glm::vec3 m_gimbalRot = glm::vec3(0.0, 3.14159265, 0.0);
    float m_distance = 4.0;
	Shader m_shader;
	Camera* m_camera;

    glm::vec4 m_cubeColor = glm::vec4(0.1f, 0.1f, 0.9f, 1.0f);

    void UpdateCamera();

public:
	Panel3D(const std::string& name, const glm::vec3& clearColor);
	void OnResize();
	void HandleInput(const ImGuiIO& io, const glm::vec2& relativeMousePos);
	void Draw();

    float* GetCubeColorReference();
};