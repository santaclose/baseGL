#include "FramebufferMSAA.h"
#include <glad/glad.h>
#include <iostream>

FramebufferMSAA::FramebufferMSAA(uint32_t width, uint32_t height, uint32_t samples, bool includeDepth)
{
	m_width = width;
	m_height = height;
	m_samples = samples;
	m_includeDepth = includeDepth;
	if (m_width > 0 && m_height > 0)
		Invalidate();
}

FramebufferMSAA::~FramebufferMSAA()
{
	glDeleteFramebuffers(1, &m_gl_id);
	glDeleteTextures(1, &m_gl_color_attachment);
	if (m_includeDepth) glDeleteTextures(1, &m_gl_depth_attachment);
}

void FramebufferMSAA::Invalidate()
{
	if (m_gl_id != 0)
	{
		glDeleteFramebuffers(1, &m_gl_id);
		glDeleteTextures(1, &m_gl_color_attachment);
		if (m_includeDepth) glDeleteTextures(1, &m_gl_depth_attachment);
	}

	glCreateFramebuffers(1, &m_gl_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gl_id);

	glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_gl_color_attachment);
	if (m_includeDepth) glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_gl_depth_attachment);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_gl_color_attachment);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGB, m_width, m_height, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_gl_color_attachment, 0);

	if (m_includeDepth)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_gl_depth_attachment);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_DEPTH_COMPONENT, m_width, m_height, GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_gl_depth_attachment, 0);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "[OPENGL] Error creating multisample framebuffer with width " << m_width << " and height " << m_height << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferMSAA::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0)
		return;
	m_width = width;
	m_height = height;
	Invalidate();
}

void FramebufferMSAA::SetSamples(uint32_t samples)
{
	m_samples = samples;
	if (m_width > 0 && m_height > 0)
		Invalidate();
}

void FramebufferMSAA::Bind(bool readOnly)
{
	if (readOnly)
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gl_id);
	else
		glBindFramebuffer(GL_FRAMEBUFFER, m_gl_id);

	glViewport(0, 0, m_width, m_height);
}

void FramebufferMSAA::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t FramebufferMSAA::getColorAttachmentID()
{
	return m_gl_color_attachment;
}
