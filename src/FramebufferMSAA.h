#pragma once

#include <imgui.h>
#include <cstdint>

class FramebufferMSAA
{
	uint32_t m_gl_id = 0;
	uint32_t m_gl_color_attachment = 0;
	uint32_t m_width = 0, m_height = 0;
	uint32_t m_samples = 4;

public:
	FramebufferMSAA(uint32_t width, uint32_t height, uint32_t samples = 4);
	~FramebufferMSAA();

	void Invalidate();
	void Resize(uint32_t width, uint32_t height);
	void SetSamples(uint32_t samples);
	void Bind(bool readOnly = false);
	void Unbind();
	uint32_t getColorAttachmentID();
};

