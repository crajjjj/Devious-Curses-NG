#include "OppDevices.h"

#include "Scripting.h"
#include "Settings.h"
#include "Devices.h"
#include "Sex.h"
#include "MGEF_Controller.h"
#include "Locations.h"
#include "Tats.h"
#include "Events.h"
#include "Translation.h"
#include "Utils.h"
#include "Serializer.h"

#include "SexLabPPFunctions.h"
#include "apis/DDNG_API.h"

using namespace SKSE;

namespace DCURSES {
	ODBitmask GetOppDeviceMask() {
		ODBitmask mask;

		RE::TESObjectARMO* summoner_collar = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(SUMMONER_COLLAR, "Devious Curses.esp");
		RE::TESObjectARMO* summoner_collar_r = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(SUMMONER_COLLAR_R, "Devious Curses.esp");
		RE::TESObjectARMO* latex = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(LIVING_LATEX, "Devious Curses.esp");
		RE::TESObjectARMO* latex_r = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(LIVING_LATEX_R, "Devious Curses.esp");
		RE::TESObjectARMO* latex_open = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(LIVING_LATEX_OPEN, "Devious Curses.esp");
		RE::TESObjectARMO* latex_open_r = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(LIVING_LATEX_OPEN_R, "Devious Curses.esp");
		RE::TESObjectARMO* dwarven_cuirass = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(DWARVEN_CURIAS, "Devious Curses.esp");
		RE::TESObjectARMO* dwarven_cuirass_r = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(DWARVEN_CURIAS_R, "Devious Curses.esp");
		RE::TESObjectARMO* dwarven_heavy = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(DWARVEN_CURIAS_HEAVY, "Devious Curses.esp");
		RE::TESObjectARMO* dwarven_heavy_r = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(DWARVEN_CURIAS_HEAVY_R, "Devious Curses.esp");
		RE::TESObjectARMO* madness_plug = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(MADNESS_PLUG, "Devious Curses.esp");
		RE::TESObjectARMO* madness_plug_r = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(MADNESS_PLUG_R, "Devious Curses.esp");
		RE::TESObjectARMO* nocturnal_piercing = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(NOCTURNAL_PIERCING, "Devious Curses.esp");
		RE::TESObjectARMO* nocturnal_piercing_r = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(NOCTURNAL_PIERCING_R, "Devious Curses.esp");

		auto playerInventory = RE::PlayerCharacter::GetSingleton()->GetInventory();
		for (auto const& [k, v] : playerInventory) {
			RE::TESObjectARMO* armor = k->As<RE::TESObjectARMO>();
			if (armor && v.second->IsWorn()) {
				if (armor == summoner_collar || armor == summoner_collar_r) {
					mask |= ODBitmask::SummonerCollar;
				}
				if (armor == latex || armor == latex_r || armor == latex_open || armor == latex_open_r) {
					mask |= ODBitmask::LivingLatex;
				}
				if (armor == dwarven_cuirass || armor == dwarven_cuirass_r || armor == dwarven_heavy || armor == dwarven_heavy_r) {
					mask |= ODBitmask::DwarvenCuirass;
				}
				if (armor == madness_plug || armor == madness_plug_r) {
					mask |= ODBitmask::MadnessPlug;
				}
				if (armor == nocturnal_piercing || armor == nocturnal_piercing_r) {
					mask |= ODBitmask::NocturnalPiercing;
				}
			}
		}

		return mask;
	}

	bool IsWearingOppLatex(ODBitmask device_mask) {
		if (device_mask == UINT32_MAX) {
			device_mask = GetOppDeviceMask();
		}
		return device_mask & ODBitmask::LivingLatex;
	}

	bool IsWearingOppSummonerCollar(ODBitmask device_mask) {
		if (device_mask == UINT32_MAX) {
			device_mask = GetOppDeviceMask();
		}
		return device_mask & ODBitmask::SummonerCollar;
	}

	bool IsWearingOppMadness(ODBitmask device_mask) {
		if (device_mask == UINT32_MAX) {
			device_mask = GetOppDeviceMask();
		}
		return device_mask & ODBitmask::MadnessPlug;
	}

	bool IsWearingOppNocturnal(ODBitmask device_mask) {
		if (device_mask == UINT32_MAX) {
			device_mask = GetOppDeviceMask();
		}
		return device_mask & ODBitmask::NocturnalPiercing;
	}

	int RemoveDwarvenStuff() {
		static const std::set<RE::FormID> ValidForms = { 0x000AEBF1, 0x000C8861, 0x000C8864, 0x000C8866, 0x000C8868, 0x000C886A, 0x000C886C, 0x000C886E, 0x000C8870, 0x000C8872, 0x000C8874, 0x000C8878 };
		auto player = RE::PlayerCharacter::GetSingleton();
		auto inventory = player->GetInventory();
		int value = 0;
		for (auto const& [k, v] : inventory) {
			if (k->GetFormType() == RE::FormType::Misc && ValidForms.contains(k->formID)) {
				value += k->GetGoldValue() * v.first;
				player->RemoveItem(k, v.first, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
			}
		}
		return value;
	}

	bool OppSummonerCollarEvent(std::string containerName) {
		auto settings = Settings::GetSingleton();

		if (!settings->sexAllowCreature || !settings->sexEnabled) {
			return false;
		}

		auto player = RE::PlayerCharacter::GetSingleton();

		auto currentCollar = GetWornInventoryDeviceByKeyword(player, "zad_DeviousCollar");
		if (currentCollar && !DeviceInventoryIsGeneric(currentCollar)) {
			return false;
		}

		RE::TESObjectARMO* summoner_collar = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(SUMMONER_COLLAR, "Devious Curses.esp");

		auto magic = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka);
		player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kMagicka, -magic);

		ScriptingManager().LockDevice(player, summoner_collar, true);
		if (!containerName.empty()) {
			//PlayerMessage(fmt::format("As you touch the {} you feel very dizzy as a heavy collar forms around your neck and absorbs your magicka!", containerName));
			PlayerMessage(Translator(Translation::ODeviceSummonerCollarStart, containerName), Settings::GetSingleton()->surpriseMode);
		};
		if (Settings::GetSingleton()->eventVisualEffects) {
			ScriptingManager().PlaySummonVFX(player);
		}
		return true;
	}

	bool OppLivingLatexEvent(std::string containerName) {
		auto player = RE::PlayerCharacter::GetSingleton();

		auto currentSuit = GetWornInventoryDeviceByKeyword(player, "zad_DeviousSuit");
		if (currentSuit && !DeviceInventoryIsGeneric(currentSuit)) {
			return false;
		}

		RE::TESObjectARMO* latex = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(LIVING_LATEX, "Devious Curses.esp");
		RE::TESObjectARMO* latex_open = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(LIVING_LATEX_OPEN, "Devious Curses.esp");

		auto stamina = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina);
		player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, -stamina / 2);

		auto scriptManager = ScriptingManager();

		if (Settings::GetSingleton()->oppLivingLatexOpen) {
			scriptManager.LockDevice(player, latex_open, true);
		}
		else {
			scriptManager.LockDevice(player, latex, true);
		}

		if (!containerName.empty()) {
			//PlayerMessage(fmt::format("As you touch the {} a ball of goo jumps out at you and covers your body!", containerName));
			PlayerMessage(Translator(Translation::ODeviceLivingLatexStart, containerName), Settings::GetSingleton()->surpriseMode);
		};
		if (Settings::GetSingleton()->eventVisualEffects) {
			ScriptingManager().PlaySummonVFX(player);
		}
		return true;
	}

	bool OppDwarvenCuirassEvent(std::string containerName, bool skipLocation) {
		auto player = RE::PlayerCharacter::GetSingleton();
		auto settings = Settings::GetSingleton();

		if (settings->oppDwarvenRequireLoc && !skipLocation && GetPlayerLocationType() != LocationType::Dwarven) {
			return false;
		}

		auto scriptManager = ScriptingManager();

		auto currentSuit = GetWornInventoryDeviceByKeyword(player, "zad_DeviousSuit");
		if (currentSuit && !DeviceInventoryIsGeneric(currentSuit)) {
			return false;
		}
		else {
			scriptManager.UnlockDevice(player, currentSuit);
		}

		auto currentHeavy = GetWornInventoryDeviceByKeyword(player, "zad_DeviousHeavyBondage");
		if (currentHeavy && !DeviceInventoryIsGeneric(currentHeavy)) {
			return false;
		}
		else {
			scriptManager.UnlockDevice(player, currentHeavy);
		}

		auto currentArmCuffs = GetWornInventoryDeviceByKeyword(player, "zad_DeviousArmCuffs");
		if (currentArmCuffs && !DeviceInventoryIsGeneric(currentArmCuffs)) {
			return false;
		}
		else {
			scriptManager.UnlockDevice(player, currentArmCuffs);
		}

		auto currentLegCuffs = GetWornInventoryDeviceByKeyword(player, "zad_DeviousLegCuffs");
		if (currentLegCuffs && !DeviceInventoryIsGeneric(currentLegCuffs)) {
			return false;
		}
		else {
			scriptManager.UnlockDevice(player, currentLegCuffs);
		}

		auto currentGloves = GetWornInventoryDeviceByKeyword(player, "zad_DeviousGloves");
		if (currentGloves && !DeviceInventoryIsGeneric(currentGloves)) {
			return false;
		}
		else {
			scriptManager.UnlockDevice(player, currentGloves);
		}

		auto currentBoots = GetWornInventoryDeviceByKeyword(player, "zad_DeviousBoots");
		if (currentBoots && !DeviceInventoryIsGeneric(currentBoots)) {
			return false;
		}
		else {
			scriptManager.UnlockDevice(player, currentBoots);
		}

		RE::TESObjectARMO* dwarven;
		if (settings->oppDwarvenHeavyRestraint) {
			dwarven = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(DWARVEN_CURIAS_HEAVY, "Devious Curses.esp");
		}
		else {
			dwarven = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(DWARVEN_CURIAS, "Devious Curses.esp");
		}

		RemoveDwarvenStuff();

		//Util::ExecuteWithDelay(1500ms, [player, dwarven] {
		scriptManager.LockDevice(player, dwarven, true);
		//});

		if (!containerName.empty()) {
			PlayerMessage(Translator(Translation::ODeviceDwarvenCuirassStart, containerName), Settings::GetSingleton()->surpriseMode);
		};
		if (Settings::GetSingleton()->eventVisualEffects) {
			ScriptingManager().PlaySummonVFX(player);
		}

		return true;
	}

	bool OppMadnessPlugEvent(std::string containerName) {
		auto player = RE::PlayerCharacter::GetSingleton();

		auto scriptManager = ScriptingManager();

		auto currentPlug = GetWornInventoryDeviceByKeyword(player, "zad_DeviousPlugAnal");
		if (currentPlug && !DeviceInventoryIsGeneric(currentPlug)) {
			return false;
		}
		else {
			scriptManager.UnlockDevice(player, currentPlug);
		}

		if (Settings::GetSingleton()->oppMadnessBeltFilter) {
			auto currentBelt = GetWornInventoryDeviceByKeyword(player, "zad_DeviousBelt");
			if (currentBelt && !DeviceInventoryIsGeneric(currentBelt)) {
				return false;
			}

			auto currentVag = GetWornInventoryDeviceByKeyword(player, "zad_DeviousPlugVaginal");
			if (currentVag && !DeviceInventoryIsGeneric(currentVag)) {
				return false;
			}
		}

		RE::TESObjectARMO* madness_plug = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(MADNESS_PLUG, "Devious Curses.esp");
		scriptManager.LockDevice(player, madness_plug, true);

		if (!containerName.empty()) {
			PlayerMessage(Translator(Translation::ODeviceMadnessPlugStart, containerName), Settings::GetSingleton()->surpriseMode);
		};
		if (Settings::GetSingleton()->eventVisualEffects) {
			ScriptingManager().PlaySummonVFX(player);
		}
		return true;
	}

	bool OppNocturnalPiercingEvent(std::string containerName) {
		auto player = RE::PlayerCharacter::GetSingleton();

		auto scriptManager = ScriptingManager();

		auto currentPiercing = GetWornInventoryDeviceByKeyword(player, "zad_DeviousPiercingsVaginal");
		if (currentPiercing && !DeviceInventoryIsGeneric(currentPiercing)) {
			return false;
		}
		else {
			scriptManager.UnlockDevice(player, currentPiercing);
		}

		RE::TESObjectARMO* nocturnal_piercing = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(NOCTURNAL_PIERCING, "Devious Curses.esp");
		scriptManager.LockDevice(player, nocturnal_piercing, true);

		if (!containerName.empty()) {
			PlayerMessage(Translator(Translation::ODeviceNocturnalPiercingStart, containerName), Settings::GetSingleton()->surpriseMode);
		};
		if (Settings::GetSingleton()->eventVisualEffects) {
			ScriptingManager().PlaySummonVFX(player);
		}
		return true;
	}

	bool DoOppDeviceEvent(std::string containerName) {
		auto settings = Settings::GetSingleton();

		if (GetOppDeviceMask() && settings->oppOneAtATime) {
			return false;
		}

		std::vector<std::pair<bool (*)(std::string), double>> events;
		events.push_back(std::make_pair(OppSummonerCollarEvent, settings->oppSummonerCollarWeight));
		events.push_back(std::make_pair(OppLivingLatexEvent, settings->oppLivingLatexWeight));
		auto _OppDwarvenCuirassEvent = [](std::string s) {return OppDwarvenCuirassEvent(s); };
		events.push_back(std::make_pair(_OppDwarvenCuirassEvent, settings->oppDwarvenCuirassWeight));
		events.push_back(std::make_pair(OppMadnessPlugEvent, settings->oppMadnessPlugWeight));
		events.push_back(std::make_pair(OppNocturnalPiercingEvent, settings->oppNocturnalWeight));

		if (Util::VectorGetWeightsSum(events) == 0) {
			return false;
		}

		Util::ShuffleVector(events);

		while (!events.empty()) {
			auto pair = Util::VectorSelectWeighted(events);
			if (!pair.has_value()) {
				return false;
			}
			if (pair.value().first(containerName)) {
				Serialized::GetCounters()->clock_GlobalTicker -= Serialized::GetCounters()->clock_GlobalTicker % ODEVICE_TICK;
				return true;
			}
			events.erase(std::remove(events.begin(), events.end(), pair), events.end());
		}
		return false;
	}

	void OppLatexMagicEvent() {
		auto player = RE::PlayerCharacter::GetSingleton();

		if (IsWearingOppLatex() && Serialized::GetOppDeviceCounters()->livingLatexCounter == 2) {
			std::vector<std::string> skips;
			if (!Settings::GetSingleton()->oppLivingLatexHeavy) {
				skips.push_back("zad_DeviousHeavyBondage");
			}
			DoStandardEvent(player, false, "", "(black & (ebonite | rubber))", 4, 1, skips);
			//PlayerMessage("The latex writhes while being shocked. It seems as if it has been weakened significantly!");
			PlayerMessage(Translator(Translation::ODeviceLivingLatexOnHit));
			Serialized::GetOppDeviceCounters()->livingLatexCounter = 1;
			//SetEffectDescription(LIVING_LATEX_EFFECT, "The latex is clinging to you only by your other devices, removing them should do the trick!");
			SetEffectDescription(LIVING_LATEX_EFFECT, Translator(Translation::EffectLivingLatexCling));
		}
	}

	void OppDeviceOnMagicHitEvent(const RE::TESMagicEffectApplyEvent* magicEvent) {
		auto player = RE::PlayerCharacter::GetSingleton();

		//Living Latex
		auto effect = RE::TESForm::LookupByID(magicEvent->magicEffect)->As<RE::EffectSetting>();
		if (effect &&
			magicEvent->target.get() && magicEvent->target.get()->formID == player->formID &&
			(effect->HasArchetype(RE::EffectSetting::Archetype::kValueModifier) || effect->HasArchetype(RE::EffectSetting::Archetype::kDualValueModifier)) &&
			effect->IsDetrimental() &&
			(effect->data.primaryAV == RE::ActorValue::kHealth || effect->data.secondaryAV == RE::ActorValue::kHealth)) {
			bool damageType = effect->data.resistVariable == RE::ActorValue::kResistShock || Util::FormEditorIdContains(effect, "traprunelightning");
			if (damageType) {
				OppLatexMagicEvent();
			}
		}
	}

	void OppNocturnalHands(bool fromRecast) {
		std::vector<std::pair<bool (*)(RE::Actor*, ScriptingManager&), double>> results;

		results.push_back({ [](RE::Actor* player, ScriptingManager& scriptManager) {
			scriptManager.VibrateEffect(player, 2, 30, true);
			scriptManager.DBGNotification(Translator(Translation::ODeviceNocturnalPiercingVibrate));
			return true;
		} , 10 });

		results.push_back({ [](RE::Actor* player, ScriptingManager& scriptManager) {
			scriptManager.VibrateEffect(player, 5, 30, true);
			scriptManager.DBGNotification(Translator(Translation::ODeviceNocturnalPiercingVibrate));
			return true;
		} , 5 });

		if (fromRecast) {
			results.push_back({ [](RE::Actor* player, ScriptingManager& scriptManager) {
				scriptManager.ModifyArousal(player, 7);
				scriptManager.DBGNotification(Translator(Translation::ODeviceNocturnalPiercingHandsRecast));
				return true;
			} , 100 });

			results.push_back({ [](RE::Actor* player, ScriptingManager& scriptManager) {
				scriptManager.ModifyArousal(player, 19);
				scriptManager.DBGNotification(Translator(Translation::ODeviceNocturnalPiercingHands4));
				return true;
			} , 25 });
		}
		else {
			results.push_back({ [](RE::Actor* player, ScriptingManager& scriptManager) {
				scriptManager.ModifyArousal(player, 4);
				scriptManager.DBGNotification(Translator(Translation::ODeviceNocturnalPiercingHands));
				return true;
			} , 75 });

			results.push_back({ [](RE::Actor* player, ScriptingManager& scriptManager) {
				scriptManager.ModifyArousal(player, 7);
				scriptManager.DBGNotification(Translator(Translation::ODeviceNocturnalPiercingHands2));
				return true;
			} , 50 });

			results.push_back({ [](RE::Actor* player, ScriptingManager& scriptManager) {
				scriptManager.ModifyArousal(player, 11);
				scriptManager.DBGNotification(Translator(Translation::ODeviceNocturnalPiercingHands3));
				return true;
			} , 35 });

			results.push_back({ [](RE::Actor* player, ScriptingManager& scriptManager) {
				scriptManager.ModifyArousal(player, 19);
				scriptManager.DBGNotification(Translator(Translation::ODeviceNocturnalPiercingHands4));
				return true;
			} , 20 });
		}

		Util::ShuffleVector(results);

		auto player = RE::PlayerCharacter::GetSingleton();
		auto scriptManager = ScriptingManager();

		while (!results.empty()) {
			auto pair = Util::VectorSelectWeighted(results);
			if (!pair.has_value()) {
				log::warn("Somehow got error on VectorSelectWeighted with static weights.");
				return;
			}
			if (pair.value().first(player, scriptManager)) {
				return;
			}
			results.erase(std::remove(results.begin(), results.end(), pair), results.end());
		}
		log::warn("Unable to select Nocturnal Event");
	}

	void OppDeviceUpdate() {
		if (Serialized::GetCounters()->clock_GlobalTicker % ODEVICE_TICK == 3) {
			auto player = RE::PlayerCharacter::GetSingleton();
			auto scriptManager = ScriptingManager();
			auto devices = Devices::GetSingleton();
			auto settings = Settings::GetSingleton();

			// Device Weapon Fix
			//auto heavy = GetWornInventoryDeviceByKeyword(player, "zad_DeviousHeavyBondage");
			//if (heavy) {
			//	UnequipItems(player);
			//}

			RE::TESKey* summoner_collar_key = StaticDataHolder::GetSingleton()->LookupForm<RE::TESKey>(SUMMONER_COLLAR_KEY, "Devious Curses.esp");
			RE::TESObjectARMO* summoner_collar = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(SUMMONER_COLLAR, "Devious Curses.esp");
			RE::TESObjectARMO* latex = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(LIVING_LATEX, "Devious Curses.esp");
			RE::TESObjectARMO* latex_open = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(LIVING_LATEX_OPEN, "Devious Curses.esp");
			RE::TESObjectARMO* dwarven_cuirass = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(DWARVEN_CURIAS, "Devious Curses.esp");
			RE::TESObjectARMO* dwarven_heavy = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(DWARVEN_CURIAS_HEAVY, "Devious Curses.esp");
			//RE::TESObjectARMO* nocturnal_piercing = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(NOCTURNAL_PIERCING, "Devious Curses.esp");


			auto oppDeviceMask = GetOppDeviceMask();

			bool isWearingSummonerCollar = oppDeviceMask & ODBitmask::SummonerCollar;
			bool isWearingLatex = oppDeviceMask & ODBitmask::LivingLatex;
			bool isWearingDwarven = oppDeviceMask & ODBitmask::DwarvenCuirass;
			bool isWearingMadness = oppDeviceMask & ODBitmask::MadnessPlug;
			bool isWearingNocturnal = oppDeviceMask & ODBitmask::NocturnalPiercing;

			auto oppdCounters = Serialized::GetOppDeviceCounters();

			if (isWearingSummonerCollar && oppdCounters->summonCollarCounter == INT64_MIN) {
				log::trace("Reseting summoner collar counter.");
				oppdCounters->summonCollarCounter = settings->oppSummonerSexCount;
			}
			else if (!isWearingSummonerCollar) {
				oppdCounters->summonCollarCounter = INT64_MIN;
			}

			if (isWearingLatex && oppdCounters->livingLatexCounter == INT64_MIN) {
				log::trace("Reseting living latex counter.");
				oppdCounters->livingLatexCounter = static_cast<int64_t>(settings->oppLivingLatexStartTime * 60 * Util::random(0.9f, 1.2f));
			}
			else if (!isWearingLatex) {
				oppdCounters->livingLatexCounter = INT64_MIN;
			}

			if (isWearingDwarven && oppdCounters->dwarvenCuirassCounter == INT64_MIN) {
				log::trace("Reseting dwarven cuirass counter.");
				oppdCounters->dwarvenCuirassCounter = settings->oppDwarvenValueNeeded;
				RemoveDwarvenStuff();
			}
			else if (!isWearingDwarven) {
				oppdCounters->dwarvenCuirassCounter = INT64_MIN;
			}

			if (isWearingMadness && oppdCounters->madnessPlugCounter == INT64_MIN) {
				log::trace("Reseting madness plug counter.");
				oppdCounters->madnessPlugCounter = settings->oppMadnessPlugIterations * ODEVICE_MADNESS_DIVISOR + settings->oppMadnessplugOrgasms;
			}
			else if (!isWearingMadness) {
				oppdCounters->madnessPlugCounter = INT64_MIN;
			}

			if (isWearingNocturnal && oppdCounters->nocturnalPiercingCounter == INT64_MIN) {
				log::trace("Reseting nocturnal piercing counter.");
				oppdCounters->nocturnalPiercingCounter = settings->oppNocturnalSexCount;
			}
			else if (!isWearingNocturnal) {
				oppdCounters->nocturnalPiercingCounter = INT64_MIN;
			}

			//Summoner Collar
			if (isWearingSummonerCollar) {
				auto summons_list = getPlayerCommandedActors();
				if (summons_list.empty()) {
					if (Util::random<double>() < settings->oppSummonChance) {
						RE::TESNPC* familiar = StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(SUMMON_FAMILIAR, "Skyrim.esm");
						RE::TESNPC* flame = StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(SUMMON_ATRONACH_FLAME, "Skyrim.esm");
						RE::TESNPC* frost = StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(SUMMON_ATRONACH_FROST, "Skyrim.esm");
						RE::TESNPC* storm = StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(SUMMON_ATRONACH_STORM, "Skyrim.esm");
						RE::TESNPC* dremora = StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x804, "MVC Dremora Summon Replacer Danika.esp");
						if (dremora == nullptr) {
							dremora = StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(SUMMON_DREMORA_LORD, "Skyrim.esm");
						}

						RE::TESNPC* conjure_target = nullptr;

						switch (settings->oppSummonerNPC) {
						case 0: conjure_target = familiar; break;
						case 1: conjure_target = flame; break;
						case 2: conjure_target = frost; break;
						case 3: conjure_target = storm; break;
						case 4: conjure_target = dremora; break;
						case 5: {
							std::vector<RE::TESNPC*> choices = { flame, frost, storm };
							conjure_target = choices[Util::random(choices.size())];
							break;
						}
						case 6: {
							std::vector<RE::TESNPC*> choices = { familiar, flame, frost, storm, dremora };
							conjure_target = choices[Util::random(choices.size())];
							break;
						}
						}

						if (conjure_target) {

							RE::SpellItem* conjure_spell = StaticDataHolder::GetSingleton()->LookupForm<RE::SpellItem>(SUMMONER_SUMMON_SPELL, "Devious Curses.esp");
							RE::EffectSetting* conjure_effect = StaticDataHolder::GetSingleton()->LookupForm<RE::EffectSetting>(SOMMONER_COLLAR_SUMMON_EFFECT, "Devious Curses.esp");
							conjure_effect->data.associatedForm = conjure_target;
							RE::MagicCaster* caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kOther);
							caster->CastSpellImmediate(conjure_spell, false, player, 3.0f, false, 0.0f, nullptr);
							auto magic = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka);
							if (settings->oppSCollarDrainsMagicka) player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kMagicka, -magic);

							auto conjure_base_id = conjure_target->formID;

							std::thread{ [conjure_base_id] {
								for (int i = 0; i < 10; i++) {
									std::this_thread::sleep_for(100ms);
									auto summons = getPlayerCommandedActors();
									for (auto summon : summons) {
										if (summon->GetActorBase()->formID == conjure_base_id) {
											Serialized::GetOppDeviceCounters()->summonCollarTarget = summon->formID;
											log::trace("Marking actor {} as summoned.", summon->GetName());
											return;
										}
									}
								}
								log::warn("Unable to mark any actor as summoned.");
							} }.detach();
						}
					}
				}
				else {
					if (settings->sexEnabled && Serialized::GetCounters()->clock_lastSex >= settings->sexCooldown && Util::random() < settings->oppSummonSexChance) {
						Util::ShuffleVector(summons_list);
						for (auto summon : summons_list) {
							if (SexActorFilter(summon, false)) {
								scriptManager.StartSex(summon, settings->sexAggressiveAnims);
								Serialized::GetCounters()->clock_lastSex = -10;
								break;
							}
						}
					}
				}

				if (oppdCounters->summonCollarCounter <= 0) {
					if (GetItemCount(player, summoner_collar_key) == 0 && !settings->oppSCollarAutoRemove) {
						player->AddObjectToContainer(skyrim_cast<RE::TESBoundObject*>(summoner_collar_key), nullptr, 1, nullptr);
						//PlayerMessage("The Summoner Collar is now satisfied and you can unlock it!");
						PlayerMessage(Translator(Translation::ODeviceSummonerCollarUnlock));
					}
					else if (settings->oppSCollarAutoRemove) {
						player->RemoveItem(skyrim_cast<RE::TESBoundObject*>(summoner_collar_key), 5, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
						scriptManager.UnlockDevice(player, summoner_collar, nullptr, nullptr, true, false);
						PlayerMessage(Translator(Translation::ODeviceSummonerCollarRemove));
						if (Settings::GetSingleton()->eventVisualEffects) { ScriptingManager().PlaySummonVFX(player); }
					}
				}
				SetEffectMagnitude(SUMMONER_COLLAR_EFFECT, static_cast<float>(oppdCounters->summonCollarCounter));
			}

			//Living Latex

			if (isWearingLatex) {
				if (oppdCounters->livingLatexCounter >= 3) {
					//SetEffectDescription(LIVING_LATEX_EFFECT, "The latex suit seems to be dormant.");
					SetEffectDescription(LIVING_LATEX_EFFECT, Translator(Translation::EffectLivingLatexWait));
					oppdCounters->livingLatexCounter -= ODEVICE_TICK;
					if (oppdCounters->livingLatexCounter <= 2) {
						//SetEffectDescription(LIVING_LATEX_EFFECT, "The latex suit seems to be dormant. Maybe it would do something if it was attacked.");
						SetEffectDescription(LIVING_LATEX_EFFECT, Translator(Translation::EffectLivingLatexDormant));
						oppdCounters->livingLatexCounter = 3;
						if (player->IsInCombat()) {
							std::vector<std::string> skips;
							if (!settings->oppLivingLatexHeavy) {
								skips.push_back("zad_DeviousHeavyBondage");
							}
							else {
								UnequipItems(player);
							}
							DoStandardEvent(player, false, "", "(black & (ebonite | rubber)) | plugpumps", 20, 1, skips);
							PlayerMessage(Translator(Translation::ODeviceLivingLatexTrigger));
							if (Settings::GetSingleton()->eventVisualEffects) { ScriptingManager().PlaySummonVFX(player); }
							SetEffectDescription(LIVING_LATEX_EFFECT, Translator(Translation::EffectLivingLatexAwoken));
							oppdCounters->livingLatexCounter = 2;
						}
					}
				}
				else if (oppdCounters->livingLatexCounter == 2) {
					//SetEffectDescription(LIVING_LATEX_EFFECT, "The latex has awoken! Maybe you can weaken it by shocking it.");
					SetEffectDescription(LIVING_LATEX_EFFECT, Translator(Translation::EffectLivingLatexAwoken));
					if (settings->oppLivingLatexMore > 0.0f && Serialized::GetCounters()->clock_GlobalTicker % static_cast<uint64_t>(settings->oppLivingLatexMore * 60) < ODEVICE_TICK) {
						std::vector<std::string> skips;
						if (!settings->oppLivingLatexHeavy) {
							skips.push_back("zad_DeviousHeavyBondage");
						}
						if (DoStandardEvent(player, false, "", "(black & (ebonite | rubber))", Util::random(1, 2), 1, skips)) {
							//DBGNotification("The latex has spread over your body!");
							PlayerMessage(Translator(Translation::ODeviceLivingLatexSpread));
							if (Settings::GetSingleton()->eventVisualEffects) { ScriptingManager().PlaySummonVFX(player); }
						}
					}
				}
				else if (oppdCounters->livingLatexCounter <= 1) {
					if (GetWornDeviceCount(player, "(black & (ebonite | rubber))") == 0 || !settings->oppLivingLatexRequireRem) {
						scriptManager.UnlockDevice(player, latex, nullptr, nullptr, true, false);
						scriptManager.UnlockDevice(player, latex_open, nullptr, nullptr, true, false);
						//PlayerMessage("The latex suit dissolves from your body!");
						PlayerMessage(Translator(Translation::ODeviceLivingLatexRemove));
						if (Settings::GetSingleton()->eventVisualEffects) { ScriptingManager().PlaySummonVFX(player); }
					}
					else {
						//SetEffectDescription(LIVING_LATEX_EFFECT, "The latex is clinging to you only by your other devices, removing them should do the trick!");
						SetEffectDescription(LIVING_LATEX_EFFECT, Translator(Translation::EffectLivingLatexCling));
					}
				}
			}

			//Dwarven Cuirass

			if (isWearingDwarven) {
				int playerArousal = scriptManager.GetArousal(player);
				RE::TESObjectARMO* belt = nullptr;
				bool canPiercingN = true;
				bool canPiercingV = true;
				bool canPlugA = true;
				bool canPlugV = !settings->useGenderedPlugs || SexLab::GetSex(player) == 1;
				bool canCollar = true;
				auto playerInventory = player->GetInventory();
				for (auto const& [k, v] : playerInventory) {
					if (v.second.get()->IsWorn()) {
						RE::TESObjectARMO* wornArmor = k->As<RE::TESObjectARMO>();
						if (!wornArmor) {
							continue;
						}
						RE::TESObjectARMO* rend = DeviousDevicesAPI::g_API->GetDeviceRender(wornArmor);
						if (!rend) {
							continue;
						}
						for (std::string kw : GetDeviceKeywords(rend, false)) {
							//log::trace("kw: {}", kw);
							if (kw == "zad_DeviousBelt") {
								belt = rend;
							}
							else if (kw == "zad_DeviousPiercingsNipple") {
								canPiercingN = false;
							}
							else if (kw == "zad_DeviousPiercingsVaginal") {
								canPiercingV = false;
							}
							else if (kw == "zad_DeviousPlugAnal") {
								canPlugA = false;
							}
							else if (kw == "zad_DeviousPlugVaginal") {
								canPlugV = false;
							}
							else if (kw == "zad_DeviousCollar") {
								canCollar = false;
							}
						}
					}
				}

				if (belt && DeviceInventoryIsGeneric(belt) && Util::random() <= 10) {
					scriptManager.UnlockDevice(player, belt, nullptr, nullptr, true);
					PlayerMessage(Translator(Translation::ODeviceDwarvenCuirassDestroyBelt));
				}
				else if (!belt && playerArousal > settings->oppDwarvenArousal && Serialized::GetCounters()->clock_lastSex > 45 && (GetDeviceMask(player) & 0b0010) != 0 && Util::random() <= 10) {
					//Util::ExecuteWithDelay(500ms, [player] {
						//ScriptingManager().StartMasturbation(player);
					//});
					PlayerMessage(Translator(Translation::ODeviceDwarvenCuirassMasturbate));
					StartMasturbationImpl();
				}
				else if (oppdCounters->dwarvenCuirassCounter <= settings->oppDwarvenValueNeeded / 2 && (canPiercingN || canPiercingV || canPlugA || canPlugV || canCollar) && Util::random() <= 10) {
					DeviceList* deviceList;
					std::vector<std::pair<DeviceList*, double>> list;
					if (canPiercingN) { list.push_back({ &devices->piercingsN, 10.0 }); }
					if (canPiercingV) { list.push_back({ &devices->piercingsV, 10.0 }); }
					if (canPlugA) { list.push_back({ &devices->plugsA, 30.0 }); }
					if (canPlugV) { list.push_back({ &devices->plugsV, 30.0 }); }
					if (canCollar) {
						auto collars = GetAdjustedDeviceList(&devices->collars, {}, "metal | steel | rust | lustr");
						list.push_back({ &collars, 20.0 });
					}
					auto potentialList = Util::VectorSelectWeighted(list);
					if (potentialList.has_value()) {
						deviceList = potentialList.value().first;

						auto device = GetRandomDevice(deviceList);
						if (device.has_value()) {
							scriptManager.LockDevice(player, device->inv);

							oppdCounters->dwarvenCuirassCounter += Util::random(4, 20);
							PlayerMessage(Translator(Translation::ODeviceDwarvenCuirassCraft, device->inv->GetName()));
							if (Settings::GetSingleton()->eventVisualEffects) { ScriptingManager().PlaySummonVFX(player); }
						}
					}
				}
				else {
					if (Util::random() <= 15) {
						int value = RemoveDwarvenStuff();
						if (value > 0) {
							oppdCounters->dwarvenCuirassCounter -= value;
							scriptManager.DBGNotification(Translator(Translation::ODeviceDwarvenCuirassEat));
						}
					}
					else if (oppdCounters->dwarvenCuirassCounter <= 0) {
						RE::TESNPC* thing = nullptr;
						auto level = player->GetLevel();
						if (level >= 50) {
							std::vector<std::pair<RE::TESNPC*, double>> list;
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10ec8e, "Skyrim.esm"), 10.0 }); //Sphere Master
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10e753, "Skyrim.esm"), 20.0 }); //Centurion Guardian
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x23a96, "Skyrim.esm"), 30.0 }); //Centurion Guardian
							thing = Util::VectorSelectWeighted(list).value().first;
						}
						else if (level >= 38) {
							std::vector<std::pair<RE::TESNPC*, double>> list;
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x23a97, "Skyrim.esm"), 10.0 }); //Sphere Guardian
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10f9b9, "Skyrim.esm"), 10.0 }); //Centurion
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10ec8e, "Skyrim.esm"), 30.0 }); //Sphere Master
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10e753, "Skyrim.esm"), 30.0 }); //Centurion Guardian
							thing = Util::VectorSelectWeighted(list).value().first;
						}
						else if (level >= 30) {
							std::vector<std::pair<RE::TESNPC*, double>> list;
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10ec89, "Skyrim.esm"), 10.0 }); //Sphere
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10ec87, "Skyrim.esm"), 10.0 }); //Spider Guardian
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x23a97, "Skyrim.esm"), 30.0 }); //Sphere Guardian
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10f9b9, "Skyrim.esm"), 30.0 }); //Centurion
							thing = Util::VectorSelectWeighted(list).value().first;
						}
						else if (level >= 24) {
							std::vector<std::pair<RE::TESNPC*, double>> list;
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x23a98, "Skyrim.esm"), 10.0 }); //Spider
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10ec89, "Skyrim.esm"), 30.0 }); //Sphere
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10ec87, "Skyrim.esm"), 30.0 }); //Spider Guardian
							thing = Util::VectorSelectWeighted(list).value().first;
						}
						else if (level >= 16) {
							std::vector<std::pair<RE::TESNPC*, double>> list;
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10ec86, "Skyrim.esm"), 10.0 }); //Spider Worker
							list.push_back({ StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x23a98, "Skyrim.esm"), 20.0 }); //Spider
							thing = Util::VectorSelectWeighted(list).value().first;
						}
						else {
							thing = StaticDataHolder::GetSingleton()->LookupForm<RE::TESNPC>(0x10ec86, "Skyrim.esm"); //Spider Worker
						}

						Util::ExecuteWithDelay(1000ms, [player, thing] {
							auto thing_ref = player->PlaceObjectAtMe(thing, false).get();
							if (Settings::GetSingleton()->eventVisualEffects) { ScriptingManager().PlaySummonVFX(thing_ref); }
							});

						PlayerMessage(Translator(Translation::ODeviceDwarvenCuirassRemove, thing->GetName()));

						scriptManager.UnlockDevice(player, dwarven_cuirass, nullptr, nullptr, true, false);
						scriptManager.UnlockDevice(player, dwarven_heavy, nullptr, nullptr, true, false);
					}
				}
				SetEffectMagnitude(DWARVEN_CUIRASS_EFFECT, static_cast<float>(oppdCounters->dwarvenCuirassCounter));
			}

			//Madness Plug
			if (isWearingMadness) {
				auto sexCountTotal = (oppdCounters->madnessPlugCounter / ODEVICE_MADNESS_DIVISOR) * settings->oppMadnessplugOrgasms + oppdCounters->madnessPlugCounter % ODEVICE_MADNESS_DIVISOR;
				SetEffectMagnitude(MADNESS_PLUG_EFFECT, static_cast<float>(sexCountTotal));
				if (settings->oppMadnessOnlyDevices) {
					SetEffectDescription(MADNESS_PLUG_EFFECT, Translator(Translation::ODeviceMadnessDescription));
				}
				else {
					SetEffectDescription(MADNESS_PLUG_EFFECT, Translator(Translation::ODeviceMadnessDescriptionSex));
				}

				if (CheckUD()) {
					if (Util::random<float>() < 0.5f * settings->oppMadnessMultiplier) {
						if (!settings->surpriseMode) { scriptManager.DBGNotification(Translator(Translation::ODeviceMadnessPlugLongVib)); }
						for (int i = 0; i < 10; i++) {
							scriptManager.VibrateEffect(player, 1, 60, false, true);
						}
					}
					else if (Util::random<float>() < 2.0f * settings->oppMadnessMultiplier) {
						if (!settings->surpriseMode) { scriptManager.DBGNotification(Translator(Translation::ODeviceMadnessPlugMediumVib)); }
						for (int i = 0; i < 8; i++) {
							scriptManager.VibrateEffect(player, 3, 15, false, true);
						}
					}
					else if (Util::random<float>() < 0.5f * settings->oppMadnessMultiplier) {
						if (!settings->surpriseMode) { scriptManager.DBGNotification(Translator(Translation::ODeviceMadnessPlugSuperVib)); }
						for (int i = 0; i < 10; i++) {
							scriptManager.VibrateEffect(player, 6, 10, false, true);
						}
					}
				}
				else {
					if (Util::random<float>() < 1.0f * settings->oppMadnessMultiplier) {
						if (!settings->surpriseMode) { scriptManager.DBGNotification(Translator(Translation::ODeviceMadnessPlugOrgasm)); }
						scriptManager.ActorDeviceOrgasm(player);
					}
					else if (Util::random<float>() < 0.5f * settings->oppMadnessMultiplier) {
						if (!settings->surpriseMode) { scriptManager.DBGNotification(Translator(Translation::ODeviceMadnessPlugLongVib)); }
						for (int i = 0; i < 4; i++) {
							scriptManager.ModifyArousal(player, 15);
							scriptManager.VibrateEffect(player, 1, 60, false, true);
						}
					}
					else if (Util::random<float>() < 2.0f * settings->oppMadnessMultiplier) {
						if (!settings->surpriseMode) { scriptManager.DBGNotification(Translator(Translation::ODeviceMadnessPlugMediumVib)); }
						for (int i = 0; i < 3; i++) {
							scriptManager.ModifyArousal(player, 33);
							scriptManager.VibrateEffect(player, 3, 30, false, true);
						}
					}
					else if (Util::random<float>() < 0.5f * settings->oppMadnessMultiplier) {
						if (!settings->surpriseMode) { scriptManager.DBGNotification(Translator(Translation::ODeviceMadnessPlugSuperVib)); }
						for (int i = 0; i < 4; i++) {
							scriptManager.ModifyArousal(player, 100);
							scriptManager.VibrateEffect(player, 10, 15, false, true);
						}
					}
				}
			}

			//Nocturnal Piercing
			if (isWearingNocturnal) {
				auto invis_effect = GetEffectByFormID(NOCTURNAL_INVIS_EFFECT);
				RE::SpellItem* nocturnal_invis = StaticDataHolder::GetSingleton()->LookupForm<RE::SpellItem>(NOCTURNAL_INVIS_SPELL, "Devious Curses.esp");

				if (invis_effect) {
					//log::trace("Is invisible");

					if (scriptManager.GetArousal(player) >= settings->oppNocturnalArousal) {
						RE::MagicTarget* target = player->GetMagicTarget();
						auto player_handle = player->GetHandle();
						target->DispelEffect(nocturnal_invis, player_handle);
					}
					else {
						invis_effect->elapsedSeconds = 1.0;
					}
				}
				else {
					//log::trace("Is not invisible");
					if (scriptManager.GetArousal(player) < settings->oppNocturnalArousal) {
						//log::trace("Attempting invis cast");
						RE::MagicCaster* caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kOther);
						caster->CastSpellImmediate(nocturnal_invis, false, player, 1.0f, false, 0.0f, nullptr);
					}
				}

				if (Util::random() < settings->oppNocturnalHandChance) {
					OppNocturnalHands();
				}

				SetEffectMagnitude(NOCTURNAL_PIERCING_EFFECT, static_cast<float>(oppdCounters->nocturnalPiercingCounter));
			}
		}
	}

	void OppDoMadnessEffect(bool ending_event) {
		std::vector<std::pair<bool (*)(RE::Actor*, ScriptingManager&), double>> results;

		if (!ending_event) {
			results.push_back({ [](RE::Actor* player, ScriptingManager& scriptManager) {// Piercings
				auto currentPiercings = GetWornInventoryDeviceByKeyword(player, "zad_DeviousPiercingsNipple");
				log::trace("Has Piercings: {}", (bool)currentPiercings);
				if (currentPiercings && !DeviceInventoryIsGeneric(currentPiercings)) {
					return false;
				}
				log::trace("Madness Piercings");
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugPiercings), Settings::GetSingleton()->surpriseMode);
				if (Settings::GetSingleton()->eventVisualEffects) { ScriptingManager().PlaySummonVFX(player); }
				RE::TESObjectARMO* piercings = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(MADNESS_PIERCINGS, "Devious Curses.esp");
				scriptManager.SwapDevices(player, piercings);
				return true;
			} , 5000 });

			results.push_back({ [](RE::Actor*, ScriptingManager&) {// Increase Orgasms
				if (Serialized::GetOppDeviceCounters()->madnessPlugCounter <= 0) { return false; }
				int r = static_cast<int>(std::max(Util::random(0.25, 2.0) * Settings::GetSingleton()->oppMadnessplugOrgasms, 1.0));
				Serialized::GetOppDeviceCounters()->madnessPlugCounter += r;
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugDesire, r), Settings::GetSingleton()->surpriseMode);
				return true;
			} , 25 });

			results.push_back({ [](RE::Actor* player, ScriptingManager&) {// Standard Event
				if (GetWornDeviceCount(player) <= Settings::GetSingleton()->maxRestraints && DoStandardEvent(player, false, "")) {
					log::trace("Madness Standard Event");
					PlayerMessage(Translator(Translation::ODeviceMadnessPlugStandard), Settings::GetSingleton()->surpriseMode);
					return true;
				}
				return false;
			} , 100 });

			results.push_back({ [](RE::Actor* player, ScriptingManager&) {// Boss Event
				if (DoStandardEvent(player, true, "", "", -1, Settings::GetSingleton()->bossAditionalRestraints)) {
					log::trace("Madness Boss Event");
					PlayerMessage(Translator(Translation::ODeviceMadnessPlugBoss), Settings::GetSingleton()->surpriseMode);
					return true;
				}
				return false;
			} , 75 });

			results.push_back({ [](RE::Actor*, ScriptingManager&) {// Tie Followers
			auto followers = Util::GetFollowers();
			if (followers.size() > 0 && Settings::GetSingleton()->followerBondageEvent) {
				auto didSomething = false;
				for (auto follower : followers) {
					log::trace("{} devices: {}", follower->GetName(), GetWornDeviceCount(follower));
					if (GetWornDeviceCount(follower) <= Settings::GetSingleton()->maxRestraints) {
						didSomething |= DoStandardEvent(follower, false, "", Settings::GetSingleton()->followerOverrideTheme);
					}
				}
				if (!didSomething) { return false; }
				log::trace("Tie Followers");
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugFollowers), Settings::GetSingleton()->surpriseMode);
				return true;
			}
			return false;
			} , 60 });
		}

		results.push_back({ [](RE::Actor* player, ScriptingManager&) {// Full Tie
			if (DoStandardEvent(player, false, "", "", 20, 5)) {
				log::trace("Madness Full Tie");
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugFullTie), Settings::GetSingleton()->surpriseMode);
				return true;
			}
			return false;
		} , 25 });

		results.push_back({ [](RE::Actor*, ScriptingManager&) {// Full Tie Followers
			auto followers = Util::GetFollowers();
			if (followers.size() > 0 && Settings::GetSingleton()->followerBondageEvent) {
				auto didSomething = false;
				for (auto follower : followers) {
					log::trace("{} devices: {}", follower->GetName(), GetWornDeviceCount(follower));
					if (GetWornDeviceCount(follower) <= 15 + Settings::GetSingleton()->followerDeviceModifier) {
						didSomething |= DoStandardEvent(follower, false, "", Settings::GetSingleton()->followerOverrideTheme, 15, 5);
					}
				}
				if (!didSomething) { return false; }
				log::trace("Tie Followers");
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugFullTieFollowers), Settings::GetSingleton()->surpriseMode);
				return true;
			}
			return false;
		} , 30 });

		results.push_back({ [](RE::Actor*, ScriptingManager&) {// Lewd Mark
			if (GetLewdMark() == MARK::TAT_NONE) {
				DoLewdMarkEvent("", Util::random(0.3,0.8));
				log::trace("Madness Lewd Mark");
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugMark), Settings::GetSingleton()->surpriseMode);
				return true;
			}
			return false;
		} , 75 });

		results.push_back({ [](RE::Actor* player, ScriptingManager&) {// Full Tattoos
			if (CheckRapeTattoos() && GetTattooCount(player) <= 7) {
				//scriptManager.RTDoTattooEvent(player, 20);
				DoTattooEvent(player, "", 20, true);
				log::trace("Madness Full Tattoos");
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugTattoos), Settings::GetSingleton()->surpriseMode);
				return true;
			}
			return false;
		} , 50 });

		results.push_back({ [](RE::Actor*, ScriptingManager&) {// Full Tattoos Followers
			auto followers = Util::GetFollowers();
			if (CheckRapeTattoos() && followers.size() > 0 && Settings::GetSingleton()->followerTattooEvent) {
				auto didSomething = false;
				for (auto follower : followers) {
					if (GetTattooCount(follower) <= 7) {
						didSomething |= DoTattooEvent(follower, "", 20, true);
					}
				}
				if (!didSomething) { return false; }
				log::trace("Madness Full Tattoos Followers");
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugTattooFollowers), Settings::GetSingleton()->surpriseMode);
				return true;
			}
			return false;
		} , 50 });

		results.push_back({ [](RE::Actor* player, ScriptingManager& scriptManager) {// Launch
			log::trace("Madness Launch");
			scriptManager.PushActorAway(player, player, 15);
			return true;
		} , 10 });

		results.push_back({ [](RE::Actor* player, ScriptingManager&) {// Paralyze
			log::trace("Madness Paralyze");
			RE::SpellItem* paralyze = StaticDataHolder::GetSingleton()->LookupForm<RE::SpellItem>(0x5AD5F, "Skyrim.esm");
			paralyze->data.delivery = RE::MagicSystem::Delivery::kSelf;
			RE::MagicCaster* caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kOther);
			caster->CastSpellImmediate(paralyze, false, player, 1.0f, false, 0.0f, nullptr);
			paralyze->data.delivery = RE::MagicSystem::Delivery::kAimed;
			return true;
		} , 10 });


		if (!ending_event && Settings::GetSingleton()->oppMadnessChaos) {
			results.push_back({ [](RE::Actor* player, ScriptingManager&) {// Stat Loss
				log::trace("Madness Stat Loss");
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugStats));
				auto avOwner = player->AsActorValueOwner();
				if (avOwner->GetBaseActorValue(RE::ActorValue::kHealth) > 50) {
					avOwner->SetBaseActorValue(RE::ActorValue::kHealth, avOwner->GetBaseActorValue(RE::ActorValue::kHealth) - 10);
				}
				if (avOwner->GetBaseActorValue(RE::ActorValue::kHealth) > 50) {
					avOwner->SetBaseActorValue(RE::ActorValue::kStamina, avOwner->GetBaseActorValue(RE::ActorValue::kStamina) - 10);
				}
				if (avOwner->GetBaseActorValue(RE::ActorValue::kHealth) > 50) {
					avOwner->SetBaseActorValue(RE::ActorValue::kMagicka, avOwner->GetBaseActorValue(RE::ActorValue::kMagicka) - 10);
				}
				return true;
			} , 10 });

			results.push_back({ [](RE::Actor*, ScriptingManager&) {// Increase Events
				Serialized::GetOppDeviceCounters()->madnessPlugCounter += ODEVICE_MADNESS_DIVISOR;
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugHunger), Settings::GetSingleton()->surpriseMode);
				return true;
			} , 7 });

			results.push_back({ [](RE::Actor* player, ScriptingManager&) {// Gold Loss
				log::trace("Madness Gold Loss");
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugGold), Settings::GetSingleton()->surpriseMode);
				auto inventory = player->GetInventory();
				RE::TESForm* gold = RE::TESForm::LookupByID(std::stoi("0f", 0, 16));
				for (auto const& [k, v] : inventory) {
					if (k == gold) {
						player->RemoveItem(skyrim_cast<RE::TESBoundObject*>(gold), v.first, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
						return true;
					}
				}

				return false;
			} , 30 });

			results.push_back({ [](RE::Actor*, ScriptingManager& scriptManager) {// Crash
				log::trace("Madness Crash (LOL)");
				scriptManager.RequestSaveGame();
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugCrash));
				Util::ExecuteWithDelay(4s, [] {
					log::trace("Wow, such crash {}", *((int*)0));
				});
				return true;
			} , 5 });
		}

		Util::ShuffleVector(results);

		auto player = RE::PlayerCharacter::GetSingleton();
		auto scriptManager = ScriptingManager();

		while (!results.empty()) {
			auto pair = Util::VectorSelectWeighted(results);
			if (!pair.has_value()) {
				log::warn("Somehow got error on VectorSelectWeighted with static weights.");
				break;
			}
			if (pair.value().first(player, scriptManager)) {
				return;
			}
			results.erase(std::remove(results.begin(), results.end(), pair), results.end());
		}
		log::warn("Unable to select Madness Event");
	}

	void OppOnSexEnd(RE::Actor* actor) {
		if (getIsPlayerCommandedActor(actor) && IsWearingOppSummonerCollar()) {
			if (actor->formID == Serialized::GetOppDeviceCounters()->summonCollarTarget) {
				actor->KillImmediate();
				Serialized::GetOppDeviceCounters()->summonCollarTarget = 0;
			}
			if (Serialized::GetOppDeviceCounters()->summonCollarCounter >= 0) {
				Serialized::GetOppDeviceCounters()->summonCollarCounter -= 1;
				SetEffectMagnitude(SUMMONER_COLLAR_EFFECT, static_cast<float>(Serialized::GetOppDeviceCounters()->summonCollarCounter));
			}
		}
	}

	void OppNocturnalPlayerSex(const RE::BSTArray<RE::Actor*>& partners) {
		if (IsWearingOppNocturnal()) {
			auto player = RE::PlayerCharacter::GetSingleton();
			auto watching_actors = Util::GetWatchingActors(player);
			// The people in the scene face the player too, but only bystanders count as spectators.
			std::erase_if(watching_actors, [&partners](RE::Actor* actor) {
				return std::find(partners.begin(), partners.end(), actor) != partners.end();
			});
			auto oppdCounters = Serialized::GetOppDeviceCounters();
			log::trace("Sex: {} watching", watching_actors.size());
			if (watching_actors.size() >= Settings::GetSingleton()->oppNocturnalPeople) {
				oppdCounters->nocturnalPiercingCounter -= 1;
				if (oppdCounters->nocturnalPiercingCounter > 0 && DoStandardEvent(player, false, "", "black & leather", 2, 1, { "zad_DeviousHeavyBondage" })) {
					PlayerMessage(Translator(Translation::ODeviceNocturnalPiercingAnger));
					if (Settings::GetSingleton()->eventVisualEffects) { ScriptingManager().PlaySummonVFX(player); }
				}
				else if (oppdCounters->nocturnalPiercingCounter <= 0) {
					RE::TESObjectARMO* nocturnal_piercing = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(NOCTURNAL_PIERCING, "Devious Curses.esp");
					ScriptingManager().UnlockDevice(player, nocturnal_piercing, nullptr, nullptr, true, false);
					DoStandardEvent(player, false, "", "(black & leather) | plug | piercing", 20, 1, {});
					PlayerMessage(Translator(Translation::ODeviceNocturnalPiercingRemove));
					if (Settings::GetSingleton()->eventVisualEffects) { ScriptingManager().PlaySummonVFX(player); }
				}
			}
		}
	}

	void OppMadnessPlayerOrgasm() {

		//Madness Plug
		if (IsWearingOppMadness()) {
			auto player = RE::PlayerCharacter::GetSingleton();
			auto oppdCounters = Serialized::GetOppDeviceCounters();
			oppdCounters->madnessPlugCounter -= 1;
			auto orgasmCountTotal = (oppdCounters->madnessPlugCounter / ODEVICE_MADNESS_DIVISOR) * Settings::GetSingleton()->oppMadnessplugOrgasms + oppdCounters->madnessPlugCounter % ODEVICE_MADNESS_DIVISOR;
			SetEffectMagnitude(MADNESS_PLUG_EFFECT, static_cast<float>(orgasmCountTotal));
			if (oppdCounters->madnessPlugCounter <= 0) {
				RE::TESObjectARMO* madness_plug = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(MADNESS_PLUG, "Devious Curses.esp");
				ScriptingManager().UnlockDevice(player, madness_plug, nullptr, nullptr, true, false);
				RE::TESObjectARMO* madness_piercings = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectARMO>(MADNESS_PIERCINGS, "Devious Curses.esp");
				ScriptingManager().UnlockDevice(player, madness_piercings, nullptr, nullptr, true, false);
				PlayerMessage(Translator(Translation::ODeviceMadnessPlugRemove));
				if (Settings::GetSingleton()->eventVisualEffects) { ScriptingManager().PlaySummonVFX(player); }
				Util::ExecuteWithDelay(500ms, [player] {
					OppDoMadnessEffect(true);
					});
				Util::ExecuteWithDelay(100ms, [player] {
					auto device_count = Util::random(10, 20);
					for (int i = 0; i < device_count; i++) {
						auto dev = GetRandomDevice(&Devices::GetSingleton()->anything);
						//activatedObject->GetContainer()->AddObjectToContainer((RE::TESBoundObject*)pair->first, 1, GetPlayer());
						if (dev) {
							auto ptr = player->PlaceObjectAtMe(dev.value().inv, false);
							RE::TESObjectREFR* refr = ptr.get();
							refr->data.location += RE::NiPoint3(Util::random<float>(-50, 50), Util::random<float>(-50, 50), Util::random<float>(50, 150));
							refr->MoveHavok(true);
							//activatedObject->AddObjectToContainer((RE::TESBoundObject*)dev.value().inv, nullptr, 1, nullptr);
						}
					}

					std::vector<std::pair<RE::TESForm*, int>> gold = {
						{RE::TESForm::LookupByID(0x0f),		Util::random(5,10)},
						{RE::TESForm::LookupByID(0xD790C),	Util::random(5,15)},
						{RE::TESForm::LookupByID(0xD8E7F),	Util::random(3,10)},
						{RE::TESForm::LookupByID(0xD8E80),	Util::random(1,5)},
						{RE::TESForm::LookupByID(0x5ACDE),	Util::random(2,5)},
						{RE::TESForm::LookupByID(0x5AD9E),	Util::random(1,3)},
					};
					for (auto& [gold_form, count] : gold) {
						for (int i = 0; i < count; i++) {
							auto ptr = player->PlaceObjectAtMe(skyrim_cast<RE::TESBoundObject*>(gold_form), false);
							RE::TESObjectREFR* refr = ptr.get();
							refr->data.location += RE::NiPoint3(Util::random<float>(-50, 50), Util::random<float>(-50, 50), Util::random<float>(50, 150));
							refr->MoveHavok(true);
							//activatedObject->AddObjectToContainer((RE::TESBoundObject*)dev.value().inv, nullptr, 1, nullptr); 
						}
					}
					});
			}
			else if (oppdCounters->madnessPlugCounter % ODEVICE_MADNESS_DIVISOR == 0) {
				oppdCounters->madnessPlugCounter -= ODEVICE_MADNESS_DIVISOR;
				oppdCounters->madnessPlugCounter += Settings::GetSingleton()->oppMadnessplugOrgasms;

				OppDoMadnessEffect();
			}
		}
	}

	void OppNocturnalTryRecast() {
		if (IsWearingOppNocturnal()) {
			auto player = RE::PlayerCharacter::GetSingleton();
			if (ScriptingManager().GetArousal(player) < Settings::GetSingleton()->oppNocturnalArousal) {
				RE::SpellItem* nocturnal_invis = StaticDataHolder::GetSingleton()->LookupForm<RE::SpellItem>(NOCTURNAL_INVIS_SPELL, "Devious Curses.esp");
				RE::MagicCaster* caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kOther);
				caster->CastSpellImmediate(nocturnal_invis, false, player, 1.0f, false, 0.0f, nullptr);

				if (Util::random() <= Settings::GetSingleton()->oppNocturnalRecastChance) {
					OppNocturnalHands(true);
				}
			}
		}
	}
}