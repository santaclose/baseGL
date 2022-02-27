#pragma once
#include <string>
#include <glm/glm.hpp>

#undef DrawText // stupid windows thing
#undef CreateFont// stupid windows thing

namespace Renderer {

	void UpdateWindowSize(const glm::vec2& newSize);
	void SetMatrix(const float* mat);

	void DrawCube(const float* color);
	void DrawSquare(const float* color);
	void DrawText(unsigned int fontId, const std::string& text, const float* color, bool screenSpace = true, const glm::vec2& pos = { 0.0f, 0.0f }, float scale = 1.0f, bool center=false);
	void DrawSVG(unsigned int svgId, const glm::vec2& pos = { 0.0f, 0.0f }, float scale = 1.0f);

	unsigned int CreateFont(const std::string& fontFilePath);
	unsigned int CreateSVG(const std::string& svgFilePath);
}