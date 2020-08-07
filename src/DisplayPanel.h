#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Panel.h"
#include "Framebuffer.h"

class DisplayPanel : public Panel
{
protected:
	glm::vec3 m_clearColor;
	glm::vec2 m_size = { 0, 0 };
	Framebuffer m_frameBuffer = Framebuffer(0, 0);

public:
	DisplayPanel(const std::string& name, const glm::vec3& clearColor);
	void ImGuiCall(const ImGuiIO& io) override;

protected:
	virtual void OnResize() = 0;
	virtual void HandleInput(const ImGuiIO& io, const glm::vec2& relativeMousePos) = 0;
	virtual void Draw() = 0;
};