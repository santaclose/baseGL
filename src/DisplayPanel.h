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
	Framebuffer* m_frameBuffer = nullptr;
	Framebuffer* m_frameBufferMSAA = nullptr;
	bool m_includeDepth = false;
	uint32_t m_samples = 0;

public:
	DisplayPanel(const std::string& name, const glm::vec3& clearColor, uint32_t samples = 4, bool includeDepth = true);
	~DisplayPanel();
	void ImGuiCall(const ImGuiIO& io) override;
	void SetSamples(uint32_t samples);

protected:
	virtual void OnResize() = 0;
	virtual void HandleInput(const ImGuiIO& io, const glm::vec2& relativeMousePos) = 0;
	virtual void Draw() = 0;
};