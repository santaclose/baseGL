#include "PropertiesPanel.h"

PropertiesPanel::PropertiesPanel(const std::string& name) : Panel(name) {}

void PropertiesPanel::ImGuiCall(const ImGuiIO& io)
{
	ImGui::Begin(m_name.c_str());
	ImGui::SliderFloat3("Square Color", squareColorRef, 0.0f, 1.0f);
	ImGui::SliderFloat3("Cube Color", cubeColorRef, 0.0f, 1.0f);

	int lastMsaaCount = currentMsaaCount;
	ImGui::SliderInt("MSAA Count", &currentMsaaCount, 2, 8);
	if (
		(currentMsaaCount == 2 || currentMsaaCount == 4 || currentMsaaCount == 8) &&
		currentMsaaCount != lastMsaaCount
		)
	{
		panel3d->SetSamples((uint32_t)currentMsaaCount);
	}

	ImGui::InputText("Input Text", textInputBuffer, 256);

	ImGui::End();
}
