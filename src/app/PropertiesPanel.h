#pragma once
#include <imgui.h>
#include "Panel.h"
#include "Panel3D.h"

class PropertiesPanel : public Panel
{
public:
	float* squareColorRef;
	float* cubeColorRef;
	DisplayPanelMSAA* panel3d;
	int currentMsaaCount = 4;

	PropertiesPanel(const std::string& name);
	void ImGuiCall(const ImGuiIO& io) override;
};