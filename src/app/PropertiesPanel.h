#pragma once
#include <imgui.h>
#include "Panel.h"
#include "Panel3D.h"

class PropertiesPanel : public Panel
{
public:
	float* squareColorRef;
	float* cubeColorRef;
	char textInputBuffer[256] = "210asdf";

	DisplayPanelMSAA* panel3d;
	int currentMsaaCount = 8;

	PropertiesPanel(const std::string& name);
	void ImGuiCall(const ImGuiIO& io) override;

	inline const char* GetTextInputBuffer()
	{
		return textInputBuffer;
	}
};