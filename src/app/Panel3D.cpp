#include "Panel3D.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>

#include "Renderer.h"

#define SENSITIVITY 0.01f
#define MAX_DISTANCE 7.0f
#define MIN_DISTANCE 2.0f

#define CAMERA_LIMIT 1.565

void Panel3D::UpdateCamera()
{
	m_camera->SetPosition(m_gimbal.Forward() * m_distance);
	m_camera->LookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

Panel3D::Panel3D(const std::string& name, const glm::vec3& clearColor) : DisplayPanelMSAA(name, clearColor, 8)
{
	CameraSpecs ss;
	ss.perspective = true;
	m_camera = new Camera(ss);
	m_distance = 3.19f;
	m_gimbal.SetRotation(glm::fquat(m_gimbalRot));
	UpdateCamera();
}

void Panel3D::OnResize()
{
	m_camera->m_specs.aspectRatio = m_size.x / m_size.y;
}

void Panel3D::HandleInput(const ImGuiIO& io, const glm::vec2& relativeMousePos)
{
	m_distance -= io.MouseWheel;
	if (m_distance > MAX_DISTANCE)
		m_distance = MAX_DISTANCE;
	else if (m_distance < MIN_DISTANCE)
		m_distance = MIN_DISTANCE;

	if (io.MouseDown[2])
	{
		inputHandlingRetained = this;
		m_gimbalRot.y -= io.MouseDelta.x * SENSITIVITY;
		m_gimbalRot.x += io.MouseDelta.y * SENSITIVITY;

		if (m_gimbalRot.x > CAMERA_LIMIT)
			m_gimbalRot.x = CAMERA_LIMIT;
		else if (m_gimbalRot.x < -CAMERA_LIMIT)
			m_gimbalRot.x = -CAMERA_LIMIT;

		m_gimbal.SetRotation(glm::fquat(m_gimbalRot));
	}
	else
	{
		inputHandlingRetained = nullptr;
	}
	UpdateCamera();
}

void Panel3D::Draw()
{
	m_camera->Bind();
	m_camera->ComputeMatrices();

	Renderer::SetMatrix((float*)(&(m_camera->GetMatrix())));
	Renderer::DrawCube(&m_cubeColor.x);
}

float* Panel3D::GetCubeColorReference()
{
	return &m_cubeColor.x;
}
