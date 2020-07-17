#pragma once
#include <imgui.h>

namespace Application
{
	void Init();
	void OnUpdate(const ImGuiIO& io);
	void Terminate();
}