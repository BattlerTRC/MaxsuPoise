#include "Hooks/PoiseRegenHandler.h"
#include "PoiseHealthHandler.h"
#include "SpecialBar.h"
#include "Utils.h"

namespace MaxsuPoise
{
	void PoiseRegenHandler::CharacterEx::Hook_Update(float a_delta)
	{
		func(this, a_delta);
		UpdatePoiseValue(this, RE::GetSecondsSinceLastFrame());
	}

	void PoiseRegenHandler::PlayerEx::Hook_Update(float a_delta)
	{
		func(this, a_delta);
		UpdatePoiseValue(this, a_delta);
	}

	void PoiseRegenHandler::UpdatePoiseValue(RE::Actor* a_target, float a_delta)
	{
		if (!a_target || !a_target->GetActorRuntimeData().currentProcess || !a_target->GetActorRuntimeData().currentProcess->high || !a_target->Is3DLoaded() || !a_target->IsInitialized())
			return;

		const auto totalPoiseHealth = PoiseHealthHandler::GetTotalPoiseHealth(a_target);
		auto currentPoiseHealth = PoiseHealthHandler::GetCurrentPoiseHealth(a_target);
		auto recoveryState = RecoveryStateHandler::GetRecoveryState(a_target);

		// Some small changes are made here so that if the user enables the TrueHUD special bar, the bar is visually consistent.
		// The mod's original code made the bar instantly snap back to full after it was depleted which looked strange.

		if (!a_target->IsInCombat() || (currentPoiseHealth - totalPoiseHealth) >= 0.01f) {

			// End recovery state if poise is completely recovered or combat ends.
			if (recoveryState != 0) {
				RecoveryStateHandler::SetRecoveryState(a_target, 0);
				auto specialBar = SpecialBar::GetSingleton();
				if (specialBar->enabled) {
					specialBar->RevertBarColor(a_target->GetHandle());
				}
			}

			currentPoiseHealth = totalPoiseHealth;
		}
		else {
			auto regenDelayTimer = RegenDelayHandler::GetPoiseRegenDelayTimer(a_target);
			if (regenDelayTimer > 0.f)
				RegenDelayHandler::SetPoiseRegenDelayTimer(a_target, regenDelayTimer - a_delta);
			else {
				currentPoiseHealth += totalPoiseHealth * a_delta * GetPoiseRegenRate();
			}

			auto staggerProtectTimer = StaggerProtectHandler::GetStaggerProtectTimer(a_target);
			if (staggerProtectTimer > 0.f) {
				StaggerProtectHandler::SetStaggerProtectTimer(a_target, staggerProtectTimer - a_delta);
			}
			else {
				// End recovery state if stagger protect timer runs out.
				RecoveryStateHandler::SetRecoveryState(a_target, 0);
				auto specialBar = SpecialBar::GetSingleton();
				if (specialBar->enabled) {
					specialBar->RevertBarColor(a_target->GetHandle());
				}
			}
		}

		PoiseHealthHandler::SetCurrentPoiseHealth(a_target, currentPoiseHealth);
	}

	float PoiseRegenHandler::GetPoiseRegenRate()
	{
		return GetGameSettingFloat("fMaxsuPoise_PoiseRegen", 0.05f);
	}

	float RegenDelayHandler::GetMaxRegenDelayTime()
	{
		return GetGameSettingFloat("fMaxsuPoise_RegenDelayTime", 1.0f);
	}

	float RegenDelayHandler::GetPoiseRegenDelayTimer(RE::Actor* a_target)
	{
		float result = 0.f;
		if (!a_target || !a_target->GetGraphVariableFloat(CURRENT_REGEN_DELAY_GV, result))
			WARN("Not Graph Variable Float Found: {}", CURRENT_REGEN_DELAY_GV);

		return result;
	}

	bool RegenDelayHandler::SetPoiseRegenDelayTimer(RE::Actor* a_target, const float& a_in)
	{
		return a_target && a_target->SetGraphVariableFloat(CURRENT_REGEN_DELAY_GV, a_in);
	}

	float StaggerProtectHandler::GetStaggerProtectTimer(RE::Actor* a_target)
	{
		float result = 0.f;
		if (!a_target || !a_target->GetGraphVariableFloat(CURRENT_STAGGER_TIMER_GV, result))
			WARN("Not Graph Variable Float Found: {}", CURRENT_STAGGER_TIMER_GV);

		return result;
	}

	bool StaggerProtectHandler::SetStaggerProtectTimer(RE::Actor* a_target, const float& a_in)
	{
		return a_target && a_target->SetGraphVariableFloat(CURRENT_STAGGER_TIMER_GV, a_in);
	}

	float StaggerProtectHandler::GetMaxStaggerProtectTime()
	{
		return GetGameSettingFloat("fMaxsuPoise_StaggerProtectTime", 0.85f);
	}

	int RecoveryStateHandler::GetRecoveryState(RE::Actor* a_target)
	{
		int result = 0;
		if (!a_target || !a_target->GetGraphVariableInt(RECOVERY_STATE, result))
			WARN("Not Graph Variable Float Found: {}", RECOVERY_STATE);

		return result;
	}

	bool RecoveryStateHandler::SetRecoveryState(RE::Actor* a_target, const int& a_in)
	{
		return a_target && a_target->SetGraphVariableFloat(RECOVERY_STATE, a_in);
	}

}