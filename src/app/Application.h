#pragma once
#include <imgui.h>

#define APPLICATION_NAME "example"
#define APPLICATION_WIDTH 1280
#define APPLICATION_HEIGHT 720

namespace Application
{
	void Init();
	void OnUpdate(const ImGuiIO& io);
	void Terminate();
}