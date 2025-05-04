#pragma once
#include "TrueHUDAPI.h"

namespace MaxsuPoise
{
	class SpecialBar
	{
	public:
		static SpecialBar* GetSingleton()
		{
			static SpecialBar manager;
			return &manager;
		}

		void Initialize();
		void FlashSpecialBar(RE::ActorHandle a_targetHandle);
		void SetBarColor(RE::ActorHandle a_targetHandle, std::uint32_t a_color);
		void RevertBarColor(RE::ActorHandle a_targetHandle);

		bool enabled;

	private:
		constexpr SpecialBar() noexcept = default;
		SpecialBar(const SpecialBar&) = delete;
		SpecialBar(SpecialBar&&) = delete;

		~SpecialBar() = default;

		SpecialBar& operator=(const SpecialBar&) = delete;
		SpecialBar& operator=(SpecialBar&&) = delete;

		static float GetMaxSpecial(RE::Actor* a_actor);
		static float GetCurrentSpecial(RE::Actor* a_actor);

		static inline TRUEHUD_API::IVTrueHUD4* trueHUDInterface;
	};
}