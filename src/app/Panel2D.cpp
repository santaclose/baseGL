#include "Panel2D.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>
#include <string>

#include "Renderer.h"

#include <Font.h>

#define MAX_CAMERA_SIZE 100.0f
#define MIN_CAMERA_SIZE 1.0f

Panel2D::Panel2D(const std::string& name, const glm::vec3& clearColor) : DisplayPanel(name, clearColor)
{
	CameraSpecs ss;
	ss.perspective = false;
	m_camera = new Camera(ss);
	m_camera->SetPosition(0.0, 0.0, 1.0);

	m_fontId = Renderer::CreateFont("assets/fonts/FiraCode/FiraCode-Regular.ttf");
	//m_fontId = Renderer::CreateFont("assets/fonts/arial.ttf");
}

void Panel2D::OnResize()
{
	m_camera->m_specs.aspectRatio = m_size.x / m_size.y;
	Renderer::UpdateWindowSize(m_size);
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

	Renderer::SetMatrix((float*)(&(m_camera->GetMatrix())));
	//Renderer::DrawText(m_fontId, textToDrawBuffer, &m_squareColor.x, true, { center.x, center.y }, 11.0f);
	//Renderer::DrawText(m_fontId, textToDrawBuffer, &m_squareColor.x, false);

#define K 1.35
	glm::vec2 center = { 0.0f, 0.0f };
	Renderer::DrawText(m_fontId, textToDrawBuffer, &m_squareColor.x, false, { center.x, center.y }, 1.0);
	Renderer::DrawText(m_fontId, textToDrawBuffer, &m_squareColor.x, false, { center.x, center.y + K }, 0.5);
	Renderer::DrawText(m_fontId, textToDrawBuffer, &m_squareColor.x, false, { center.x, center.y + K + K * 0.5 }, 0.25);
	Renderer::DrawText(m_fontId, textToDrawBuffer, &m_squareColor.x, false, { center.x, center.y + K + K * 0.5 + K * 0.25 }, 0.125);
	Renderer::DrawText(m_fontId, textToDrawBuffer, &m_squareColor.x, false, { center.x, center.y + K + K * 0.5 + K * 0.25 + K * 0.125 }, 0.0625);
#undef K
}

float* Panel2D::GetSquareColorReference()
{
	return &m_squareColor.x;
}
