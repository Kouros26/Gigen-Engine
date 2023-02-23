#include "FPSDisplay.h"
#include "imgui.h"
#include "InterfaceManager.h"

FPSDisplay::FPSDisplay()
{
	InterfaceManager::AddEditorElement(this);
}

FPSDisplay::~FPSDisplay()
= default;

void FPSDisplay::Draw()
{
	ImGui::Begin("FPS Counter");
	ImGui::Text("Average FPS : %.2f", Time::FPS::GetAverageFPS());
	static float lastDelay;
	static float delay;
	ImGui::SliderFloat("FPS Update delay", &delay, 0, 20);

	if (delay != lastDelay)
		Time::FPS::SetFPSUpdateDelay(delay);

	lastDelay = delay;

	ImGui::End();
}
