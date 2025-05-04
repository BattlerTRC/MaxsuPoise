#pragma once

namespace MaxsuPoise
{
	class PoiseRegenHandler
	{
	public:
		class CharacterEx : public RE::Character
		{
		public:
			static void InstallHook()
			{
				REL::Relocation<std::uintptr_t> CharacterVtbl{ VTABLE[0] };
				func = CharacterVtbl.write_vfunc(0x0AD, &Hook_Update);
				INFO("Hook Actor Update!");
			}

		private:
			void Hook_Update(float a_delta);

			static inline REL::Relocation<decltype(&RE::Character::Update)> func;
		};

		class PlayerEx : public RE::PlayerCharacter
		{
		public:
			static void InstallHook()
			{
				REL::Relocation<std::uintptr_t> PlayerCharacterVtbl{ VTABLE[0] };
				func = PlayerCharacterVtbl.write_vfunc(0x0AD, &Hook_Update);
				INFO("Hook Actor Update!");
			}

		private:
			void Hook_Update(float a_delta);

			static inline REL::Relocation<decltype(&RE::PlayerCharacter::Update)> func;
		};

		friend class CharacterEx;
		friend class PlayerEx;

		static float GetPoiseRegenRate();

	private:
		static void UpdatePoiseValue(RE::Actor* a_target, float a_delta);

		PoiseRegenHandler() = delete;
		~PoiseRegenHandler() = delete;
	};

	class RegenDelayHandler
	{
		RegenDelayHandler() = delete;
		~RegenDelayHandler() = delete;

		static constexpr char CURRENT_REGEN_DELAY_GV[] = "MaxsuPoise_RegenDelayTimer";

	public:
		static float GetPoiseRegenDelayTimer(RE::Actor* a_target);
		static bool SetPoiseRegenDelayTimer(RE::Actor* a_target, const float& a_in);
		static float GetMaxRegenDelayTime();
	};

	class StaggerProtectHandler
	{
		StaggerProtectHandler() = delete;
		~StaggerProtectHandler() = delete;

		static constexpr char CURRENT_STAGGER_TIMER_GV[] = "MaxsuPoise_StaggerProtectTimer";

	public:
		static float GetStaggerProtectTimer(RE::Actor* a_target);
		static bool SetStaggerProtectTimer(RE::Actor* a_target, const float& a_in);
		static float GetMaxStaggerProtectTime();
	};

	class RecoveryStateHandler
	{
		RecoveryStateHandler() = delete;
		~RecoveryStateHandler() = delete;

		static constexpr char RECOVERY_STATE[] = "MaxsuPoise_RecoveryState";

	public:
		static int GetRecoveryState(RE::Actor* a_target);
		static bool SetRecoveryState(RE::Actor* a_target, const int& a_in);
	};

}