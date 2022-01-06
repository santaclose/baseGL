#include "Framebuffer.h"
#include <glad/glad.h>
#include <iostream>

Framebuffer::Framebuffer(uint32_t width, uint32_t height, bool includeDepth)
{
	m_width = width;
	m_height = height;
	m_includeDepth = includeDepth;
	if (m_width > 0 && m_height > 0)
		Invalidate();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_gl_id);
	glDeleteTextures(1, &m_gl_color_attachment);
	if (m_includeDepth) glDeleteFramebuffers(1, &m_gl_depth_attachment);
}

void Framebuffer::Invalidate()
{
	if (m_gl_id != 0)
	{
		glDeleteFramebuffers(1, &m_gl_id);
		glDeleteTextures(1, &m_gl_color_attachment);
		if (m_includeDepth) glDeleteFramebuffers(1, &m_gl_depth_attachment);
	}

	glCreateFramebuffers(1, &m_gl_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gl_id);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_gl_color_attachment);
	if (m_includeDepth) glGenRenderbuffers(1, &m_gl_depth_attachment);

	glBindTexture(GL_TEXTURE_2D, m_gl_color_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gl_color_attachment, 0);

	if (m_includeDepth)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_gl_depth_attachment);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gl_depth_attachment);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "[OPENGL] Error creating framebuffer with width " << m_width << " and height " << m_height << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0)
		return;
	m_width = width;
	m_height = height;
	Invalidate();
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_gl_id);
	glViewport(0, 0, m_width, m_height);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t Framebuffer::getColorAttachmentID()
{
	return m_gl_color_attachment;
}
