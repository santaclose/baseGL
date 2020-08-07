#include "PropertiesPanel.h"

PropertiesPanel::PropertiesPanel(const std::string& name) : Panel(name) {}

void PropertiesPanel::ImGuiCall(const ImGuiIO& io)
{
	ImGui::Begin(m_name.c_str());
	ImGui::SliderFloat3("Square Color", squareColorRef, 0.0f, 1.0f);
	ImGui::SliderFloat3("Cube Color", cubeColorRef, 0.0f, 1.0f);
	ImGui::End();
}
