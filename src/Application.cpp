#include "Application.h"
#include "Panel2D.h"
#include "Panel3D.h"

Panel2D* panel2d;
Panel3D* panel3d;

void Application::Init()
{
	panel2d = new Panel2D("2D", glm::vec3(0.7, 0.7, 0.7));
	panel3d = new Panel3D("3D", glm::vec3(0.7, 0.7, 0.7));
}

void Application::OnUpdate(const ImGuiIO& io)
{
	for (Panel* p : Panel::all)
		p->ImGuiCall(io);
}

void Application::Terminate()
{
	delete panel2d;
	delete panel3d;
}
