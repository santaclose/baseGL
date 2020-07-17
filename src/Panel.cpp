#include "Panel.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>
#include "ImVec2Operators.h"

std::vector<Panel*> Panel::all;

Panel::Panel(const std::string& name, const glm::vec3& clearColor)
{
	all.push_back(this);
	m_name = name;
	m_clearColor = clearColor;
}

void Panel::ImGuiCall(const ImGuiIO& io)
{
	ImGui::Begin(m_name.c_str());

	if (/*ImGui::IsWindowFocused() && */ImGui::IsMouseHoveringRect(ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos(), ImGui::GetWindowContentRegionMax() + ImGui::GetWindowPos()))
	{
		ImVec2 mousePos = io.MousePos - ImGui::GetWindowContentRegionMin() - ImGui::GetWindowPos();
		HandleInput(io, *((glm::vec2*)&mousePos));
	}
	glm::vec2 currentSize = *((glm::vec2*)&ImGui::GetContentRegionAvail());
	if (currentSize != m_size)
	{
		m_size = currentSize;
		m_frameBuffer.Resize(m_size.x, m_size.y);
		OnResize();
	}
	m_frameBuffer.Bind();
	Draw();
	m_frameBuffer.Unbind();
	uint32_t textureID = m_frameBuffer.getColorAttachmentID();
	ImGui::Image((void*)textureID, *((ImVec2*) &m_size), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}
