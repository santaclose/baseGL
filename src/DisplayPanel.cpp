#include "DisplayPanel.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>
#include "ImVec2Operators.h"

DisplayPanel::DisplayPanel(const std::string& name, const glm::vec3& clearColor, uint32_t samples, bool includeDepth) : Panel(name)
{
	m_clearColor = clearColor;
	m_includeDepth = includeDepth;
	m_samples = samples;
	assert(m_samples == 1 || m_samples == 2 || m_samples == 4 || m_samples == 8);
	if (m_samples > 1)
		m_frameBufferMSAA = new Framebuffer(0, 0, m_samples, m_includeDepth);
	m_frameBuffer = new Framebuffer(0, 0, 1, includeDepth);
}

DisplayPanel::~DisplayPanel()
{
	delete m_frameBuffer;
	if (m_frameBufferMSAA != nullptr)
		delete m_frameBufferMSAA;
}

void DisplayPanel::ImGuiCall(const ImGuiIO& io)
{
	ImGui::Begin(m_name.c_str());

	if (inputHandlingRetained == nullptr &&
		/*ImGui::IsWindowFocused() && */
		ImGui::IsMouseHoveringRect(
			ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos(),
			ImGui::GetWindowContentRegionMax() + ImGui::GetWindowPos())
		||
		inputHandlingRetained == this)

	{
		ImVec2 mousePos = io.MousePos - ImGui::GetWindowContentRegionMin() - ImGui::GetWindowPos();
		glm::vec2 mp = glm::vec2(mousePos.x, mousePos.y);
		HandleInput(io, mp);
	}
	ImVec2 avail = ImGui::GetContentRegionAvail();
	glm::uvec2 currentSize(
		std::max(0, (int)avail.x),
		std::max(0, (int)avail.y)
	);
	if (currentSize != m_size)
	{
		m_size = currentSize;
		if (m_samples > 1)
			m_frameBufferMSAA->Resize(m_size.x, m_size.y);
		m_frameBuffer->Resize(m_size.x, m_size.y);
		OnResize();
	}
	if (m_samples > 1)
		m_frameBufferMSAA->Bind();
	else
		m_frameBuffer->Bind();
	auto clearValue = GL_COLOR_BUFFER_BIT;
	if (m_includeDepth)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		clearValue |= GL_DEPTH_BUFFER_BIT;
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, 1.0);
	glClear(clearValue);
	Draw();
	if (m_samples > 1)
	{
		m_frameBufferMSAA->Unbind();

		m_frameBuffer->Bind(GL_DRAW_FRAMEBUFFER);
		m_frameBufferMSAA->Bind(GL_READ_FRAMEBUFFER);
		glBlitFramebuffer(0, 0, m_size.x, m_size.y, 0, 0, m_size.x, m_size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		m_frameBufferMSAA->Unbind(GL_READ_FRAMEBUFFER);
		m_frameBuffer->Unbind(GL_DRAW_FRAMEBUFFER);
	}
	else
		m_frameBuffer->Unbind();
	uint32_t textureID = m_frameBuffer->getColorAttachmentID();
	ImVec2 sizeFloats = { (float) m_size.x, (float) m_size.y };
	ImGui::Image((void*)textureID, sizeFloats, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void DisplayPanel::SetSamples(uint32_t samples)
{
	m_samples = samples;
	assert(m_samples == 1 || m_samples == 2 || m_samples == 4 || m_samples == 8);
	if (m_samples > 1)
	{
		if (m_frameBufferMSAA == nullptr)
			m_frameBufferMSAA = new Framebuffer(0, 0, m_samples, m_includeDepth);
		m_frameBufferMSAA->SetSamples(m_samples);
	}
}