#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Framebuffer.h"

class Panel
{
protected:
	int m_id;
	glm::vec3 m_clearColor;
	std::string m_name;
	glm::vec2 m_size = {0, 0};
	Framebuffer m_frameBuffer = Framebuffer(0, 0);
	bool m_cursorLoopEnabled = false;

public:
	static std::vector<Panel*> all;

public:
	Panel(const std::string& name, const glm::vec3& clearColor);
	void ImGuiCall(const ImGuiIO& io);

protected:
	virtual void OnResize() = 0;
	virtual void HandleInput(const ImGuiIO& io, const glm::vec2& relativeMousePos) = 0;
	virtual void Draw() = 0;
};