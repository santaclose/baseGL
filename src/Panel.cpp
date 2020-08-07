#include "Panel.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>
#include "ImVec2Operators.h"

std::vector<Panel*> Panel::all;

Panel::Panel(const std::string& name)
{
	m_name = name;
	all.push_back(this);
}