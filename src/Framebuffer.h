#pragma once

#include <imgui.h>
#include <cstdint>

class Framebuffer
{
	uint32_t m_gl_id = 0;
	uint32_t m_gl_color_attachment = 0, m_gl_depth_attachment = 0;
	uint32_t m_width = 0, m_height = 0;
	bool m_includeDepth = false;

public:
	Framebuffer(uint32_t width, uint32_t height, bool includeDepth = false);
	~Framebuffer();

	void Invalidate();
	void Resize(uint32_t width, uint32_t height);
	void Bind();
	void Unbind();
	uint32_t getColorAttachmentID();
};

