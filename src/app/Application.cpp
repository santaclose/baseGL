#include "Application.h"
#include "Panel2D.h"
#include "Panel3D.h"
#include "PropertiesPanel.h"
#include <iostream>

PropertiesPanel* propertiesPanel;
Panel2D* panel2d;
Panel3D* panel3d;

void Application::Init()
{
	panel2d = new Panel2D("2D", glm::vec3(0.7, 0.7, 0.7));
	panel3d = new Panel3D("3D", glm::vec3(0.7, 0.7, 0.7));
	propertiesPanel = new PropertiesPanel("Properties");

	propertiesPanel->cubeColorRef = panel3d->GetCubeColorReference();
	propertiesPanel->squareColorRef = panel2d->GetSquareColorReference();
}

void Application::OnUpdate(const ImGuiIO& io)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	panel2d->ImGuiCall(io);
	panel3d->ImGuiCall(io);
	ImGui::PopStyleVar();
	propertiesPanel->ImGuiCall(io);
}

void Application::Terminate()
{
	delete propertiesPanel;
	delete panel3d;
	delete panel2d;
}
