#include "Panel2D.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>

#define MAX_CAMERA_SIZE 30
#define MIN_CAMERA_SIZE 2

Panel2D::Panel2D(const std::string& name, const glm::vec3& clearColor) : DisplayPanel(name, clearColor)
{
	CameraSpecs ss;
	ss.perspective = false;
	m_camera = new Camera(ss);
	m_camera->SetPosition(0.0, 0.0, 1.0);

	m_shader.CreateFromFiles("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");
	m_shader.Bind();

	// 1. bind Vertex Array Object
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Panel2D::OnResize()
{
	m_camera->m_specs.aspectRatio = m_size.x / m_size.y;
}

void Panel2D::HandleInput(const ImGuiIO& io, const glm::vec2& relativeMousePos)
{
	m_camera->m_specs.orthographicScale -= io.MouseWheel;
	if (m_camera->m_specs.orthographicScale > MAX_CAMERA_SIZE)
		m_camera->m_specs.orthographicScale = MAX_CAMERA_SIZE;
	else if (m_camera->m_specs.orthographicScale < MIN_CAMERA_SIZE)
		m_camera->m_specs.orthographicScale = MIN_CAMERA_SIZE;

	if (io.MouseDown[2])
	{
		inputHandlingRetained = this;
		float pixelsToWorldFactor;
		if (m_camera->m_specs.aspectRatio >= 1.0)
			pixelsToWorldFactor = m_camera->m_specs.orthographicScale / m_size.y;
		else
			pixelsToWorldFactor = m_camera->m_specs.orthographicScale / m_size.x;

		m_camera->SetPosition(m_camera->GetPosition() + glm::vec3(-io.MouseDelta.x * pixelsToWorldFactor, io.MouseDelta.y * pixelsToWorldFactor, 0.0));
	}
	else
	{
		inputHandlingRetained = nullptr;
	}
}

void Panel2D::Draw()
{
	m_camera->Bind();
	m_camera->ComputeMatrices();

	m_shader.Bind();
	m_shader.SetUniformMatrix4fv("u_Mat", (float*)(&(m_camera->GetMatrix())));
	m_shader.SetUniform4fv("u_Color", &m_squareColor.x);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

float* Panel2D::GetSquareColorReference()
{
	return &m_squareColor.x;
}
