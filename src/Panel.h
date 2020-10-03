#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Framebuffer.h"

class Panel
{
protected:
	std::string m_name;
	static Panel* inputHandlingRetained;

public:
	static std::vector<Panel*> all;

public:
	Panel(const std::string& name);
	virtual void ImGuiCall(const ImGuiIO& io) = 0;
};