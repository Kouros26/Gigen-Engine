#include "FPSDisplay.h"
#include "imgui.h"
#include "InterfaceManager.h"

FPSDisplay::FPSDisplay()
{
	delay = Time::FPS::GetFPSUpdateDelay();
}

FPSDisplay::~FPSDisplay()
= default;

void FPSDisplay::Draw()
{
	FPSGraph();
	FPSDelay();
	VSync();
}

void FPSDisplay::FPSDelay()
{
	ImGui::Text("Average FPS : %.2f", Time::FPS::GetAverageFPS());
	ImGui::Text("FPS Update delay"); ImGui::SameLine();
	ImGui::SliderFloat("##1", &delay, 0.01f, 2, "%.1f");

	if (delay != lastDelay)
		Time::FPS::SetFPSUpdateDelay(delay);

	lastDelay = delay;
	ImGui::Separator();
}

void FPSDisplay::VSync()
{
	ImGui::Text("VSync"); ImGui::SameLine();
	ImGui::Checkbox("##2", &vSync);

	if (vSync != lastVSync)
		Time::FPS::ToggleVSync(vSync);

	lastVSync = vSync;
}

void FPSDisplay::FPSGraph() const
{
	ImGui::Text("FPS : %.2f", Time::FPS::GetFPS());
	ImGui::PlotLines("##3 ", Time::FPS::GetFPSVec().data(), 10);
	ImGui::Separator();
}