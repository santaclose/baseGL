#pragma once
#include <imgui.h>
#include "Panel.h"

class PropertiesPanel : public Panel
{
public:
	float* squareColorRef;
	float* cubeColorRef;

	PropertiesPanel(const std::string& name);
	void ImGuiCall(const ImGuiIO& io) override;
};