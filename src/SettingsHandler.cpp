#include "SettingsHandler.h"
#include "SimpleIni.h"
#include "SpecialBar.h"

namespace MaxsuPoise
{
	static constexpr char fileName[] = R"(Data\SKSE\Plugins\MaxsuPoise.ini)";
	static constexpr char extraSettingsFile[] = R"(Data\SKSE\Plugins\MaxsuPoise_ExtraSettings.ini)";

	bool SettingsHandler::Register()
	{
		UpdateWeapTypeMult();
		InitArmorSlotMult();
		InitTrueHUDSpecialBar();

		static SettingsHandler singleton;
		auto eventSource = SKSE::GetModCallbackEventSource();
		if (!eventSource) {
			ERROR("EventSource not found!");
			return false;
		}

		eventSource->AddEventSink(&singleton);

		INFO("Register {}", typeid(singleton).name());

		return true;
	}

	void SettingsHandler::UpdateWeapTypeMult()
	{
		auto& WeapTypeEnumTbl = dku::static_enum<WEAPON_TYPE>();

		CSimpleIniA ini;
		if (ini.LoadFile(fileName))  // Load the ini file
			ERROR("Get Error When loading file {}", fileName);

		// get a pointer to the "WeaponTypeMult" section
		const CSimpleIniA::TKeyVal* section = ini.GetSection("WeaponTypeMult");
		if (!section) {
			return;
		}

		// iterate through the key-value pairs in the section
		for (CSimpleIniA::TKeyVal::const_iterator it = section->begin(); it != section->end(); ++it) {
			const char* key = it->first.pItem;
			const char* value = it->second;
			auto weapEnum = WeapTypeEnumTbl.from_string(key);
			if (weapEnum.has_value()) {
				weapTypeMultMap[weapEnum.value()] = (std::stof(value));
			}
		}
	}

	void SettingsHandler::InitArmorSlotMult()
	{
		auto& BipeSlotEnumTbl = dku::static_enum<BipedSlot>();

		CSimpleIniA ini;
		if (ini.LoadFile(extraSettingsFile))  // Load the ini file
			ERROR("Get Error When loading file {}", extraSettingsFile);

		// get a pointer to the "ArmorSlotMult" section
		const CSimpleIniA::TKeyVal* section = ini.GetSection("ArmorSlotMult");
		if (!section) {
			return;
		}

		// iterate through the key-value pairs in the section
		for (CSimpleIniA::TKeyVal::const_iterator it = section->begin(); it != section->end(); ++it) {
			const char* key = it->first.pItem;
			const char* value = it->second;
			auto armorSlotEnum = BipeSlotEnumTbl.from_string(key);
			if (armorSlotEnum.has_value()) {
				armorSlotMultMap[armorSlotEnum.value()] = (std::stof(value));
			}
		}
	}

	void SettingsHandler::InitTrueHUDSpecialBar()
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(extraSettingsFile);

		bool TrueHUDEnable = ini.GetBoolValue("TrueHUD", "SpecialBar", true);
		INFO("TrueHUDEnable is: {}", TrueHUDEnable)

		if (!TrueHUDEnable) {
			return;
		}

		MaxsuPoise::SpecialBar::GetSingleton()->Initialize();
	}

}
