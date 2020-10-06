#include "DisplayPanel.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>
#include "ImVec2Operators.h"

DisplayPanel::DisplayPanel(const std::string& name, const glm::vec3& clearColor, bool includeDepth) : Panel(name)
{
	m_clearColor = clearColor;
	m_includeDepth = includeDepth;
	m_frameBuffer = new Framebuffer(0, 0, includeDepth);
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
		HandleInput(io, *((glm::vec2*)&mousePos));
	}
	glm::vec2 currentSize = *((glm::vec2*)&ImGui::GetContentRegionAvail());
	if (currentSize != m_size)
	{
		m_size = currentSize;
		m_frameBuffer->Resize((uint32_t) m_size.x, (uint32_t) m_size.y);
		OnResize();
	}
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
	m_frameBuffer->Unbind();
	uint32_t textureID = m_frameBuffer->getColorAttachmentID();
	ImGui::Image((void*)textureID, *((ImVec2*) &m_size), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}