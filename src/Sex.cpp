#include "Sex.h"

#include "Devices.h"
#include "OppDevices.h"
#include "tats.h"
#include "Consequences.h"
#include "Locations.h"
#include "SexLabPPFunctions.h"
#include "Settings.h"
#include "Scripting.h"
#include "Serializer.h"

using namespace SKSE;

namespace DCURSES {
	bool getIsPlayerCommandedActor(RE::Actor* actor) {
		if (!actor) {
			return false;
		}

		return actor->IsCommandedActor() && (actor->GetCommandingActor().get() == RE::PlayerCharacter::GetSingleton());
	}

	bool SexActorFilter(RE::Actor* actor, bool allowInCombat) {
		auto settings = Settings::GetSingleton();

		if (!settings->sexEnabled || !actor) {
			return false;
		}
		//auto player = RE::PlayerCharacter::GetSingleton();
		//RE::TESFaction* arousalFaction = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESFaction>(std::stoi("03FC36", 0, 16), "SexLabAroused.esm");
		//RE::TESFaction* SexlabGenderFaction = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESFaction>(std::stoi("043A43", 0, 16), "SexLab.esm");
		//RE::TESFaction* PlayerMarriedFaction = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESFaction>(std::stoi("0C6472", 0, 16), "Skyrim.esm");
		RE::TESFaction* SexlabAnimatingFaction = StaticDataHolder::GetSingleton()->LookupForm<RE::TESFaction>(0x00E50F, "SexLab.esm");
		RE::TESFaction* ZadAnimatingFaction = StaticDataHolder::GetSingleton()->LookupForm<RE::TESFaction>(0x029567, "Devious Devices - Integration.esm");

		RE::TESRace* ManakinRace = StaticDataHolder::GetSingleton()->LookupForm<RE::TESRace>(0x10760a, "Skyrim.esm");
		RE::TESRace* ChickenRace = StaticDataHolder::GetSingleton()->LookupForm<RE::TESRace>(0xa919d, "Skyrim.esm");
		RE::TESRace* GoatRace = StaticDataHolder::GetSingleton()->LookupForm<RE::TESRace>(0x131fa, "Skyrim.esm");
		RE::TESRace* CowRace = StaticDataHolder::GetSingleton()->LookupForm<RE::TESRace>(0x4e785, "Skyrim.esm");
		RE::TESRace* HareRace = StaticDataHolder::GetSingleton()->LookupForm<RE::TESRace>(0x6dc99, "Skyrim.esm");
		//RE::TESRace* OldPeopleRace = StaticDataHolder::GetSingleton()->LookupForm<RE::TESRace>(0x67cd8, "Skyrim.esm");

		auto actorRace = actor->GetRace();

		if (((actor->IsInCombat() || actor->IsHostileToActor(RE::PlayerCharacter::GetSingleton())) && !allowInCombat) || actor->IsInFaction(SexlabAnimatingFaction) || actor->IsInFaction(ZadAnimatingFaction) || actor->IsInWater() || actor->IsInRagdollState() || actor->IsChild() || actor->AsActorState()->GetSitSleepState() == RE::SIT_SLEEP_STATE::kIsSleeping || actorRace == ManakinRace) {
			return false;
		}

		if (!settings->sexAllowFarmAnimals && (actorRace == ChickenRace || actorRace == GoatRace || actorRace == CowRace || actorRace == HareRace)) {
			return false;
		}

		bool isCreature = Util::ActorIsCreature(actor);

		bool isFuta = false;

		bool isMale = false;
		bool isFemale = false;

		if (!isCreature) {
			auto sex = SexLab::GetSex(actor);
			isMale = sex == 0;
			isFemale = sex == 1;
			isFuta = sex == 2;
		}
		//log::info("Actor {} is {}", actor->GetName(), isMale ? "Male" : (isFemale ? "Female" : (isFuta ? "Futa" : isCreature ? "Creature" : "Unknown Gender")));
		//else {
		//	isMale = actorSex == 0;
		//	isFemale = actorSex == 1;
		//}

		if (settings->sexFilterDevices) {
			if (isFemale && (GetDeviceMask(actor) & 0b0011) == 0) {
				return false;
			}
			if ((isFuta || isMale) && (GetDeviceMask(actor) & 0b0010) == 0) {
				return false;
			}
		}

		if ((isMale && settings->sexAllowMale) ||
			(isFemale && settings->sexAllowFemale) ||
			(isFuta && settings->sexAllowFuta) ||
			(isCreature && settings->sexAllowCreature)) {
			return true;
		}
		return false;

	}

	int GetDeviceMask(RE::Actor* actor) {
		if (!actor) return 0;
		int canAnal = 1;
		int canVaginal = 1;
		int canOral = 1;
		int canBoobjob = 1;

		for (auto const& [k, v] : actor->GetInventory()) {
			if (v.second.get()->IsWorn()) {
				RE::TESObjectARMO* wornArmor = k->As<RE::TESObjectARMO>();
				if (!wornArmor) {
					continue;
				}

				if (wornArmor->HasKeywordString("zad_DeviousBelt") && !wornArmor->HasKeywordString("zad_PermitVaginal")) {
					canVaginal = 0;
				}
				if (wornArmor->HasKeywordString("zad_DeviousSuit") && !wornArmor->HasKeywordString("zad_PermitVaginal")) {
					canVaginal = 0;
				}
				if (wornArmor->HasKeywordString("zad_DeviousPlugVaginal") && !wornArmor->HasKeywordString("zad_PermitVaginal")) {
					canVaginal = 0;
				}

				if (wornArmor->HasKeywordString("zad_DeviousBelt") && !wornArmor->HasKeywordString("zad_PermitAnal")) {
					canAnal = 0;
				}
				if (wornArmor->HasKeywordString("zad_DeviousSuit") && !wornArmor->HasKeywordString("zad_PermitAnal")) {
					canAnal = 0;
				}
				if (wornArmor->HasKeywordString("zad_DeviousPlugAnal") && !wornArmor->HasKeywordString("zad_PermitAnal")) {
					canAnal = 0;
				}

				if (wornArmor->HasKeywordString("zad_DeviousGag") && !wornArmor->HasKeywordString("zad_DeviousGagPanel") && !wornArmor->HasKeywordString("zad_PermitOral")) {
					canOral = 0;
				}

				if (wornArmor->HasKeywordString("zad_DeviousBra") || wornArmor->HasKeywordString("zad_DeviousSuit")) {
					canBoobjob = 0;
				}
			}
		}
		auto mask = canAnal | (canVaginal << 1) | (canOral << 2) | (canBoobjob << 3);
		//log::trace("Device mask: {:b}", mask);
		return mask;
	}

	bool sexCheckPlayerRequirements() {
		auto settings = Settings::GetSingleton();
		auto scriptManager = ScriptingManager();

		auto player = RE::PlayerCharacter::GetSingleton();

		auto playerNumDevices = GetVisibleDeviceCount(player);
		auto playerIsNude = getIsNude(player);
		auto playerWornDeviceKeywords = GetWornDeviceKeywords(player);
		auto playerIsWearingCollar = playerWornDeviceKeywords.contains("zad_DeviousCollar");
		auto playerIsWearingHeavyBondage = playerWornDeviceKeywords.contains("zad_DeviousHeavyBondage");

		auto playerTattooCount = GetTattooCount(player);
		int playerArousal = scriptManager.GetArousal(player);

		if (!(settings->sexRequiredPlayerBindings >= 0) && !settings->sexRequirePlayerCollar && !settings->sexRequirePlayerHeavy && !settings->sexRequirePlayerNude && !(settings->sexRequiredPlayerArousal >= 0) && !(settings->sexRequiredPlayerTattoos >= 0)) {
			return true;
		}

		if (settings->sexRequireAllPlayer) {
			return (!(settings->sexRequiredPlayerBindings >= 0) || playerNumDevices > 0) &&
				(!settings->sexRequirePlayerCollar || playerIsWearingCollar) &&
				(!settings->sexRequirePlayerHeavy || playerIsWearingHeavyBondage) &&
				(!settings->sexRequirePlayerNude || playerIsNude) &&
				(!(settings->sexRequiredPlayerTattoos >= 0) || playerTattooCount >= settings->sexRequiredPlayerTattoos) &&
				(!(settings->sexRequiredPlayerArousal >= 0) || playerArousal >= settings->sexRequiredPlayerArousal);
		}
		else {
			return ((settings->sexRequiredPlayerBindings >= 0) && playerNumDevices > 0) ||
				(settings->sexRequirePlayerCollar && playerIsWearingCollar) ||
				(settings->sexRequirePlayerHeavy && playerIsWearingHeavyBondage) ||
				(settings->sexRequirePlayerNude && playerIsNude) ||
				((settings->sexRequiredPlayerTattoos >= 0) && playerTattooCount >= settings->sexRequiredPlayerTattoos) ||
				((settings->sexRequiredPlayerArousal >= 0) && playerArousal >= settings->sexRequiredPlayerArousal);
		}
	}

	bool sexCheckActorRequirements(RE::Actor* actor) {
		auto settings = Settings::GetSingleton();
		auto scriptManager = ScriptingManager();

		auto actorNumDevices = GetVisibleDeviceCount(actor);
		auto actorIsNude = getIsNude(actor);
		auto actorWornDeviceKeywords = GetWornDeviceKeywords(actor);
		auto actorIsWearingCollar =actorWornDeviceKeywords.contains("zad_DeviousCollar");
		//auto actorIsWearingHeavyBondage = Util::vectorContains<std::string>(actorWornDeviceKeywords, "zad_DeviousHeavyBondage");

		auto actorTattooCount = GetTattooCount(actor);
		int actorArousal = scriptManager.GetArousal(actor);

		if (!(settings->sexRequiredNPCBindings >= 0) && !settings->sexRequireNPCCollar && !settings->sexRequireNPCNude && !(settings->sexRequiredNPCArousal >= 0) && !(settings->sexRequiredNPCTattoos >= 0)) {
			return true;
		}

		if (settings->sexRequireAllNPC) {
			return (!(settings->sexRequiredNPCBindings >= 0) || actorNumDevices > 0) &&
				(!settings->sexRequireNPCCollar || actorIsWearingCollar) &&
				//(!settings->sexRequirePlayerHeavy || actorIsWearingHeavyBondage) &&
				(!settings->sexRequireNPCNude || actorIsNude) &&
				(!(settings->sexRequiredNPCTattoos >= 0) || actorTattooCount >= settings->sexRequiredNPCTattoos) &&
				(!(settings->sexRequiredNPCArousal >= 0) || actorArousal >= settings->sexRequiredNPCArousal);
		}
		else {
			return ((settings->sexRequiredNPCBindings >= 0) && actorNumDevices > 0) ||
				(settings->sexRequireNPCCollar && actorIsWearingCollar) ||
				//(settings->sexRequirePlayerHeavy && actorIsWearingHeavyBondage) ||
				(settings->sexRequireNPCNude && actorIsNude) ||
				((settings->sexRequiredNPCTattoos >= 0) && actorTattooCount >= settings->sexRequiredNPCTattoos) ||
				((settings->sexRequiredNPCArousal >= 0) && actorArousal >= settings->sexRequiredNPCArousal);
		}
	}

	bool sexCheckCreatureRequirements(RE::Actor* actor) {
		auto settings = Settings::GetSingleton();
		auto scriptManager = ScriptingManager();

		int actorArousal = scriptManager.GetArousal(actor);
		if (!(settings->sexRequiredCreatureArousal >= 0)) {
			return true;
		}

		return actorArousal >= settings->sexRequiredCreatureArousal;
	}

	std::vector<std::pair<RE::Actor*, std::string>> getAllAvailableActors() {
		auto settings = Settings::GetSingleton();
		auto player = RE::PlayerCharacter::GetSingleton();
		auto playerPosition = player->GetPosition();

		//auto playerTattooCount = GetTattooCount(player);
		auto playerLewdMark = GetLewdMark();

		auto locationType = GetLocationType(player->GetCurrentLocation());

		auto playerNumDevices = GetVisibleDeviceCount(player);
		auto playerWornDeviceKeywords = GetWornDeviceKeywords(player);
		auto playerIsNude = getIsNude(player);
		//auto playerIsWearingCollar = Util::vectorContains<std::string>(playerWornDeviceKeywords, "zad_DeviousCollar");
		//auto playerIsWearingHeavyBondage = Util::vectorContains<std::string>(playerWornDeviceKeywords, "zad_DeviousHeavyBondage");
		//auto playerIsWearingBlindfold = Util::vectorContains<std::string>(playerWornDeviceKeywords, "zad_DeviousBlindfold");
		//auto playerIsWearingBoots = Util::vectorContains<std::string>(playerWornDeviceKeywords, "zad_DeviousBoots");

		auto playerHasSummonerCollar = IsWearingOppSummonerCollar();

		RE::TESFaction* PlayerMarriedFaction = StaticDataHolder::GetSingleton()->LookupForm<RE::TESFaction>(std::stoi("0C6472", 0, 16), "Skyrim.esm");
		//RE::TESGlobal* GameHour = StaticDataHolder::GetSingleton()->LookupForm<RE::TESGlobal>(std::stoi("38", 0, 16), "Skyrim.esm");

		RE::TESFaction* zadDisable = StaticDataHolder::GetSingleton()->LookupForm<RE::TESFaction>(0x4653B, "Devious Devices - Integration.esm");

		auto scriptManager = ScriptingManager();

		bool playerRequires = sexCheckPlayerRequirements();
		int playerArousal = scriptManager.GetArousal(player);

		std::vector<std::pair<RE::Actor*, std::string>> result;
		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			RE::BSSimpleList<RE::ActorHandle>* arr = &(processLists->aliveActorList);
			if (arr) {
				for (auto& actorHandle : *arr) {
					auto actorPtr = actorHandle.get();
					auto actor = actorPtr.get();
					if (actor && actor->Is3DLoaded() && !actor->IsDead() && actor->GetPosition().GetSquaredDistance(playerPosition) <= settings->sexSearchRadius * settings->sexSearchRadius) {
						if (actor->IsInFaction(zadDisable)) {
							log::trace("Ignoring NPC {}.", actor->GetName());
							continue;
						}
						if (!SexActorFilter(actor, false)) {
							continue;
						}

						bool actorIsFollower = actor->IsPlayerTeammate();
						bool actorIsCreature = Util::ActorIsCreature(actor);
						bool isPlayerCommandedActor = getIsPlayerCommandedActor(actor);

						if (actorIsFollower) {
							if (!settings->sexRandAllowFollowers) {
								//log::trace("Skipping {}, followers not enabled", actor->GetName());
								continue;
							}
						}
						else if (isPlayerCommandedActor) {
							if (!settings->sexRandAllowSummons) {
								//log::trace("Skipping {}, summons not enabled", actor->GetName());
								continue;
							}
						}
						else if (actorIsCreature) {
							if (!settings->sexRandAllowCreatures) {
								//log::trace("Skipping {}, creatures not enabled", actor->GetName());
								continue;
							}
						}
						else if (!settings->sexRandAllowNPCs) {
							//log::trace("Skipping {}, NPCs not enabled", actor->GetName());
							continue;
						}

						bool actorIsSpouse = actor->GetFactionRank(PlayerMarriedFaction, false) >= 0;

						bool playerHasRequirements = playerRequires;
						bool actorHasRequirements = false;
						if (settings->sexFollowerBypassPlayer && actorIsFollower) {
							playerHasRequirements = true;
						}
						if (settings->sexFollowerBypassNPC && actorIsFollower) {
							actorHasRequirements = true;
						}
						if (settings->sexSpouseBypassPlayer && actorIsSpouse) {
							playerHasRequirements = true;
						}
						if (settings->sexSpouseBypassNPC && actorIsSpouse) {
							actorHasRequirements = true;
						}
						if (settings->sexSummonBypassPlayer && isPlayerCommandedActor) {
							playerHasRequirements = true;
						}
						if (settings->sexSummonBypassNPC && isPlayerCommandedActor) {
							actorHasRequirements = true;
						}
						if (settings->LMAllureSkipRequirements && playerLewdMark == MARK::TAT_ALLURE) {
							playerHasRequirements = true;
							actorHasRequirements = true;
						}

						if (!actorHasRequirements) {
							if (actorIsCreature) {
								actorHasRequirements = sexCheckCreatureRequirements(actor);
							}
							else {
								actorHasRequirements = sexCheckActorRequirements(actor);
							}
						}

						bool enabled = false;
						if (!settings->sexRequireBoth && (playerHasRequirements || actorHasRequirements)) {
							enabled = true;
						}
						else if (playerHasRequirements && actorHasRequirements) {
							enabled = true;
						}

						//log::trace("Player: {}, Actor: {}, Enabled: {}", playerHasRequirements, actorHasRequirements, enabled);

						if (enabled) {
							bool actorIsNude = getIsNude(actor);
							int actorArousal = scriptManager.GetArousal(actor);

							double chance = settings->sexBaseChance;
							std::string logMessage = fmt::format("{}% -> ", settings->sexBaseChance);
							if (actorIsCreature) {
								chance *= settings->sexCreatureModifier;
								logMessage += fmt::format("(creature {:.2f}) ", settings->sexCreatureModifier);
							}
							if (actorIsFollower && !isPlayerCommandedActor) {
								chance *= settings->sexFollowerModifier;
								logMessage += fmt::format("(follower {:.2f}) ", settings->sexFollowerModifier);
							}
							if (isPlayerCommandedActor) {
								chance *= settings->sexSummonModifier;
								logMessage += fmt::format("(summon {:.2f}) ", settings->sexSummonModifier);
							}
							if (playerNumDevices > 0) {
								double mult = pow(settings->sexRestraintsModifier, playerNumDevices);
								chance *= mult;
								logMessage += fmt::format("(devices {:.2f}) ", mult);
							}
							if (locationType == LocationType::PlayerHome || locationType == LocationType::City || locationType == LocationType::Town) {
								chance *= settings->sexSafeLocationModifier;
								logMessage += fmt::format("(safe {:.2f}) ", settings->sexSafeLocationModifier);
							}
							else if (locationType != LocationType::Wilderness && locationType != LocationType::Unknown) {
								chance *= settings->sexUnsafeLocationModifier;
								logMessage += fmt::format("(unsafe {:.2f}) ", settings->sexUnsafeLocationModifier);
							}
							if (playerIsNude) {
								chance *= settings->sexPlayerNudityModifier;
								logMessage += fmt::format("(player nude {:.2f}) ", settings->sexPlayerNudityModifier);
							}
							if (actorIsNude && !actorIsCreature) {
								chance *= settings->sexNPCNudityModifier;
								logMessage += fmt::format("(npc nude {:.2f}) ", settings->sexNPCNudityModifier);
							}
							if (playerArousal > settings->sexArousalModifierCutoff) {
								double mult = ((playerArousal - settings->sexArousalModifierCutoff) * (settings->sexPlayerArousalModifier - 1.0)) / (100.0 - settings->sexArousalModifierCutoff) + 1.0;
								chance *= mult;
								logMessage += fmt::format("(player aroused {:.2f}) ", mult);
							}
							if (actorArousal > settings->sexArousalModifierCutoff) {
								double mult = ((actorArousal - settings->sexArousalModifierCutoff) * (settings->sexNPCArousalModifier - 1.0)) / (100.0 - settings->sexArousalModifierCutoff) + 1.0;
								chance *= mult;
								logMessage += fmt::format("(npc aroused {:.2f}) ", mult);
							}
							if (playerLewdMark == MARK::TAT_ALLURE) {
								chance *= settings->sexAllureModifier;
								logMessage += fmt::format("(allure {:.2f}) ", settings->sexAllureModifier);
							}
							if (playerHasSummonerCollar && isPlayerCommandedActor) {
								chance *= settings->sexSummonerModifier;
								logMessage += fmt::format("(summoner collar {:.2f}) ", settings->sexSummonerModifier);
							}

							if (settings->sexScalingModifier) {
								auto sinceLastSex = Serialized::GetCounters()->SinceLastSex;
								double scaling = static_cast<double>(sinceLastSex) / (sinceLastSex + settings->sexScalingModifier);
								chance *= scaling;
								logMessage += fmt::format("(scaling {:.2f}) ", scaling);
							}

							double r = Util::random<double>(0, 100);
							auto msg = fmt::format("Sex with {}: {} Total: {:.2f}% ({:.2f})", actor->GetName(), logMessage, chance, r);
							log::trace("{}", msg);
							if (r < chance) {
								result.push_back(std::make_pair(actor, msg));
							}
						}
					}
				}
			}
		}

		return result;
	}

	std::vector<RE::Actor*> getPlayerCommandedActors() {
		auto playerPosition = RE::PlayerCharacter::GetSingleton()->GetPosition();
		std::vector<RE::Actor*> result;
		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			RE::BSSimpleList<RE::ActorHandle>* arr = &(processLists->aliveActorList);
			if (arr) {
				for (auto& actorHandle : *arr) {
					auto actorPtr = actorHandle.get();
					if (auto actor = actorPtr.get(); actor && actor->Is3DLoaded() && !actor->IsDead()) {
						if (getIsPlayerCommandedActor(actor)) {
							result.push_back(actor);
						}
					}
				}
			}
		}
		return result;
	}

	std::string P_GetAnimationFilterTags(RE::StaticFunctionTag*, RE::Actor* akActor) {
		int mask = GetDeviceMask(akActor);

		if (!Settings::GetSingleton()->sexFilterDevices) {
			return "";
		}

		std::string tagsToRemove;

		if (!(mask & 0b0100)) {
			tagsToRemove = "Oral," + tagsToRemove;
		}
		if (!(mask & 0b0001)) {
			tagsToRemove = "Anal," + tagsToRemove;
		}
		if (!(mask & 0b0010)) {
			tagsToRemove = "Vaginal,Fisting,Fingering,Masturbation," + tagsToRemove;
		}
		if (!(mask & 0b1000)) {
			tagsToRemove = "Boobjob," + tagsToRemove;
		}
		tagsToRemove.pop_back();
		return tagsToRemove;
	}

	std::string P_GetAnimationFilterTagsP(RE::StaticFunctionTag*, RE::Actor* akActor) {
		int mask = GetDeviceMask(akActor);

		std::string tags;

		if (!Settings::GetSingleton()->sexFilterDevices) {
			return "";
		}

		if (!(mask & 0b0100)) {
			tags = "-Oral," + tags;
		}
		//else {
		//	tags = "~Oral," + tags;
		//}
		if (!(mask & 0b0001)) {
			tags = "-Anal," + tags;
		}
		//else {
		//	tags = "~Anal," + tags;
		//}
		if (!(mask & 0b0010)) {
			tags = "-Vaginal,-Fisting,-Fingering,-Masturbation," + tags;
		}
		//else {
		//	tags = "~Vaginal," + tags;
		//}
		if (!(mask & 0b1000)) {
			tags = "-Boobjob," + tags;
		}
		//else {
		//	tags = "~Boobjob," + tags;
		//}
		tags.pop_back();
		return tags;
	}

	void StartMasturbationImpl() {
		auto scriptManager = ScriptingManager();
		RE::TESForm* sexlab = StaticDataHolder::GetSingleton()->LookupForm(0x0D62, "SexLab.esm");

		auto intent = ScriptIntent(sexlab, RE::FormType::Quest, "SexLabFramework", "GetVersion");
		scriptManager.RunIntentWithResult<int>(intent, [sexlab](std::optional<int> version) {
			if (version.value() <= 16601) {
				auto args = RE::MakeFunctionArguments<RE::Actor*, RE::Actor*, RE::Actor*, RE::Actor*, RE::Actor*, RE::Actor*, std::string, std::string>(std::move(RE::PlayerCharacter::GetSingleton()), nullptr, nullptr, nullptr, nullptr, nullptr, "", "");
				auto intent = ScriptIntent(sexlab, RE::FormType::Quest, "SexLabFramework", "QuickStart", args);
				return RE::BSTSmartPointer(new ScriptCallbackFunctor(intent));
			}
			std::vector<RE::Actor*> vec = { RE::PlayerCharacter::GetSingleton() };
			auto tags = P_GetAnimationFilterTagsP(nullptr, RE::PlayerCharacter::GetSingleton());
			auto args = RE::MakeFunctionArguments<std::vector<RE::Actor*>, std::string, RE::Actor*, RE::TESObjectREFR*, int, std::string>(std::move(vec), std::move(tags), nullptr, nullptr, 1, "");
			auto intent = ScriptIntent(sexlab, RE::FormType::Quest, "SexLabFramework", "StartScene", args);
			return RE::BSTSmartPointer(new ScriptCallbackFunctor(intent));
			});
	}

	void SexUpdate() {
		auto player = RE::PlayerCharacter::GetSingleton();
		auto settings = Settings::GetSingleton();
		auto counters = Serialized::GetCounters();

		if (!settings->sexEnabled || !settings->sexRandomEnabled) {
			return;
		}

		if (counters->clock_lastSex < settings->sexCooldown) {
			return;
		}

		if (counters->clock_SexTimeout <= settings->sexSearchInterval) {
			return;
		}

		if (player->IsInCombat() || player->IsInWater() || player->IsInRagdollState() || RE::UI::GetSingleton()->IsMenuOpen("Dialogue Menu") || RE::UI::GetSingleton()->IsMenuOpen("Crafting Menu")) {
			return;
		}

		if ((GetDeviceMask(RE::PlayerCharacter::GetSingleton()) & 0b0111) == 0) {
			return;
		}

		//log::info("Attempting random sex event.");

		auto actors = getAllAvailableActors();

		counters->clock_SexTimeout = 0;

		if (actors.size() == 0) {
			counters->SinceLastSex++;
			return;
		}

		counters->SinceLastSex = 0;

		//log::info("Got actors for event.");

		int r = Util::random(static_cast<int>(actors.size()));
		auto const& actorData = (actors)[r];

		log::info("{}", actorData.second);

		ScriptingManager().StartSex(actorData.first, settings->sexAggressiveAnims);

		counters->clock_lastSex = -10;
	}

	class EffectVisitor2 : public RE::MagicTarget::ForEachActiveEffectVisitor {
	public:
		RE::Actor* match = nullptr;
		// Inherited via ForEachActiveEffectVisitor
		virtual RE::BSContainer::ForEachResult Accept(RE::ActiveEffect* a_effect) override
		{
			if (a_effect->GetBaseObject()->data.associatedForm) {
				RE::TESNPC* actor_base = a_effect->GetBaseObject()->data.associatedForm->As<RE::TESNPC>();
				if (actor_base && actor_base == match->GetActorBase()) {
					log::info("Increasing summon time for {}", match->GetName());
					a_effect->duration += 180;
				}
			}
			return RE::BSContainer::ForEachResult::kContinue;
		}

		EffectVisitor2(RE::Actor* actor) {
			match = actor;
		}
	};

	void P_SexStarted(RE::StaticFunctionTag*, RE::BSTArray<RE::Actor*> actors) {
		for (auto actor : actors) {
			if (actor && actor != RE::PlayerCharacter::GetSingleton()) {
				log::trace("Sex started with {}", actor->GetName());
				if (getIsPlayerCommandedActor(actor)) {
					EffectVisitor2 temp(actor);
					RE::PlayerCharacter::GetSingleton()->AsMagicTarget()->VisitEffects(temp);
				}
				break;
			}
		}
	}

	void P_SexEnded(RE::StaticFunctionTag*, RE::BSTArray<RE::Actor*> actors) {
		if (IsModDisabled()) {
			log::trace("Sex ended skipped, mod is disabled.");
			return;
		}
		for (auto actor : actors) {
			if (actor && actor != RE::PlayerCharacter::GetSingleton()) {
				log::trace("Sex ended with {}", actor->GetName());
				OppOnSexEnd(actor);
				ConsSexEnded(actor);
				TatsOnSexEnd(actor);
				break;
			}
		}
		if (!Settings::GetSingleton()->oppMadnessOnlyDevices) {
			OppMadnessPlayerOrgasm();
		}
		OppNocturnalPlayerSex(actors);
	}

	void P_DDPlayerOrgasm(RE::StaticFunctionTag*) {
		log::trace("Player device orgasm");
		OppMadnessPlayerOrgasm();
		//OppNocturnalPlayerOrgasm();
	}

	bool PapyrusFunctionsSex(RE::BSScript::IVirtualMachine* ivm) {
		ivm->RegisterFunction("GetAnimationFilterTags", "DCursesLib", P_GetAnimationFilterTags);
		ivm->RegisterFunction("GetAnimationFilterTagsP", "DCursesLib", P_GetAnimationFilterTagsP);
		ivm->RegisterFunction("SexStarted", "DCursesLib", P_SexStarted);
		ivm->RegisterFunction("SexEnded", "DCursesLib", P_SexEnded);
		ivm->RegisterFunction("DDPlayerOrgasm", "DCursesLib", P_DDPlayerOrgasm);
		return true;
	}
}