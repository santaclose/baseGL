#pragma once

#include <imgui.h>
#include <cstdint>
#include <glad/glad.h>

class Framebuffer
{
	uint32_t m_gl_id = 0;
	uint32_t m_gl_color_attachment = 0, m_gl_depth_attachment = 0;
	uint32_t m_width = 0, m_height = 0;
	uint32_t m_samples = 0;
	bool m_includeDepth = false;

	void Delete();

public:
	Framebuffer(uint32_t width, uint32_t height, uint32_t samples = 4, bool includeDepth = true);
	~Framebuffer();

	void Invalidate();
	void Resize(uint32_t width, uint32_t height);
	void SetSamples(uint32_t samples);
	void Bind(GLenum target = GL_FRAMEBUFFER);
	void Unbind(GLenum target = GL_FRAMEBUFFER);
	uint32_t getColorAttachmentID();
};

