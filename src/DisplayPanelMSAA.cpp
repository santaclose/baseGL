#include "DisplayPanelMSAA.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>
#include "ImVec2Operators.h"

DisplayPanelMSAA::DisplayPanelMSAA(const std::string& name, const glm::vec3& clearColor, uint32_t samples) : Panel(name)
{
	m_clearColor = clearColor;
	m_frameBufferMSAA.SetSamples(samples);
}

void DisplayPanelMSAA::ImGuiCall(const ImGuiIO& io)
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
		HandleInput(io, *((glm::vec2*) & mousePos));
	}
	glm::vec2 currentSize = *((glm::vec2*) & ImGui::GetContentRegionAvail());
	if (currentSize != m_size)
	{
		m_size = currentSize;
		m_frameBufferMSAA.Resize((uint32_t)m_size.x, (uint32_t)m_size.y);
		m_frameBuffer.Resize((uint32_t)m_size.x, (uint32_t)m_size.y);
		OnResize();
	}
	m_frameBufferMSAA.Bind();
	Draw();
	m_frameBufferMSAA.Unbind();

	m_frameBuffer.Bind();
	m_frameBufferMSAA.Bind(true);
	glBlitFramebuffer(0, 0, (uint32_t)m_size.x, (uint32_t)m_size.y, 0, 0, (uint32_t)m_size.x, (uint32_t)m_size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	m_frameBuffer.Unbind();

	uint32_t textureID = m_frameBuffer.getColorAttachmentID();
	ImGui::Image((void*)textureID, *((ImVec2*)&m_size), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void DisplayPanelMSAA::SetSamples(uint32_t samples)
{
	m_frameBufferMSAA.SetSamples(samples);
}
