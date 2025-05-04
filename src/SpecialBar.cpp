#include "SpecialBar.h"
#include "PoiseHealthHandler.h"

namespace MaxsuPoise
{
	float SpecialBar::GetMaxSpecial(RE::Actor* a_actor)
	{
		if (a_actor) {
			return PoiseHealthHandler::GetTotalPoiseHealth(a_actor);
		}
		return 1.0f;
	}

	float SpecialBar::GetCurrentSpecial(RE::Actor* a_actor)
	{
		if (a_actor) {
			return PoiseHealthHandler::GetCurrentPoiseHealth(a_actor);
		}
		return 1.0f;
	}

	void SpecialBar::SetBarColor(RE::ActorHandle a_targetHandle, std::uint32_t a_color)
	{
		trueHUDInterface->OverrideSpecialBarColor(a_targetHandle, TRUEHUD_API::BarColorType::BarColor, a_color);
	}

	void SpecialBar::RevertBarColor(RE::ActorHandle a_targetHandle)
	{
		trueHUDInterface->RevertSpecialBarColor(a_targetHandle, TRUEHUD_API::BarColorType::BarColor);
	}

	void SpecialBar::FlashSpecialBar(RE::ActorHandle a_targetHandle)
	{
		trueHUDInterface->FlashActorSpecialBar(SKSE::GetPluginHandle(), a_targetHandle, false);
	}

	void SpecialBar::Initialize()
	{
		trueHUDInterface = reinterpret_cast<TRUEHUD_API::IVTrueHUD4*>(TRUEHUD_API::RequestPluginAPI(TRUEHUD_API::InterfaceVersion::V4));
		if (trueHUDInterface) {
			if (trueHUDInterface->RequestSpecialResourceBarsControl(SKSE::GetPluginHandle()) == TRUEHUD_API::APIResult::OK) {
				trueHUDInterface->RegisterSpecialResourceFunctions(SKSE::GetPluginHandle(), MaxsuPoise::SpecialBar::GetCurrentSpecial, MaxsuPoise::SpecialBar::GetMaxSpecial, true);
				INFO("TrueHUD special bar initialized")
				enabled = true;
			}
			else {
				WARN("Failed to obtain TrueHUD API")
				enabled = false;
			}
		}
		else {
			WARN("Failed to obtain TrueHUD API");
			enabled = false;
		}
	}
}