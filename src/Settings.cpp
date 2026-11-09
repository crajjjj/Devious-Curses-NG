#include "Settings.h"

#include "apis/quicklootAPI_3.h"
#include "apis/quicklootAPI_4.h"
#include "apis/SlaveTatsNG_Interface.h"
#include "apis/Json.hpp"

#include "Scripting.h"
#include "Serializer.h"
#include "Utils.h"
#include "Devices.h"
#include "Events.h"
#include "Tats.h"
#include "Translation.h"

#include "Quest/QuestEvents.h"

#include <stdlib.h>

using namespace SKSE;

constexpr auto SETTINGS_FILE = "Data/SKSE/Plugins/DeviousCurses.json";

namespace DCURSES {
	void SetMCMInt(std::string name, int value) {
		RE::BSScript::Variable var;
		var.SetSInt(value);
		SetMCMSetting(name, var, "Devious Curses.esp", "DCurses_MCM");
	}

	void SetMCMFloat(std::string name, float value) {
		RE::BSScript::Variable var;
		var.SetFloat(value);
		SetMCMSetting(name, var, "Devious Curses.esp", "DCurses_MCM");
	}

	void SetMCMBool(std::string name, bool value) {
		RE::BSScript::Variable var;
		var.SetBool(value);
		SetMCMSetting(name, var, "Devious Curses.esp", "DCurses_MCM");
	}

	void SetMCMString(std::string name, std::string value) {
		RE::BSScript::Variable var;
		var.SetString(value);
		SetMCMSetting(name, var, "Devious Curses.esp", "DCurses_MCM");
	}

	RE::BSScript::Variable* MCMSetting(std::string name) {
		if (auto variable = GetMCMSetting(name, "Devious Curses.esp", "DCurses_MCM")) {
			return variable;
		}
		// An MCM script older than this DLL lacks the newest properties (GetMCMSetting logs which one); read
		// them as 0 / false instead of crashing in the generated code that dereferences the result.
		static RE::BSScript::Variable missing;
		missing = RE::BSScript::Variable();
		return &missing;
	}

	void PlayerMessage(Translator trans, bool useSurpriseMode) {
		auto scriptManager = ScriptingManager();
		if (useSurpriseMode) {}
		else if (Settings::GetSingleton()->noMessageBoxes) {
			scriptManager.DBGNotification(trans);
		}
		else {
			scriptManager.DBGMessageBox(trans);
		}
		log::info("[Message] {}", trans.GetTranslation());
	}
	void ResetMCMSettings() {
		auto settings = Settings::GetSingleton();
		//CODEGEN_START_RESET
		settings->minGoldRequired = 20;
		SetMCMInt("minGoldRequired",settings->minGoldRequired);
		settings->eventScalingMod = 15;
		SetMCMInt("eventScalingMod",settings->eventScalingMod);
		settings->minRestraints = 1;
		SetMCMInt("minRestraints",settings->minRestraints);
		settings->maxRestraints = 3;
		SetMCMInt("maxRestraints",settings->maxRestraints);
		settings->bossAditionalRestraints = 2;
		SetMCMInt("bossAditionalRestraints",settings->bossAditionalRestraints);
		settings->restraintCap = 7;
		SetMCMInt("restraintCap",settings->restraintCap);
		settings->minArousal = 10;
		SetMCMInt("minArousal",settings->minArousal);
		settings->beltWeight = 40;
		SetMCMInt("beltWeight",settings->beltWeight);
		settings->chastityPiercingWeight = 20;
		SetMCMInt("chastityPiercingWeight",settings->chastityPiercingWeight);
		settings->braWeight = 30;
		SetMCMInt("braWeight",settings->braWeight);
		settings->plugsWeight = 50;
		SetMCMInt("plugsWeight",settings->plugsWeight);
		settings->lockingPlugsWeight = 30;
		SetMCMInt("lockingPlugsWeight",settings->lockingPlugsWeight);
		settings->inflatablePlugsWeight = 15;
		SetMCMInt("inflatablePlugsWeight",settings->inflatablePlugsWeight);
		settings->nipplePiercingsWeight = 50;
		SetMCMInt("nipplePiercingsWeight",settings->nipplePiercingsWeight);
		settings->vaginalPiercingsWeight = 50;
		SetMCMInt("vaginalPiercingsWeight",settings->vaginalPiercingsWeight);
		settings->corsetWeight = 40;
		SetMCMInt("corsetWeight",settings->corsetWeight);
		settings->beltedCorsetsWeight = 30;
		SetMCMInt("beltedCorsetsWeight",settings->beltedCorsetsWeight);
		settings->slaveHarnessWeight = 50;
		SetMCMInt("slaveHarnessWeight",settings->slaveHarnessWeight);
		settings->chastityHarnessWeight = 40;
		SetMCMInt("chastityHarnessWeight",settings->chastityHarnessWeight);
		settings->armbinderWeight = 30;
		SetMCMInt("armbinderWeight",settings->armbinderWeight);
		settings->elbowbinderWeight = 20;
		SetMCMInt("elbowbinderWeight",settings->elbowbinderWeight);
		settings->yokeWeight = 10;
		SetMCMInt("yokeWeight",settings->yokeWeight);
		settings->shacklesWeight = 20;
		SetMCMInt("shacklesWeight",settings->shacklesWeight);
		settings->straitjacketWeight = 30;
		SetMCMInt("straitjacketWeight",settings->straitjacketWeight);
		settings->straitjacketLegbinderWeight = 15;
		SetMCMInt("straitjacketLegbinderWeight",settings->straitjacketLegbinderWeight);
		settings->petSuitWeight = 10;
		SetMCMInt("petSuitWeight",settings->petSuitWeight);
		settings->collarWeight = 60;
		SetMCMInt("collarWeight",settings->collarWeight);
		settings->armCuffsWeight = 60;
		SetMCMInt("armCuffsWeight",settings->armCuffsWeight);
		settings->legCuffsWeight = 60;
		SetMCMInt("legCuffsWeight",settings->legCuffsWeight);
		settings->gagWeight = 35;
		SetMCMInt("gagWeight",settings->gagWeight);
		settings->ringGagWeight = 45;
		SetMCMInt("ringGagWeight",settings->ringGagWeight);
		settings->largeGagWeight = 30;
		SetMCMInt("largeGagWeight",settings->largeGagWeight);
		settings->largeRingGagWeight = 30;
		SetMCMInt("largeRingGagWeight",settings->largeRingGagWeight);
		settings->blindfoldWeight = 20;
		SetMCMInt("blindfoldWeight",settings->blindfoldWeight);
		settings->hoodBothWeight = 20;
		SetMCMInt("hoodBothWeight",settings->hoodBothWeight);
		settings->hoodGagWeight = 20;
		SetMCMInt("hoodGagWeight",settings->hoodGagWeight);
		settings->hoodBlindWeight = 20;
		SetMCMInt("hoodBlindWeight",settings->hoodBlindWeight);
		settings->hoodNoneWeight = 20;
		SetMCMInt("hoodNoneWeight",settings->hoodNoneWeight);
		settings->catsuitWeight = 30;
		SetMCMInt("catsuitWeight",settings->catsuitWeight);
		settings->hobbleSkirtWeight = 20;
		SetMCMInt("hobbleSkirtWeight",settings->hobbleSkirtWeight);
		settings->hobbleSkirtDifficulty = 50;
		SetMCMInt("hobbleSkirtDifficulty",settings->hobbleSkirtDifficulty);
		settings->bootsWeight = 30;
		SetMCMInt("bootsWeight",settings->bootsWeight);
		settings->glovesWeight = 30;
		SetMCMInt("glovesWeight",settings->glovesWeight);
		settings->mittensWeight = 0;
		SetMCMInt("mittensWeight",settings->mittensWeight);
		settings->minKeysLooted = 1;
		SetMCMInt("minKeysLooted",settings->minKeysLooted);
		settings->maxKeysLooted = 1;
		SetMCMInt("maxKeysLooted",settings->maxKeysLooted);
		settings->maxHeldKeys = 3;
		SetMCMInt("maxHeldKeys",settings->maxHeldKeys);
		settings->restraintsKeyWeight = 80;
		SetMCMInt("restraintsKeyWeight",settings->restraintsKeyWeight);
		settings->chastityKeyWeight = 50;
		SetMCMInt("chastityKeyWeight",settings->chastityKeyWeight);
		settings->piercingToolWeight = 20;
		SetMCMInt("piercingToolWeight",settings->piercingToolWeight);
		settings->eventStandardWeight = 100;
		SetMCMInt("eventStandardWeight",settings->eventStandardWeight);
		settings->eventStandardBossReduction = 20;
		SetMCMInt("eventStandardBossReduction",settings->eventStandardBossReduction);
		settings->eventOppressiveWeight = 15;
		SetMCMInt("eventOppressiveWeight",settings->eventOppressiveWeight);
		settings->eventContraptionWeight = 25;
		SetMCMInt("eventContraptionWeight",settings->eventContraptionWeight);
		settings->eventContDeviceOverride = 0;
		SetMCMInt("eventContDeviceOverride",settings->eventContDeviceOverride);
		settings->eventWickedWeight = 15;
		SetMCMInt("eventWickedWeight",settings->eventWickedWeight);
		settings->eventTattooWeight = 15;
		SetMCMInt("eventTattooWeight",settings->eventTattooWeight);
		settings->eventTattooMin = 1;
		SetMCMInt("eventTattooMin",settings->eventTattooMin);
		settings->eventTattooMax = 3;
		SetMCMInt("eventTattooMax",settings->eventTattooMax);
		settings->eventTattooCap = 8;
		SetMCMInt("eventTattooCap",settings->eventTattooCap);
		settings->eventLewdMarkWeight = 10;
		SetMCMInt("eventLewdMarkWeight",settings->eventLewdMarkWeight);
		settings->eventSimpleSlaveryWeight = 0;
		SetMCMInt("eventSimpleSlaveryWeight",settings->eventSimpleSlaveryWeight);
		settings->eventSSMinRestraints = 6;
		SetMCMInt("eventSSMinRestraints",settings->eventSSMinRestraints);
		settings->eventAbadonWeight = 5;
		SetMCMInt("eventAbadonWeight",settings->eventAbadonWeight);
		settings->eventAbadonWarriorWeight = 10;
		SetMCMInt("eventAbadonWarriorWeight",settings->eventAbadonWarriorWeight);
		settings->eventAbadonWarriorCount = 5;
		SetMCMInt("eventAbadonWarriorCount",settings->eventAbadonWarriorCount);
		settings->eventAbadonScoutWeight = 10;
		SetMCMInt("eventAbadonScoutWeight",settings->eventAbadonScoutWeight);
		settings->eventAbadonScoutCount = 5;
		SetMCMInt("eventAbadonScoutCount",settings->eventAbadonScoutCount);
		settings->eventAbadonWitchWeight = 10;
		SetMCMInt("eventAbadonWitchWeight",settings->eventAbadonWitchWeight);
		settings->eventAbadonWitchCount = 5;
		SetMCMInt("eventAbadonWitchCount",settings->eventAbadonWitchCount);
		settings->followerDeviceModifier = 0;
		SetMCMInt("followerDeviceModifier",settings->followerDeviceModifier);
		settings->LMAllureWeight = 10;
		SetMCMInt("LMAllureWeight",settings->LMAllureWeight);
		settings->LMAllureMod = 5;
		SetMCMInt("LMAllureMod",settings->LMAllureMod);
		settings->LMAllureSex = 25;
		SetMCMInt("LMAllureSex",settings->LMAllureSex);
		settings->LMHeatWeight = 10;
		SetMCMInt("LMHeatWeight",settings->LMHeatWeight);
		settings->LMHeatMod = 30;
		SetMCMInt("LMHeatMod",settings->LMHeatMod);
		settings->LMHeatContainerCount = 50;
		SetMCMInt("LMHeatContainerCount",settings->LMHeatContainerCount);
		settings->LMBrandingWeight = 10;
		SetMCMInt("LMBrandingWeight",settings->LMBrandingWeight);
		settings->LMBrandingTotal = 12;
		SetMCMInt("LMBrandingTotal",settings->LMBrandingTotal);
		settings->LMHealslutWeight = 5;
		SetMCMInt("LMHealslutWeight",settings->LMHealslutWeight);
		settings->LMHealslutHealing = 4000;
		SetMCMInt("LMHealslutHealing",settings->LMHealslutHealing);
		settings->LMHealslutArousal = 90;
		SetMCMInt("LMHealslutArousal",settings->LMHealslutArousal);
		settings->LMHealslutDuration = 300;
		SetMCMInt("LMHealslutDuration",settings->LMHealslutDuration);
		settings->LMBondageWeight = 5;
		SetMCMInt("LMBondageWeight",settings->LMBondageWeight);
		settings->LMBondageDeviceCapAdd = 3;
		SetMCMInt("LMBondageDeviceCapAdd",settings->LMBondageDeviceCapAdd);
		settings->LMBondageDeviceCount = 25;
		SetMCMInt("LMBondageDeviceCount",settings->LMBondageDeviceCount);
		settings->LMNudityWeight = 15;
		SetMCMInt("LMNudityWeight",settings->LMNudityWeight);
		settings->LMNudityTalkTimes = 35;
		SetMCMInt("LMNudityTalkTimes",settings->LMNudityTalkTimes);
		settings->LMPainslutWeight = 10;
		SetMCMInt("LMPainslutWeight",settings->LMPainslutWeight);
		settings->LMPainslutMod = 5;
		SetMCMInt("LMPainslutMod",settings->LMPainslutMod);
		settings->LMPainslutCount = 25;
		SetMCMInt("LMPainslutCount",settings->LMPainslutCount);
		settings->LMPainslutArousal = 90;
		SetMCMInt("LMPainslutArousal",settings->LMPainslutArousal);
		settings->oppSummonerCollarWeight = 20;
		SetMCMInt("oppSummonerCollarWeight",settings->oppSummonerCollarWeight);
		settings->oppSummonerSexCount = 15;
		SetMCMInt("oppSummonerSexCount",settings->oppSummonerSexCount);
		settings->oppDwarvenCuirassWeight = 15;
		SetMCMInt("oppDwarvenCuirassWeight",settings->oppDwarvenCuirassWeight);
		settings->oppDwarvenValueNeeded = 200;
		SetMCMInt("oppDwarvenValueNeeded",settings->oppDwarvenValueNeeded);
		settings->oppDwarvenArousal = 25;
		SetMCMInt("oppDwarvenArousal",settings->oppDwarvenArousal);
		settings->oppNocturnalWeight = 10;
		SetMCMInt("oppNocturnalWeight",settings->oppNocturnalWeight);
		settings->oppNocturnalSexCount = 3;
		SetMCMInt("oppNocturnalSexCount",settings->oppNocturnalSexCount);
		settings->oppNocturnalPeople = 7;
		SetMCMInt("oppNocturnalPeople",settings->oppNocturnalPeople);
		settings->oppNocturnalArousal = 35;
		SetMCMInt("oppNocturnalArousal",settings->oppNocturnalArousal);
		settings->oppLivingLatexWeight = 20;
		SetMCMInt("oppLivingLatexWeight",settings->oppLivingLatexWeight);
		settings->oppLivingLatexStartTime = 5;
		SetMCMInt("oppLivingLatexStartTime",settings->oppLivingLatexStartTime);
		settings->oppMadnessPlugWeight = 25;
		SetMCMInt("oppMadnessPlugWeight",settings->oppMadnessPlugWeight);
		settings->oppMadnessPlugIterations = 5;
		SetMCMInt("oppMadnessPlugIterations",settings->oppMadnessPlugIterations);
		settings->oppMadnessplugOrgasms = 6;
		SetMCMInt("oppMadnessplugOrgasms",settings->oppMadnessplugOrgasms);
		settings->consScanRadius = 650;
		SetMCMInt("consScanRadius",settings->consScanRadius);
		settings->consScanInterval = 5;
		SetMCMInt("consScanInterval",settings->consScanInterval);
		settings->consFineForSex = 250;
		SetMCMInt("consFineForSex",settings->consFineForSex);
		settings->consFineForNudity = 100;
		SetMCMInt("consFineForNudity",settings->consFineForNudity);
		settings->consFineForRestraints = 100;
		SetMCMInt("consFineForRestraints",settings->consFineForRestraints);
		settings->consFineForTattoos = 100;
		SetMCMInt("consFineForTattoos",settings->consFineForTattoos);
		settings->consRestrainedCount = 5;
		SetMCMInt("consRestrainedCount",settings->consRestrainedCount);
		settings->consTattooCount = 8;
		SetMCMInt("consTattooCount",settings->consTattooCount);
		settings->consSexWeight = 15;
		SetMCMInt("consSexWeight",settings->consSexWeight);
		settings->consRandomBondageWeight = 10;
		SetMCMInt("consRandomBondageWeight",settings->consRandomBondageWeight);
		settings->consTattooWeight = 10;
		SetMCMInt("consTattooWeight",settings->consTattooWeight);
		settings->consMercyWeight = 5;
		SetMCMInt("consMercyWeight",settings->consMercyWeight);
		settings->sexCooldown = 60;
		SetMCMInt("sexCooldown",settings->sexCooldown);
		settings->sexArousalModifierCutoff = 80;
		SetMCMInt("sexArousalModifierCutoff",settings->sexArousalModifierCutoff);
		settings->sexScalingModifier = 5;
		SetMCMInt("sexScalingModifier",settings->sexScalingModifier);
		settings->sexSearchRadius = 2000;
		SetMCMInt("sexSearchRadius",settings->sexSearchRadius);
		settings->sexSearchInterval = 15;
		SetMCMInt("sexSearchInterval",settings->sexSearchInterval);
		settings->sexRequiredPlayerBindings = 2;
		SetMCMInt("sexRequiredPlayerBindings",settings->sexRequiredPlayerBindings);
		settings->sexRequiredPlayerArousal = 85;
		SetMCMInt("sexRequiredPlayerArousal",settings->sexRequiredPlayerArousal);
		settings->sexRequiredPlayerTattoos = -1;
		SetMCMInt("sexRequiredPlayerTattoos",settings->sexRequiredPlayerTattoos);
		settings->sexRequiredNPCBindings = -1;
		SetMCMInt("sexRequiredNPCBindings",settings->sexRequiredNPCBindings);
		settings->sexRequiredNPCArousal = 85;
		SetMCMInt("sexRequiredNPCArousal",settings->sexRequiredNPCArousal);
		settings->sexRequiredNPCTattoos = -1;
		SetMCMInt("sexRequiredNPCTattoos",settings->sexRequiredNPCTattoos);
		settings->sexRequiredCreatureArousal = 75;
		SetMCMInt("sexRequiredCreatureArousal",settings->sexRequiredCreatureArousal);
		settings->LMAllureColor = 0xcf11c5;
		SetMCMInt("LMAllureColor",settings->LMAllureColor);
		settings->LMHeatColor = 0xe3143a;
		SetMCMInt("LMHeatColor",settings->LMHeatColor);
		settings->LMBrandingColor = 0x220022;
		SetMCMInt("LMBrandingColor",settings->LMBrandingColor);
		settings->LMHealslutColor = 0xffbb45;
		SetMCMInt("LMHealslutColor",settings->LMHealslutColor);
		settings->LMBondageColor = 0x7908cf;
		SetMCMInt("LMBondageColor",settings->LMBondageColor);
		settings->LMNudityColor = 0xd676cb;
		SetMCMInt("LMNudityColor",settings->LMNudityColor);
		settings->LMPainslutColor = 0x911300;
		SetMCMInt("LMPainslutColor",settings->LMPainslutColor);
		settings->setDebugKey = -1;
		SetMCMInt("setDebugKey",settings->setDebugKey);
		settings->oppSummonerNPC = 0;
		SetMCMInt("oppSummonerNPC",settings->oppSummonerNPC);
		settings->baseChance = 6.5f;
		SetMCMFloat("baseChance",settings->baseChance);
		settings->containerModifier = 1.0f;
		SetMCMFloat("containerModifier",settings->containerModifier);
		settings->bossContainerModifier = 2.0f;
		SetMCMFloat("bossContainerModifier",settings->bossContainerModifier);
		settings->deadBodyModifier = 1.3f;
		SetMCMFloat("deadBodyModifier",settings->deadBodyModifier);
		settings->pickpocketModifier = 1.3f;
		SetMCMFloat("pickpocketModifier",settings->pickpocketModifier);
		settings->doorModifier = 1.0f;
		SetMCMFloat("doorModifier",settings->doorModifier);
		settings->lockedModifier = 2.0f;
		SetMCMFloat("lockedModifier",settings->lockedModifier);
		settings->lockDifficultyModifier = 1.3f;
		SetMCMFloat("lockDifficultyModifier",settings->lockDifficultyModifier);
		settings->arousalModifier = 1.4f;
		SetMCMFloat("arousalModifier",settings->arousalModifier);
		settings->arousalHalfLife = 12.0f;
		SetMCMFloat("arousalHalfLife",settings->arousalHalfLife);
		settings->keyLossChance = 80.0f;
		SetMCMFloat("keyLossChance",settings->keyLossChance);
		settings->keyChance = 7.0f;
		SetMCMFloat("keyChance",settings->keyChance);
		settings->keyBonus = 1.0f;
		SetMCMFloat("keyBonus",settings->keyBonus);
		settings->keyPickpocketBonus = 2.0f;
		SetMCMFloat("keyPickpocketBonus",settings->keyPickpocketBonus);
		settings->magicKeyChance = 20.0f;
		SetMCMFloat("magicKeyChance",settings->magicKeyChance);
		settings->keyLuckyBonus = 1.75f;
		SetMCMFloat("keyLuckyBonus",settings->keyLuckyBonus);
		settings->eventContraptionTime = 4.0f;
		SetMCMFloat("eventContraptionTime",settings->eventContraptionTime);
		settings->followerDeviceMult = 0.75f;
		SetMCMFloat("followerDeviceMult",settings->followerDeviceMult);
		settings->LMBrandingChance = 1.5f;
		SetMCMFloat("LMBrandingChance",settings->LMBrandingChance);
		settings->LMBondageModifier = 5.0f;
		SetMCMFloat("LMBondageModifier",settings->LMBondageModifier);
		settings->LMPainslutSexChance = 15.0f;
		SetMCMFloat("LMPainslutSexChance",settings->LMPainslutSexChance);
		settings->oppSummonChance = 1.5f;
		SetMCMFloat("oppSummonChance",settings->oppSummonChance);
		settings->oppSummonSexChance = 25.0f;
		SetMCMFloat("oppSummonSexChance",settings->oppSummonSexChance);
		settings->oppNocturnalHandChance = 3.5f;
		SetMCMFloat("oppNocturnalHandChance",settings->oppNocturnalHandChance);
		settings->oppNocturnalRecastChance = 33.3f;
		SetMCMFloat("oppNocturnalRecastChance",settings->oppNocturnalRecastChance);
		settings->oppLivingLatexGem = 6.0f;
		SetMCMFloat("oppLivingLatexGem",settings->oppLivingLatexGem);
		settings->oppLivingLatexMore = 0.0f;
		SetMCMFloat("oppLivingLatexMore",settings->oppLivingLatexMore);
		settings->oppMadnessMultiplier = 1.0f;
		SetMCMFloat("oppMadnessMultiplier",settings->oppMadnessMultiplier);
		settings->lockedLocationBypass = 0.6f;
		SetMCMFloat("lockedLocationBypass",settings->lockedLocationBypass);
		settings->theftLocationBypass = 1.1f;
		SetMCMFloat("theftLocationBypass",settings->theftLocationBypass);
		settings->playerHomeModifier = 0.0f;
		SetMCMFloat("playerHomeModifier",settings->playerHomeModifier);
		settings->cityModifier = 0.0f;
		SetMCMFloat("cityModifier",settings->cityModifier);
		settings->townModifier = 0.0f;
		SetMCMFloat("townModifier",settings->townModifier);
		settings->banditModifier = 1.1f;
		SetMCMFloat("banditModifier",settings->banditModifier);
		settings->draugrModifier = 1.2f;
		SetMCMFloat("draugrModifier",settings->draugrModifier);
		settings->soulCairnModifier = 1.5f;
		SetMCMFloat("soulCairnModifier",settings->soulCairnModifier);
		settings->dwarvenModifier = 1.2f;
		SetMCMFloat("dwarvenModifier",settings->dwarvenModifier);
		settings->falmerModifier = 1.3f;
		SetMCMFloat("falmerModifier",settings->falmerModifier);
		settings->forswornModifier = 1.1f;
		SetMCMFloat("forswornModifier",settings->forswornModifier);
		settings->vampireModifier = 1.5f;
		SetMCMFloat("vampireModifier",settings->vampireModifier);
		settings->warlockModifier = 1.5f;
		SetMCMFloat("warlockModifier",settings->warlockModifier);
		settings->dragonLairModifier = 2.0f;
		SetMCMFloat("dragonLairModifier",settings->dragonLairModifier);
		settings->apocryphaModifier = 2.0f;
		SetMCMFloat("apocryphaModifier",settings->apocryphaModifier);
		settings->wildernessModifier = 0.9f;
		SetMCMFloat("wildernessModifier",settings->wildernessModifier);
		settings->rDeviceBaseChance = 1.5f;
		SetMCMFloat("rDeviceBaseChance",settings->rDeviceBaseChance);
		settings->rDeviceLuckyBonus = 2.5f;
		SetMCMFloat("rDeviceLuckyBonus",settings->rDeviceLuckyBonus);
		settings->wardingCharmChance = 0.5f;
		SetMCMFloat("wardingCharmChance",settings->wardingCharmChance);
		settings->tatSolventChance = 0.5f;
		SetMCMFloat("tatSolventChance",settings->tatSolventChance);
		settings->arousalPotionChance = 5.0f;
		SetMCMFloat("arousalPotionChance",settings->arousalPotionChance);
		settings->arousalPotionLibido = 10.0f;
		SetMCMFloat("arousalPotionLibido",settings->arousalPotionLibido);
		settings->arousalPotionDuration = 24.0f;
		SetMCMFloat("arousalPotionDuration",settings->arousalPotionDuration);
		settings->consTalkNude = 35.0f;
		SetMCMFloat("consTalkNude",settings->consTalkNude);
		settings->consTalkRestrained = 10.0f;
		SetMCMFloat("consTalkRestrained",settings->consTalkRestrained);
		settings->consTalkHeavy = 50.0f;
		SetMCMFloat("consTalkHeavy",settings->consTalkHeavy);
		settings->consTalkTattoo = 25.0f;
		SetMCMFloat("consTalkTattoo",settings->consTalkTattoo);
		settings->consScanSex = 25.0f;
		SetMCMFloat("consScanSex",settings->consScanSex);
		settings->consScanNude = 10.0f;
		SetMCMFloat("consScanNude",settings->consScanNude);
		settings->consScanRestrained = 0.0f;
		SetMCMFloat("consScanRestrained",settings->consScanRestrained);
		settings->consScanHeavy = 25.0f;
		SetMCMFloat("consScanHeavy",settings->consScanHeavy);
		settings->consScanTattoo = 5.0f;
		SetMCMFloat("consScanTattoo",settings->consScanTattoo);
		settings->consScanTrespass = 100.0f;
		SetMCMFloat("consScanTrespass",settings->consScanTrespass);
		settings->consGuardScanSex = 50.0f;
		SetMCMFloat("consGuardScanSex",settings->consGuardScanSex);
		settings->consGuardScanNude = 0.0f;
		SetMCMFloat("consGuardScanNude",settings->consGuardScanNude);
		settings->consGuardScanRestrained = 0.0f;
		SetMCMFloat("consGuardScanRestrained",settings->consGuardScanRestrained);
		settings->consGuardScanTattoo = 0.0f;
		SetMCMFloat("consGuardScanTattoo",settings->consGuardScanTattoo);
		settings->consGuardTalkMult = 5.0f;
		SetMCMFloat("consGuardTalkMult",settings->consGuardTalkMult);
		settings->consGaggedMultiplier = 1.2f;
		SetMCMFloat("consGaggedMultiplier",settings->consGaggedMultiplier);
		settings->sexBaseChance = 1.0f;
		SetMCMFloat("sexBaseChance",settings->sexBaseChance);
		settings->sexCreatureModifier = 0.9f;
		SetMCMFloat("sexCreatureModifier",settings->sexCreatureModifier);
		settings->sexFollowerModifier = 1.0f;
		SetMCMFloat("sexFollowerModifier",settings->sexFollowerModifier);
		settings->sexSummonModifier = 2.0f;
		SetMCMFloat("sexSummonModifier",settings->sexSummonModifier);
		settings->sexRestraintsModifier = 1.1f;
		SetMCMFloat("sexRestraintsModifier",settings->sexRestraintsModifier);
		settings->sexSafeLocationModifier = 1.3f;
		SetMCMFloat("sexSafeLocationModifier",settings->sexSafeLocationModifier);
		settings->sexUnsafeLocationModifier = 0.1f;
		SetMCMFloat("sexUnsafeLocationModifier",settings->sexUnsafeLocationModifier);
		settings->sexPlayerNudityModifier = 2.0f;
		SetMCMFloat("sexPlayerNudityModifier",settings->sexPlayerNudityModifier);
		settings->sexNPCNudityModifier = 2.0f;
		SetMCMFloat("sexNPCNudityModifier",settings->sexNPCNudityModifier);
		settings->sexPlayerArousalModifier = 2.5f;
		SetMCMFloat("sexPlayerArousalModifier",settings->sexPlayerArousalModifier);
		settings->sexNPCArousalModifier = 2.5f;
		SetMCMFloat("sexNPCArousalModifier",settings->sexNPCArousalModifier);
		settings->sexAllureModifier = 5.0f;
		SetMCMFloat("sexAllureModifier",settings->sexAllureModifier);
		settings->sexSummonerModifier = 7.5f;
		SetMCMFloat("sexSummonerModifier",settings->sexSummonerModifier);
		settings->onlyLockedDoors = true;
		SetMCMBool("onlyLockedDoors",settings->onlyLockedDoors);
		settings->eventScaling = true;
		SetMCMBool("eventScaling",settings->eventScaling);
		settings->bossOnlyHeavy = true;
		SetMCMBool("bossOnlyHeavy",settings->bossOnlyHeavy);
		settings->stripPlayerOnEvent = true;
		SetMCMBool("stripPlayerOnEvent",settings->stripPlayerOnEvent);
		settings->stripOnlyKeywords = true;
		SetMCMBool("stripOnlyKeywords",settings->stripOnlyKeywords);
		settings->beltPlugs = true;
		SetMCMBool("beltPlugs",settings->beltPlugs);
		settings->useGenderedChastity = true;
		SetMCMBool("useGenderedChastity",settings->useGenderedChastity);
		settings->plugsDontCount = true;
		SetMCMBool("plugsDontCount",settings->plugsDontCount);
		settings->useGenderedPlugs = false;
		SetMCMBool("useGenderedPlugs",settings->useGenderedPlugs);
		settings->allowLegShackles = false;
		SetMCMBool("allowLegShackles",settings->allowLegShackles);
		settings->keyForgiveness = true;
		SetMCMBool("keyForgiveness",settings->keyForgiveness);
		settings->magicKeyOppressive = false;
		SetMCMBool("magicKeyOppressive",settings->magicKeyOppressive);
		settings->preferRelevantKeys = true;
		SetMCMBool("preferRelevantKeys",settings->preferRelevantKeys);
		settings->vanishingKeys = true;
		SetMCMBool("vanishingKeys",settings->vanishingKeys);
		settings->eventContDevices = true;
		SetMCMBool("eventContDevices",settings->eventContDevices);
		settings->eventContAllDevices = false;
		SetMCMBool("eventContAllDevices",settings->eventContAllDevices);
		settings->followerBondageEvent = false;
		SetMCMBool("followerBondageEvent",settings->followerBondageEvent);
		settings->followerTattooEvent = false;
		SetMCMBool("followerTattooEvent",settings->followerTattooEvent);
		settings->onlyFemaleFollowers = true;
		SetMCMBool("onlyFemaleFollowers",settings->onlyFemaleFollowers);
		settings->followerHeavyRestraints = false;
		SetMCMBool("followerHeavyRestraints",settings->followerHeavyRestraints);
		settings->followerGags = false;
		SetMCMBool("followerGags",settings->followerGags);
		settings->onlyUseUnforgivingDevices = true;
		SetMCMBool("onlyUseUnforgivingDevices",settings->onlyUseUnforgivingDevices);
		settings->udUseAbadon = false;
		SetMCMBool("udUseAbadon",settings->udUseAbadon);
		settings->udUseAbadonSets = false;
		SetMCMBool("udUseAbadonSets",settings->udUseAbadonSets);
		settings->udUseMisc = true;
		SetMCMBool("udUseMisc",settings->udUseMisc);
		settings->LMAllureSkipRequirements = true;
		SetMCMBool("LMAllureSkipRequirements",settings->LMAllureSkipRequirements);
		settings->LMBrandingPunish = true;
		SetMCMBool("LMBrandingPunish",settings->LMBrandingPunish);
		settings->LMNudityChestOnly = false;
		SetMCMBool("LMNudityChestOnly",settings->LMNudityChestOnly);
		settings->LMNuditySkipGagged = true;
		SetMCMBool("LMNuditySkipGagged",settings->LMNuditySkipGagged);
		settings->oppOneAtATime = true;
		SetMCMBool("oppOneAtATime",settings->oppOneAtATime);
		settings->oppSCollarDrainsMagicka = true;
		SetMCMBool("oppSCollarDrainsMagicka",settings->oppSCollarDrainsMagicka);
		settings->oppSCollarAutoRemove = true;
		SetMCMBool("oppSCollarAutoRemove",settings->oppSCollarAutoRemove);
		settings->oppDwarvenHeavyRestraint = false;
		SetMCMBool("oppDwarvenHeavyRestraint",settings->oppDwarvenHeavyRestraint);
		settings->oppDwarvenRequireLoc = true;
		SetMCMBool("oppDwarvenRequireLoc",settings->oppDwarvenRequireLoc);
		settings->oppLivingLatexGemAll = false;
		SetMCMBool("oppLivingLatexGemAll",settings->oppLivingLatexGemAll);
		settings->oppLivingLatexHeavy = false;
		SetMCMBool("oppLivingLatexHeavy",settings->oppLivingLatexHeavy);
		settings->oppLivingLatexRequireRem = true;
		SetMCMBool("oppLivingLatexRequireRem",settings->oppLivingLatexRequireRem);
		settings->oppLivingLatexOpen = false;
		SetMCMBool("oppLivingLatexOpen",settings->oppLivingLatexOpen);
		settings->oppMadnessOnlyDevices = true;
		SetMCMBool("oppMadnessOnlyDevices",settings->oppMadnessOnlyDevices);
		settings->oppMadnessBeltFilter = true;
		SetMCMBool("oppMadnessBeltFilter",settings->oppMadnessBeltFilter);
		settings->oppMadnessChaos = false;
		SetMCMBool("oppMadnessChaos",settings->oppMadnessChaos);
		settings->enableQuestInteractions = true;
		SetMCMBool("enableQuestInteractions",settings->enableQuestInteractions);
		settings->enableQIMalkoran = true;
		SetMCMBool("enableQIMalkoran",settings->enableQIMalkoran);
		settings->enableQISanguine = true;
		SetMCMBool("enableQISanguine",settings->enableQISanguine);
		settings->enableQIBlackStar = true;
		SetMCMBool("enableQIBlackStar",settings->enableQIBlackStar);
		settings->enableQIMindOfMadness = true;
		SetMCMBool("enableQIMindOfMadness",settings->enableQIMindOfMadness);
		settings->enableQILaidToRest = true;
		SetMCMBool("enableQILaidToRest",settings->enableQILaidToRest);
		settings->enableQICriedWolf = true;
		SetMCMBool("enableQICriedWolf",settings->enableQICriedWolf);
		settings->enableQIDelivery = true;
		SetMCMBool("enableQIDelivery",settings->enableQIDelivery);
		settings->enableQIBarenziah = true;
		SetMCMBool("enableQIBarenziah",settings->enableQIBarenziah);
		settings->enableQIProvingHonor = true;
		SetMCMBool("enableQIProvingHonor",settings->enableQIProvingHonor);
		settings->enableQISaarthal = true;
		SetMCMBool("enableQISaarthal",settings->enableQISaarthal);
		settings->enableQIBrelyna = true;
		SetMCMBool("enableQIBrelyna",settings->enableQIBrelyna);
		settings->enableQIDwemerMuseum = true;
		SetMCMBool("enableQIDwemerMuseum",settings->enableQIDwemerMuseum);
		settings->enableQITrinityRestored = true;
		SetMCMBool("enableQITrinityRestored",settings->enableQITrinityRestored);
		settings->enableQIThalmorEmbassy = true;
		SetMCMBool("enableQIThalmorEmbassy",settings->enableQIThalmorEmbassy);
		settings->enableQIRingmaker = true;
		SetMCMBool("enableQIRingmaker",settings->enableQIRingmaker);
		settings->oppDeviceAntiCheat = true;
		SetMCMBool("oppDeviceAntiCheat",settings->oppDeviceAntiCheat);
		settings->generalDeviceAntiCheat = false;
		SetMCMBool("generalDeviceAntiCheat",settings->generalDeviceAntiCheat);
		settings->noMessageBoxes = false;
		SetMCMBool("noMessageBoxes",settings->noMessageBoxes);
		settings->eventVisualEffects = true;
		SetMCMBool("eventVisualEffects",settings->eventVisualEffects);
		settings->surpriseMode = false;
		SetMCMBool("surpriseMode",settings->surpriseMode);
		settings->dragonHoard = true;
		SetMCMBool("dragonHoard",settings->dragonHoard);
		settings->bossExtraGold = true;
		SetMCMBool("bossExtraGold",settings->bossExtraGold);
		settings->useThemes = false;
		SetMCMBool("useThemes",settings->useThemes);
		settings->disableForce3rdPerson = true;
		SetMCMBool("disableForce3rdPerson",settings->disableForce3rdPerson);
		settings->enableSlowStrip = false;
		SetMCMBool("enableSlowStrip",settings->enableSlowStrip);
		settings->restrictFastTravel = true;
		SetMCMBool("restrictFastTravel",settings->restrictFastTravel);
		settings->restrictFastTravelPart = false;
		SetMCMBool("restrictFastTravelPart",settings->restrictFastTravelPart);
		settings->restrictFastTravelFull = false;
		SetMCMBool("restrictFastTravelFull",settings->restrictFastTravelFull);
		settings->useDeviceNamesInThemes = true;
		SetMCMBool("useDeviceNamesInThemes",settings->useDeviceNamesInThemes);
		settings->saveDependentSettings = false;
		SetMCMBool("saveDependentSettings",settings->saveDependentSettings);
		settings->ANDSexTopless = true;
		SetMCMBool("ANDSexTopless",settings->ANDSexTopless);
		settings->ANDSexBottomless = true;
		SetMCMBool("ANDSexBottomless",settings->ANDSexBottomless);
		settings->ANDSexShowingChest = true;
		SetMCMBool("ANDSexShowingChest",settings->ANDSexShowingChest);
		settings->ANDSexShowingGenitals = true;
		SetMCMBool("ANDSexShowingGenitals",settings->ANDSexShowingGenitals);
		settings->ANDSexShowingAss = false;
		SetMCMBool("ANDSexShowingAss",settings->ANDSexShowingAss);
		settings->ANDSexShowingBra = false;
		SetMCMBool("ANDSexShowingBra",settings->ANDSexShowingBra);
		settings->ANDSexShowingUnderwear = false;
		SetMCMBool("ANDSexShowingUnderwear",settings->ANDSexShowingUnderwear);
		settings->ANDConsTopless = true;
		SetMCMBool("ANDConsTopless",settings->ANDConsTopless);
		settings->ANDConsBottomless = true;
		SetMCMBool("ANDConsBottomless",settings->ANDConsBottomless);
		settings->ANDConsShowingChest = true;
		SetMCMBool("ANDConsShowingChest",settings->ANDConsShowingChest);
		settings->ANDConsShowingGenitals = true;
		SetMCMBool("ANDConsShowingGenitals",settings->ANDConsShowingGenitals);
		settings->ANDConsShowingAss = false;
		SetMCMBool("ANDConsShowingAss",settings->ANDConsShowingAss);
		settings->ANDConsShowingBra = false;
		SetMCMBool("ANDConsShowingBra",settings->ANDConsShowingBra);
		settings->ANDConsShowingUnderwear = false;
		SetMCMBool("ANDConsShowingUnderwear",settings->ANDConsShowingUnderwear);
		settings->consEnable = true;
		SetMCMBool("consEnable",settings->consEnable);
		settings->consGuardNormal = false;
		SetMCMBool("consGuardNormal",settings->consGuardNormal);
		settings->consFineGuardFaction = true;
		SetMCMBool("consFineGuardFaction",settings->consFineGuardFaction);
		settings->consFallthrough = false;
		SetMCMBool("consFallthrough",settings->consFallthrough);
		settings->consAllowFollowers = false;
		SetMCMBool("consAllowFollowers",settings->consAllowFollowers);
		settings->consFollowersBlock = false;
		SetMCMBool("consFollowersBlock",settings->consFollowersBlock);
		settings->consTolerableChastity = true;
		SetMCMBool("consTolerableChastity",settings->consTolerableChastity);
		settings->consUseRelationships = true;
		SetMCMBool("consUseRelationships",settings->consUseRelationships);
		settings->consRelationBondage = false;
		SetMCMBool("consRelationBondage",settings->consRelationBondage);
		settings->consRandomHeavyBondage = false;
		SetMCMBool("consRandomHeavyBondage",settings->consRandomHeavyBondage);
		settings->consBondageIgnoreMax = false;
		SetMCMBool("consBondageIgnoreMax",settings->consBondageIgnoreMax);
		settings->consTattooCurseNumbers = false;
		SetMCMBool("consTattooCurseNumbers",settings->consTattooCurseNumbers);
		settings->sexEnabled = true;
		SetMCMBool("sexEnabled",settings->sexEnabled);
		settings->sexAggressiveAnims = false;
		SetMCMBool("sexAggressiveAnims",settings->sexAggressiveAnims);
		settings->sexFilterDevices = true;
		SetMCMBool("sexFilterDevices",settings->sexFilterDevices);
		settings->sexFilterFuta = true;
		SetMCMBool("sexFilterFuta",settings->sexFilterFuta);
		settings->sexRandomEnabled = false;
		SetMCMBool("sexRandomEnabled",settings->sexRandomEnabled);
		settings->sexRandAllowFollowers = true;
		SetMCMBool("sexRandAllowFollowers",settings->sexRandAllowFollowers);
		settings->sexRandAllowNPCs = false;
		SetMCMBool("sexRandAllowNPCs",settings->sexRandAllowNPCs);
		settings->sexRandAllowSummons = true;
		SetMCMBool("sexRandAllowSummons",settings->sexRandAllowSummons);
		settings->sexRandAllowCreatures = false;
		SetMCMBool("sexRandAllowCreatures",settings->sexRandAllowCreatures);
		settings->sexAllowMale = true;
		SetMCMBool("sexAllowMale",settings->sexAllowMale);
		settings->sexAllowFemale = true;
		SetMCMBool("sexAllowFemale",settings->sexAllowFemale);
		settings->sexAllowFuta = true;
		SetMCMBool("sexAllowFuta",settings->sexAllowFuta);
		settings->sexAllowCreature = false;
		SetMCMBool("sexAllowCreature",settings->sexAllowCreature);
		settings->sexAllowFarmAnimals = false;
		SetMCMBool("sexAllowFarmAnimals",settings->sexAllowFarmAnimals);
		settings->sexRequireAllPlayer = false;
		SetMCMBool("sexRequireAllPlayer",settings->sexRequireAllPlayer);
		settings->sexRequirePlayerCollar = false;
		SetMCMBool("sexRequirePlayerCollar",settings->sexRequirePlayerCollar);
		settings->sexRequirePlayerHeavy = false;
		SetMCMBool("sexRequirePlayerHeavy",settings->sexRequirePlayerHeavy);
		settings->sexRequirePlayerNude = true;
		SetMCMBool("sexRequirePlayerNude",settings->sexRequirePlayerNude);
		settings->sexRequireAllNPC = false;
		SetMCMBool("sexRequireAllNPC",settings->sexRequireAllNPC);
		settings->sexRequireNPCCollar = false;
		SetMCMBool("sexRequireNPCCollar",settings->sexRequireNPCCollar);
		settings->sexRequireNPCNude = true;
		SetMCMBool("sexRequireNPCNude",settings->sexRequireNPCNude);
		settings->sexRequireBoth = true;
		SetMCMBool("sexRequireBoth",settings->sexRequireBoth);
		settings->sexFollowerBypassPlayer = false;
		SetMCMBool("sexFollowerBypassPlayer",settings->sexFollowerBypassPlayer);
		settings->sexFollowerBypassNPC = true;
		SetMCMBool("sexFollowerBypassNPC",settings->sexFollowerBypassNPC);
		settings->sexSpouseBypassPlayer = true;
		SetMCMBool("sexSpouseBypassPlayer",settings->sexSpouseBypassPlayer);
		settings->sexSpouseBypassNPC = true;
		SetMCMBool("sexSpouseBypassNPC",settings->sexSpouseBypassNPC);
		settings->sexSummonBypassPlayer = false;
		SetMCMBool("sexSummonBypassPlayer",settings->sexSummonBypassPlayer);
		settings->sexSummonBypassNPC = false;
		SetMCMBool("sexSummonBypassNPC",settings->sexSummonBypassNPC);
		settings->ModSuspended = false;
		SetMCMBool("ModSuspended",settings->ModSuspended);
		settings->setAllDefaultSettings = false;
		SetMCMBool("setAllDefaultSettings",settings->setAllDefaultSettings);
		settings->helpSlaveTatsNGInstalled = false;
		SetMCMBool("helpSlaveTatsNGInstalled",settings->helpSlaveTatsNGInstalled);
		settings->helpLewdMarksInstalled = false;
		SetMCMBool("helpLewdMarksInstalled",settings->helpLewdMarksInstalled);
		settings->helpRapeTattoosInstalled = false;
		SetMCMBool("helpRapeTattoosInstalled",settings->helpRapeTattoosInstalled);
		settings->helpOSLArousedInstalled = false;
		SetMCMBool("helpOSLArousedInstalled",settings->helpOSLArousedInstalled);
		settings->helpSLOArousedInstalled = false;
		SetMCMBool("helpSLOArousedInstalled",settings->helpSLOArousedInstalled);
		settings->helpUDInstalled = false;
		SetMCMBool("helpUDInstalled",settings->helpUDInstalled);
		settings->helpQuickLootInstalled = false;
		SetMCMBool("helpQuickLootInstalled",settings->helpQuickLootInstalled);
		settings->helpDWDInstalled = false;
		SetMCMBool("helpDWDInstalled",settings->helpDWDInstalled);
		settings->helpANDInstalled = false;
		SetMCMBool("helpANDInstalled",settings->helpANDInstalled);
		settings->helpCockCagesWorking = false;
		SetMCMBool("helpCockCagesWorking",settings->helpCockCagesWorking);
		settings->excludedFollowers = "";
		SetMCMString("excludedFollowers",settings->excludedFollowers);
		settings->followerOverrideTheme = "";
		SetMCMString("followerOverrideTheme",settings->followerOverrideTheme);
		settings->followerSkipKeywords = "";
		SetMCMString("followerSkipKeywords",settings->followerSkipKeywords);
		settings->LMNudityAditionalForms = "";
		SetMCMString("LMNudityAditionalForms",settings->LMNudityAditionalForms);
		settings->debugMode = false;
		settings->lastLoadedVersion = DCURSES_VERSION;
		//CODEGEN_END_RESET
	}
	void SaveMCMSettings() {
		auto settings = Settings::GetSingleton();

		if (settings->saveDependentSettings) {
			return;
		}

		std::ofstream o(SETTINGS_FILE);
		nlohmann::json j = nlohmann::json{
			//CODEGEN_START_TOJSON
			{"minGoldRequired", settings->minGoldRequired},
			{"eventScalingMod", settings->eventScalingMod},
			{"minRestraints", settings->minRestraints},
			{"maxRestraints", settings->maxRestraints},
			{"bossAditionalRestraints", settings->bossAditionalRestraints},
			{"restraintCap", settings->restraintCap},
			{"minArousal", settings->minArousal},
			{"beltWeight", settings->beltWeight},
			{"chastityPiercingWeight", settings->chastityPiercingWeight},
			{"braWeight", settings->braWeight},
			{"plugsWeight", settings->plugsWeight},
			{"lockingPlugsWeight", settings->lockingPlugsWeight},
			{"inflatablePlugsWeight", settings->inflatablePlugsWeight},
			{"nipplePiercingsWeight", settings->nipplePiercingsWeight},
			{"vaginalPiercingsWeight", settings->vaginalPiercingsWeight},
			{"corsetWeight", settings->corsetWeight},
			{"beltedCorsetsWeight", settings->beltedCorsetsWeight},
			{"slaveHarnessWeight", settings->slaveHarnessWeight},
			{"chastityHarnessWeight", settings->chastityHarnessWeight},
			{"armbinderWeight", settings->armbinderWeight},
			{"elbowbinderWeight", settings->elbowbinderWeight},
			{"yokeWeight", settings->yokeWeight},
			{"shacklesWeight", settings->shacklesWeight},
			{"straitjacketWeight", settings->straitjacketWeight},
			{"straitjacketLegbinderWeight", settings->straitjacketLegbinderWeight},
			{"petSuitWeight", settings->petSuitWeight},
			{"collarWeight", settings->collarWeight},
			{"armCuffsWeight", settings->armCuffsWeight},
			{"legCuffsWeight", settings->legCuffsWeight},
			{"gagWeight", settings->gagWeight},
			{"ringGagWeight", settings->ringGagWeight},
			{"largeGagWeight", settings->largeGagWeight},
			{"largeRingGagWeight", settings->largeRingGagWeight},
			{"blindfoldWeight", settings->blindfoldWeight},
			{"hoodBothWeight", settings->hoodBothWeight},
			{"hoodGagWeight", settings->hoodGagWeight},
			{"hoodBlindWeight", settings->hoodBlindWeight},
			{"hoodNoneWeight", settings->hoodNoneWeight},
			{"catsuitWeight", settings->catsuitWeight},
			{"hobbleSkirtWeight", settings->hobbleSkirtWeight},
			{"hobbleSkirtDifficulty", settings->hobbleSkirtDifficulty},
			{"bootsWeight", settings->bootsWeight},
			{"glovesWeight", settings->glovesWeight},
			{"mittensWeight", settings->mittensWeight},
			{"minKeysLooted", settings->minKeysLooted},
			{"maxKeysLooted", settings->maxKeysLooted},
			{"maxHeldKeys", settings->maxHeldKeys},
			{"restraintsKeyWeight", settings->restraintsKeyWeight},
			{"chastityKeyWeight", settings->chastityKeyWeight},
			{"piercingToolWeight", settings->piercingToolWeight},
			{"eventStandardWeight", settings->eventStandardWeight},
			{"eventStandardBossReduction", settings->eventStandardBossReduction},
			{"eventOppressiveWeight", settings->eventOppressiveWeight},
			{"eventContraptionWeight", settings->eventContraptionWeight},
			{"eventContDeviceOverride", settings->eventContDeviceOverride},
			{"eventWickedWeight", settings->eventWickedWeight},
			{"eventTattooWeight", settings->eventTattooWeight},
			{"eventTattooMin", settings->eventTattooMin},
			{"eventTattooMax", settings->eventTattooMax},
			{"eventTattooCap", settings->eventTattooCap},
			{"eventLewdMarkWeight", settings->eventLewdMarkWeight},
			{"eventSimpleSlaveryWeight", settings->eventSimpleSlaveryWeight},
			{"eventSSMinRestraints", settings->eventSSMinRestraints},
			{"eventAbadonWeight", settings->eventAbadonWeight},
			{"eventAbadonWarriorWeight", settings->eventAbadonWarriorWeight},
			{"eventAbadonWarriorCount", settings->eventAbadonWarriorCount},
			{"eventAbadonScoutWeight", settings->eventAbadonScoutWeight},
			{"eventAbadonScoutCount", settings->eventAbadonScoutCount},
			{"eventAbadonWitchWeight", settings->eventAbadonWitchWeight},
			{"eventAbadonWitchCount", settings->eventAbadonWitchCount},
			{"followerDeviceModifier", settings->followerDeviceModifier},
			{"LMAllureWeight", settings->LMAllureWeight},
			{"LMAllureMod", settings->LMAllureMod},
			{"LMAllureSex", settings->LMAllureSex},
			{"LMHeatWeight", settings->LMHeatWeight},
			{"LMHeatMod", settings->LMHeatMod},
			{"LMHeatContainerCount", settings->LMHeatContainerCount},
			{"LMBrandingWeight", settings->LMBrandingWeight},
			{"LMBrandingTotal", settings->LMBrandingTotal},
			{"LMHealslutWeight", settings->LMHealslutWeight},
			{"LMHealslutHealing", settings->LMHealslutHealing},
			{"LMHealslutArousal", settings->LMHealslutArousal},
			{"LMHealslutDuration", settings->LMHealslutDuration},
			{"LMBondageWeight", settings->LMBondageWeight},
			{"LMBondageDeviceCapAdd", settings->LMBondageDeviceCapAdd},
			{"LMBondageDeviceCount", settings->LMBondageDeviceCount},
			{"LMNudityWeight", settings->LMNudityWeight},
			{"LMNudityTalkTimes", settings->LMNudityTalkTimes},
			{"LMPainslutWeight", settings->LMPainslutWeight},
			{"LMPainslutMod", settings->LMPainslutMod},
			{"LMPainslutCount", settings->LMPainslutCount},
			{"LMPainslutArousal", settings->LMPainslutArousal},
			{"oppSummonerCollarWeight", settings->oppSummonerCollarWeight},
			{"oppSummonerSexCount", settings->oppSummonerSexCount},
			{"oppDwarvenCuirassWeight", settings->oppDwarvenCuirassWeight},
			{"oppDwarvenValueNeeded", settings->oppDwarvenValueNeeded},
			{"oppDwarvenArousal", settings->oppDwarvenArousal},
			{"oppNocturnalWeight", settings->oppNocturnalWeight},
			{"oppNocturnalSexCount", settings->oppNocturnalSexCount},
			{"oppNocturnalPeople", settings->oppNocturnalPeople},
			{"oppNocturnalArousal", settings->oppNocturnalArousal},
			{"oppLivingLatexWeight", settings->oppLivingLatexWeight},
			{"oppLivingLatexStartTime", settings->oppLivingLatexStartTime},
			{"oppMadnessPlugWeight", settings->oppMadnessPlugWeight},
			{"oppMadnessPlugIterations", settings->oppMadnessPlugIterations},
			{"oppMadnessplugOrgasms", settings->oppMadnessplugOrgasms},
			{"consScanRadius", settings->consScanRadius},
			{"consScanInterval", settings->consScanInterval},
			{"consFineForSex", settings->consFineForSex},
			{"consFineForNudity", settings->consFineForNudity},
			{"consFineForRestraints", settings->consFineForRestraints},
			{"consFineForTattoos", settings->consFineForTattoos},
			{"consRestrainedCount", settings->consRestrainedCount},
			{"consTattooCount", settings->consTattooCount},
			{"consSexWeight", settings->consSexWeight},
			{"consRandomBondageWeight", settings->consRandomBondageWeight},
			{"consTattooWeight", settings->consTattooWeight},
			{"consMercyWeight", settings->consMercyWeight},
			{"sexCooldown", settings->sexCooldown},
			{"sexArousalModifierCutoff", settings->sexArousalModifierCutoff},
			{"sexScalingModifier", settings->sexScalingModifier},
			{"sexSearchRadius", settings->sexSearchRadius},
			{"sexSearchInterval", settings->sexSearchInterval},
			{"sexRequiredPlayerBindings", settings->sexRequiredPlayerBindings},
			{"sexRequiredPlayerArousal", settings->sexRequiredPlayerArousal},
			{"sexRequiredPlayerTattoos", settings->sexRequiredPlayerTattoos},
			{"sexRequiredNPCBindings", settings->sexRequiredNPCBindings},
			{"sexRequiredNPCArousal", settings->sexRequiredNPCArousal},
			{"sexRequiredNPCTattoos", settings->sexRequiredNPCTattoos},
			{"sexRequiredCreatureArousal", settings->sexRequiredCreatureArousal},
			{"baseChance", settings->baseChance},
			{"containerModifier", settings->containerModifier},
			{"bossContainerModifier", settings->bossContainerModifier},
			{"deadBodyModifier", settings->deadBodyModifier},
			{"pickpocketModifier", settings->pickpocketModifier},
			{"doorModifier", settings->doorModifier},
			{"lockedModifier", settings->lockedModifier},
			{"lockDifficultyModifier", settings->lockDifficultyModifier},
			{"arousalModifier", settings->arousalModifier},
			{"arousalHalfLife", settings->arousalHalfLife},
			{"keyLossChance", settings->keyLossChance},
			{"keyChance", settings->keyChance},
			{"keyBonus", settings->keyBonus},
			{"keyPickpocketBonus", settings->keyPickpocketBonus},
			{"magicKeyChance", settings->magicKeyChance},
			{"keyLuckyBonus", settings->keyLuckyBonus},
			{"eventContraptionTime", settings->eventContraptionTime},
			{"followerDeviceMult", settings->followerDeviceMult},
			{"LMBrandingChance", settings->LMBrandingChance},
			{"LMBondageModifier", settings->LMBondageModifier},
			{"LMPainslutSexChance", settings->LMPainslutSexChance},
			{"oppSummonChance", settings->oppSummonChance},
			{"oppSummonSexChance", settings->oppSummonSexChance},
			{"oppNocturnalHandChance", settings->oppNocturnalHandChance},
			{"oppNocturnalRecastChance", settings->oppNocturnalRecastChance},
			{"oppLivingLatexGem", settings->oppLivingLatexGem},
			{"oppLivingLatexMore", settings->oppLivingLatexMore},
			{"oppMadnessMultiplier", settings->oppMadnessMultiplier},
			{"lockedLocationBypass", settings->lockedLocationBypass},
			{"theftLocationBypass", settings->theftLocationBypass},
			{"playerHomeModifier", settings->playerHomeModifier},
			{"cityModifier", settings->cityModifier},
			{"townModifier", settings->townModifier},
			{"banditModifier", settings->banditModifier},
			{"draugrModifier", settings->draugrModifier},
			{"soulCairnModifier", settings->soulCairnModifier},
			{"dwarvenModifier", settings->dwarvenModifier},
			{"falmerModifier", settings->falmerModifier},
			{"forswornModifier", settings->forswornModifier},
			{"vampireModifier", settings->vampireModifier},
			{"warlockModifier", settings->warlockModifier},
			{"dragonLairModifier", settings->dragonLairModifier},
			{"apocryphaModifier", settings->apocryphaModifier},
			{"wildernessModifier", settings->wildernessModifier},
			{"rDeviceBaseChance", settings->rDeviceBaseChance},
			{"rDeviceLuckyBonus", settings->rDeviceLuckyBonus},
			{"wardingCharmChance", settings->wardingCharmChance},
			{"tatSolventChance", settings->tatSolventChance},
			{"arousalPotionChance", settings->arousalPotionChance},
			{"arousalPotionLibido", settings->arousalPotionLibido},
			{"arousalPotionDuration", settings->arousalPotionDuration},
			{"consTalkNude", settings->consTalkNude},
			{"consTalkRestrained", settings->consTalkRestrained},
			{"consTalkHeavy", settings->consTalkHeavy},
			{"consTalkTattoo", settings->consTalkTattoo},
			{"consScanSex", settings->consScanSex},
			{"consScanNude", settings->consScanNude},
			{"consScanRestrained", settings->consScanRestrained},
			{"consScanHeavy", settings->consScanHeavy},
			{"consScanTattoo", settings->consScanTattoo},
			{"consScanTrespass", settings->consScanTrespass},
			{"consGuardScanSex", settings->consGuardScanSex},
			{"consGuardScanNude", settings->consGuardScanNude},
			{"consGuardScanRestrained", settings->consGuardScanRestrained},
			{"consGuardScanTattoo", settings->consGuardScanTattoo},
			{"consGuardTalkMult", settings->consGuardTalkMult},
			{"consGaggedMultiplier", settings->consGaggedMultiplier},
			{"sexBaseChance", settings->sexBaseChance},
			{"sexCreatureModifier", settings->sexCreatureModifier},
			{"sexFollowerModifier", settings->sexFollowerModifier},
			{"sexSummonModifier", settings->sexSummonModifier},
			{"sexRestraintsModifier", settings->sexRestraintsModifier},
			{"sexSafeLocationModifier", settings->sexSafeLocationModifier},
			{"sexUnsafeLocationModifier", settings->sexUnsafeLocationModifier},
			{"sexPlayerNudityModifier", settings->sexPlayerNudityModifier},
			{"sexNPCNudityModifier", settings->sexNPCNudityModifier},
			{"sexPlayerArousalModifier", settings->sexPlayerArousalModifier},
			{"sexNPCArousalModifier", settings->sexNPCArousalModifier},
			{"sexAllureModifier", settings->sexAllureModifier},
			{"sexSummonerModifier", settings->sexSummonerModifier},
			{"onlyLockedDoors", settings->onlyLockedDoors},
			{"eventScaling", settings->eventScaling},
			{"bossOnlyHeavy", settings->bossOnlyHeavy},
			{"stripPlayerOnEvent", settings->stripPlayerOnEvent},
			{"stripOnlyKeywords", settings->stripOnlyKeywords},
			{"beltPlugs", settings->beltPlugs},
			{"useGenderedChastity", settings->useGenderedChastity},
			{"plugsDontCount", settings->plugsDontCount},
			{"useGenderedPlugs", settings->useGenderedPlugs},
			{"allowLegShackles", settings->allowLegShackles},
			{"keyForgiveness", settings->keyForgiveness},
			{"magicKeyOppressive", settings->magicKeyOppressive},
			{"preferRelevantKeys", settings->preferRelevantKeys},
			{"vanishingKeys", settings->vanishingKeys},
			{"eventContDevices", settings->eventContDevices},
			{"eventContAllDevices", settings->eventContAllDevices},
			{"followerBondageEvent", settings->followerBondageEvent},
			{"followerTattooEvent", settings->followerTattooEvent},
			{"onlyFemaleFollowers", settings->onlyFemaleFollowers},
			{"followerHeavyRestraints", settings->followerHeavyRestraints},
			{"followerGags", settings->followerGags},
			{"onlyUseUnforgivingDevices", settings->onlyUseUnforgivingDevices},
			{"udUseAbadon", settings->udUseAbadon},
			{"udUseAbadonSets", settings->udUseAbadonSets},
			{"udUseMisc", settings->udUseMisc},
			{"LMAllureSkipRequirements", settings->LMAllureSkipRequirements},
			{"LMBrandingPunish", settings->LMBrandingPunish},
			{"LMNudityChestOnly", settings->LMNudityChestOnly},
			{"LMNuditySkipGagged", settings->LMNuditySkipGagged},
			{"oppOneAtATime", settings->oppOneAtATime},
			{"oppSCollarDrainsMagicka", settings->oppSCollarDrainsMagicka},
			{"oppSCollarAutoRemove", settings->oppSCollarAutoRemove},
			{"oppDwarvenHeavyRestraint", settings->oppDwarvenHeavyRestraint},
			{"oppDwarvenRequireLoc", settings->oppDwarvenRequireLoc},
			{"oppLivingLatexGemAll", settings->oppLivingLatexGemAll},
			{"oppLivingLatexHeavy", settings->oppLivingLatexHeavy},
			{"oppLivingLatexRequireRem", settings->oppLivingLatexRequireRem},
			{"oppLivingLatexOpen", settings->oppLivingLatexOpen},
			{"oppMadnessOnlyDevices", settings->oppMadnessOnlyDevices},
			{"oppMadnessBeltFilter", settings->oppMadnessBeltFilter},
			{"oppMadnessChaos", settings->oppMadnessChaos},
			{"enableQuestInteractions", settings->enableQuestInteractions},
			{"enableQIMalkoran", settings->enableQIMalkoran},
			{"enableQISanguine", settings->enableQISanguine},
			{"enableQIBlackStar", settings->enableQIBlackStar},
			{"enableQIMindOfMadness", settings->enableQIMindOfMadness},
			{"enableQILaidToRest", settings->enableQILaidToRest},
			{"enableQICriedWolf", settings->enableQICriedWolf},
			{"enableQIDelivery", settings->enableQIDelivery},
			{"enableQIBarenziah", settings->enableQIBarenziah},
			{"enableQIProvingHonor", settings->enableQIProvingHonor},
			{"enableQISaarthal", settings->enableQISaarthal},
			{"enableQIBrelyna", settings->enableQIBrelyna},
			{"enableQIDwemerMuseum", settings->enableQIDwemerMuseum},
			{"enableQITrinityRestored", settings->enableQITrinityRestored},
			{"enableQIThalmorEmbassy", settings->enableQIThalmorEmbassy},
			{"enableQIRingmaker", settings->enableQIRingmaker},
			{"oppDeviceAntiCheat", settings->oppDeviceAntiCheat},
			{"generalDeviceAntiCheat", settings->generalDeviceAntiCheat},
			{"noMessageBoxes", settings->noMessageBoxes},
			{"eventVisualEffects", settings->eventVisualEffects},
			{"surpriseMode", settings->surpriseMode},
			{"dragonHoard", settings->dragonHoard},
			{"bossExtraGold", settings->bossExtraGold},
			{"useThemes", settings->useThemes},
			{"disableForce3rdPerson", settings->disableForce3rdPerson},
			{"enableSlowStrip", settings->enableSlowStrip},
			{"restrictFastTravel", settings->restrictFastTravel},
			{"restrictFastTravelPart", settings->restrictFastTravelPart},
			{"restrictFastTravelFull", settings->restrictFastTravelFull},
			{"useDeviceNamesInThemes", settings->useDeviceNamesInThemes},
			{"saveDependentSettings", settings->saveDependentSettings},
			{"ANDSexTopless", settings->ANDSexTopless},
			{"ANDSexBottomless", settings->ANDSexBottomless},
			{"ANDSexShowingChest", settings->ANDSexShowingChest},
			{"ANDSexShowingGenitals", settings->ANDSexShowingGenitals},
			{"ANDSexShowingAss", settings->ANDSexShowingAss},
			{"ANDSexShowingBra", settings->ANDSexShowingBra},
			{"ANDSexShowingUnderwear", settings->ANDSexShowingUnderwear},
			{"ANDConsTopless", settings->ANDConsTopless},
			{"ANDConsBottomless", settings->ANDConsBottomless},
			{"ANDConsShowingChest", settings->ANDConsShowingChest},
			{"ANDConsShowingGenitals", settings->ANDConsShowingGenitals},
			{"ANDConsShowingAss", settings->ANDConsShowingAss},
			{"ANDConsShowingBra", settings->ANDConsShowingBra},
			{"ANDConsShowingUnderwear", settings->ANDConsShowingUnderwear},
			{"consEnable", settings->consEnable},
			{"consGuardNormal", settings->consGuardNormal},
			{"consFineGuardFaction", settings->consFineGuardFaction},
			{"consFallthrough", settings->consFallthrough},
			{"consAllowFollowers", settings->consAllowFollowers},
			{"consFollowersBlock", settings->consFollowersBlock},
			{"consTolerableChastity", settings->consTolerableChastity},
			{"consUseRelationships", settings->consUseRelationships},
			{"consRelationBondage", settings->consRelationBondage},
			{"consRandomHeavyBondage", settings->consRandomHeavyBondage},
			{"consBondageIgnoreMax", settings->consBondageIgnoreMax},
			{"consTattooCurseNumbers", settings->consTattooCurseNumbers},
			{"sexEnabled", settings->sexEnabled},
			{"sexAggressiveAnims", settings->sexAggressiveAnims},
			{"sexFilterDevices", settings->sexFilterDevices},
			{"sexFilterFuta", settings->sexFilterFuta},
			{"sexRandomEnabled", settings->sexRandomEnabled},
			{"sexRandAllowFollowers", settings->sexRandAllowFollowers},
			{"sexRandAllowNPCs", settings->sexRandAllowNPCs},
			{"sexRandAllowSummons", settings->sexRandAllowSummons},
			{"sexRandAllowCreatures", settings->sexRandAllowCreatures},
			{"sexAllowMale", settings->sexAllowMale},
			{"sexAllowFemale", settings->sexAllowFemale},
			{"sexAllowFuta", settings->sexAllowFuta},
			{"sexAllowCreature", settings->sexAllowCreature},
			{"sexAllowFarmAnimals", settings->sexAllowFarmAnimals},
			{"sexRequireAllPlayer", settings->sexRequireAllPlayer},
			{"sexRequirePlayerCollar", settings->sexRequirePlayerCollar},
			{"sexRequirePlayerHeavy", settings->sexRequirePlayerHeavy},
			{"sexRequirePlayerNude", settings->sexRequirePlayerNude},
			{"sexRequireAllNPC", settings->sexRequireAllNPC},
			{"sexRequireNPCCollar", settings->sexRequireNPCCollar},
			{"sexRequireNPCNude", settings->sexRequireNPCNude},
			{"sexRequireBoth", settings->sexRequireBoth},
			{"sexFollowerBypassPlayer", settings->sexFollowerBypassPlayer},
			{"sexFollowerBypassNPC", settings->sexFollowerBypassNPC},
			{"sexSpouseBypassPlayer", settings->sexSpouseBypassPlayer},
			{"sexSpouseBypassNPC", settings->sexSpouseBypassNPC},
			{"sexSummonBypassPlayer", settings->sexSummonBypassPlayer},
			{"sexSummonBypassNPC", settings->sexSummonBypassNPC},
			{"ModSuspended", settings->ModSuspended},
			{"setAllDefaultSettings", settings->setAllDefaultSettings},
			{"helpSlaveTatsNGInstalled", settings->helpSlaveTatsNGInstalled},
			{"helpLewdMarksInstalled", settings->helpLewdMarksInstalled},
			{"helpRapeTattoosInstalled", settings->helpRapeTattoosInstalled},
			{"helpOSLArousedInstalled", settings->helpOSLArousedInstalled},
			{"helpSLOArousedInstalled", settings->helpSLOArousedInstalled},
			{"helpUDInstalled", settings->helpUDInstalled},
			{"helpQuickLootInstalled", settings->helpQuickLootInstalled},
			{"helpDWDInstalled", settings->helpDWDInstalled},
			{"helpANDInstalled", settings->helpANDInstalled},
			{"helpCockCagesWorking", settings->helpCockCagesWorking},
			{"LMAllureColor", settings->LMAllureColor},
			{"LMHeatColor", settings->LMHeatColor},
			{"LMBrandingColor", settings->LMBrandingColor},
			{"LMHealslutColor", settings->LMHealslutColor},
			{"LMBondageColor", settings->LMBondageColor},
			{"LMNudityColor", settings->LMNudityColor},
			{"LMPainslutColor", settings->LMPainslutColor},
			{"setDebugKey", settings->setDebugKey},
			{"excludedFollowers", settings->excludedFollowers},
			{"followerOverrideTheme", settings->followerOverrideTheme},
			{"followerSkipKeywords", settings->followerSkipKeywords},
			{"LMNudityAditionalForms", settings->LMNudityAditionalForms},
			{"oppSummonerNPC", settings->oppSummonerNPC},
			{"debugMode", settings->debugMode},
			{"lastLoadedVersion", settings->lastLoadedVersion},
			//CODEGEN_END_TOJSON
		};
		log::trace("Wrote settings to file {}", SETTINGS_FILE);
		o << std::setw(4) << j << std::endl;
		Settings::GetSingleton()->lastSettingsEditTime = std::chrono::clock_cast<std::filesystem::file_time_type::clock>(std::chrono::system_clock::now());
	}
	bool LoadSettingsFile() {
		auto settings = Settings::GetSingleton();

		if (settings->saveDependentSettings) {
			return false;
		}


		std::ifstream i(SETTINGS_FILE);
		nlohmann::json j = nlohmann::json::parse("{}");
		if (!std::filesystem::exists(SETTINGS_FILE)) {
			log::info("Settings store file does not exist.");
			return false;
		}

		try {
			i >> j;
		}
		catch (...) {
			log::error("Settings store file has garbled data.");
			return false;
		}

		//CODEGEN_START_FROMJSON
		settings->minGoldRequired = static_cast<int>(j.value("minGoldRequired", 20));
		if (settings->minGoldRequired < 0) {settings->minGoldRequired = 0;}
		settings->eventScalingMod = static_cast<int>(j.value("eventScalingMod", 15));
		if (settings->eventScalingMod < 0) {settings->eventScalingMod = 0;}
		settings->minRestraints = static_cast<int>(j.value("minRestraints", 1));
		if (settings->minRestraints < 0) {settings->minRestraints = 0;}
		settings->maxRestraints = static_cast<int>(j.value("maxRestraints", 3));
		if (settings->maxRestraints < 0) {settings->maxRestraints = 0;}
		settings->bossAditionalRestraints = static_cast<int>(j.value("bossAditionalRestraints", 2));
		if (settings->bossAditionalRestraints < 0) {settings->bossAditionalRestraints = 0;}
		settings->restraintCap = static_cast<int>(j.value("restraintCap", 7));
		if (settings->restraintCap < 0) {settings->restraintCap = 0;}
		settings->minArousal = static_cast<int>(j.value("minArousal", 10));
		if (settings->minArousal < 0) {settings->minArousal = 0;}
		settings->beltWeight = static_cast<int>(j.value("beltWeight", 40));
		if (settings->beltWeight < 0) {settings->beltWeight = 0;}
		settings->chastityPiercingWeight = static_cast<int>(j.value("chastityPiercingWeight", 20));
		if (settings->chastityPiercingWeight < 0) {settings->chastityPiercingWeight = 0;}
		settings->braWeight = static_cast<int>(j.value("braWeight", 30));
		if (settings->braWeight < 0) {settings->braWeight = 0;}
		settings->plugsWeight = static_cast<int>(j.value("plugsWeight", 50));
		if (settings->plugsWeight < 0) {settings->plugsWeight = 0;}
		settings->lockingPlugsWeight = static_cast<int>(j.value("lockingPlugsWeight", 30));
		if (settings->lockingPlugsWeight < 0) {settings->lockingPlugsWeight = 0;}
		settings->inflatablePlugsWeight = static_cast<int>(j.value("inflatablePlugsWeight", 15));
		if (settings->inflatablePlugsWeight < 0) {settings->inflatablePlugsWeight = 0;}
		settings->nipplePiercingsWeight = static_cast<int>(j.value("nipplePiercingsWeight", 50));
		if (settings->nipplePiercingsWeight < 0) {settings->nipplePiercingsWeight = 0;}
		settings->vaginalPiercingsWeight = static_cast<int>(j.value("vaginalPiercingsWeight", 50));
		if (settings->vaginalPiercingsWeight < 0) {settings->vaginalPiercingsWeight = 0;}
		settings->corsetWeight = static_cast<int>(j.value("corsetWeight", 40));
		if (settings->corsetWeight < 0) {settings->corsetWeight = 0;}
		settings->beltedCorsetsWeight = static_cast<int>(j.value("beltedCorsetsWeight", 30));
		if (settings->beltedCorsetsWeight < 0) {settings->beltedCorsetsWeight = 0;}
		settings->slaveHarnessWeight = static_cast<int>(j.value("slaveHarnessWeight", 50));
		if (settings->slaveHarnessWeight < 0) {settings->slaveHarnessWeight = 0;}
		settings->chastityHarnessWeight = static_cast<int>(j.value("chastityHarnessWeight", 40));
		if (settings->chastityHarnessWeight < 0) {settings->chastityHarnessWeight = 0;}
		settings->armbinderWeight = static_cast<int>(j.value("armbinderWeight", 30));
		if (settings->armbinderWeight < 0) {settings->armbinderWeight = 0;}
		settings->elbowbinderWeight = static_cast<int>(j.value("elbowbinderWeight", 20));
		if (settings->elbowbinderWeight < 0) {settings->elbowbinderWeight = 0;}
		settings->yokeWeight = static_cast<int>(j.value("yokeWeight", 10));
		if (settings->yokeWeight < 0) {settings->yokeWeight = 0;}
		settings->shacklesWeight = static_cast<int>(j.value("shacklesWeight", 20));
		if (settings->shacklesWeight < 0) {settings->shacklesWeight = 0;}
		settings->straitjacketWeight = static_cast<int>(j.value("straitjacketWeight", 30));
		if (settings->straitjacketWeight < 0) {settings->straitjacketWeight = 0;}
		settings->straitjacketLegbinderWeight = static_cast<int>(j.value("straitjacketLegbinderWeight", 15));
		if (settings->straitjacketLegbinderWeight < 0) {settings->straitjacketLegbinderWeight = 0;}
		settings->petSuitWeight = static_cast<int>(j.value("petSuitWeight", 10));
		if (settings->petSuitWeight < 0) {settings->petSuitWeight = 0;}
		settings->collarWeight = static_cast<int>(j.value("collarWeight", 60));
		if (settings->collarWeight < 0) {settings->collarWeight = 0;}
		settings->armCuffsWeight = static_cast<int>(j.value("armCuffsWeight", 60));
		if (settings->armCuffsWeight < 0) {settings->armCuffsWeight = 0;}
		settings->legCuffsWeight = static_cast<int>(j.value("legCuffsWeight", 60));
		if (settings->legCuffsWeight < 0) {settings->legCuffsWeight = 0;}
		settings->gagWeight = static_cast<int>(j.value("gagWeight", 35));
		if (settings->gagWeight < 0) {settings->gagWeight = 0;}
		settings->ringGagWeight = static_cast<int>(j.value("ringGagWeight", 45));
		if (settings->ringGagWeight < 0) {settings->ringGagWeight = 0;}
		settings->largeGagWeight = static_cast<int>(j.value("largeGagWeight", 30));
		if (settings->largeGagWeight < 0) {settings->largeGagWeight = 0;}
		settings->largeRingGagWeight = static_cast<int>(j.value("largeRingGagWeight", 30));
		if (settings->largeRingGagWeight < 0) {settings->largeRingGagWeight = 0;}
		settings->blindfoldWeight = static_cast<int>(j.value("blindfoldWeight", 20));
		if (settings->blindfoldWeight < 0) {settings->blindfoldWeight = 0;}
		settings->hoodBothWeight = static_cast<int>(j.value("hoodBothWeight", 20));
		if (settings->hoodBothWeight < 0) {settings->hoodBothWeight = 0;}
		settings->hoodGagWeight = static_cast<int>(j.value("hoodGagWeight", 20));
		if (settings->hoodGagWeight < 0) {settings->hoodGagWeight = 0;}
		settings->hoodBlindWeight = static_cast<int>(j.value("hoodBlindWeight", 20));
		if (settings->hoodBlindWeight < 0) {settings->hoodBlindWeight = 0;}
		settings->hoodNoneWeight = static_cast<int>(j.value("hoodNoneWeight", 20));
		if (settings->hoodNoneWeight < 0) {settings->hoodNoneWeight = 0;}
		settings->catsuitWeight = static_cast<int>(j.value("catsuitWeight", 30));
		if (settings->catsuitWeight < 0) {settings->catsuitWeight = 0;}
		settings->hobbleSkirtWeight = static_cast<int>(j.value("hobbleSkirtWeight", 20));
		if (settings->hobbleSkirtWeight < 0) {settings->hobbleSkirtWeight = 0;}
		settings->hobbleSkirtDifficulty = static_cast<int>(j.value("hobbleSkirtDifficulty", 50));
		if (settings->hobbleSkirtDifficulty < 0) {settings->hobbleSkirtDifficulty = 0;}
		settings->bootsWeight = static_cast<int>(j.value("bootsWeight", 30));
		if (settings->bootsWeight < 0) {settings->bootsWeight = 0;}
		settings->glovesWeight = static_cast<int>(j.value("glovesWeight", 30));
		if (settings->glovesWeight < 0) {settings->glovesWeight = 0;}
		settings->mittensWeight = static_cast<int>(j.value("mittensWeight", 0));
		if (settings->mittensWeight < 0) {settings->mittensWeight = 0;}
		settings->minKeysLooted = static_cast<int>(j.value("minKeysLooted", 1));
		if (settings->minKeysLooted < 0) {settings->minKeysLooted = 0;}
		settings->maxKeysLooted = static_cast<int>(j.value("maxKeysLooted", 1));
		if (settings->maxKeysLooted < 0) {settings->maxKeysLooted = 0;}
		settings->maxHeldKeys = static_cast<int>(j.value("maxHeldKeys", 3));
		if (settings->maxHeldKeys < 0) {settings->maxHeldKeys = 0;}
		settings->restraintsKeyWeight = static_cast<int>(j.value("restraintsKeyWeight", 80));
		if (settings->restraintsKeyWeight < 0) {settings->restraintsKeyWeight = 0;}
		settings->chastityKeyWeight = static_cast<int>(j.value("chastityKeyWeight", 50));
		if (settings->chastityKeyWeight < 0) {settings->chastityKeyWeight = 0;}
		settings->piercingToolWeight = static_cast<int>(j.value("piercingToolWeight", 20));
		if (settings->piercingToolWeight < 0) {settings->piercingToolWeight = 0;}
		settings->eventStandardWeight = static_cast<int>(j.value("eventStandardWeight", 100));
		if (settings->eventStandardWeight < 0) {settings->eventStandardWeight = 0;}
		settings->eventStandardBossReduction = static_cast<int>(j.value("eventStandardBossReduction", 20));
		if (settings->eventStandardBossReduction < 0) {settings->eventStandardBossReduction = 0;}
		settings->eventOppressiveWeight = static_cast<int>(j.value("eventOppressiveWeight", 15));
		if (settings->eventOppressiveWeight < 0) {settings->eventOppressiveWeight = 0;}
		settings->eventContraptionWeight = static_cast<int>(j.value("eventContraptionWeight", 25));
		if (settings->eventContraptionWeight < 0) {settings->eventContraptionWeight = 0;}
		settings->eventContDeviceOverride = static_cast<int>(j.value("eventContDeviceOverride", 0));
		if (settings->eventContDeviceOverride < 0) {settings->eventContDeviceOverride = 0;}
		settings->eventWickedWeight = static_cast<int>(j.value("eventWickedWeight", 15));
		if (settings->eventWickedWeight < 0) {settings->eventWickedWeight = 0;}
		settings->eventTattooWeight = static_cast<int>(j.value("eventTattooWeight", 15));
		if (settings->eventTattooWeight < 0) {settings->eventTattooWeight = 0;}
		settings->eventTattooMin = static_cast<int>(j.value("eventTattooMin", 1));
		if (settings->eventTattooMin < 0) {settings->eventTattooMin = 0;}
		settings->eventTattooMax = static_cast<int>(j.value("eventTattooMax", 3));
		if (settings->eventTattooMax < 0) {settings->eventTattooMax = 0;}
		settings->eventTattooCap = static_cast<int>(j.value("eventTattooCap", 8));
		if (settings->eventTattooCap < 0) {settings->eventTattooCap = 0;}
		settings->eventLewdMarkWeight = static_cast<int>(j.value("eventLewdMarkWeight", 10));
		if (settings->eventLewdMarkWeight < 0) {settings->eventLewdMarkWeight = 0;}
		settings->eventSimpleSlaveryWeight = static_cast<int>(j.value("eventSimpleSlaveryWeight", 0));
		if (settings->eventSimpleSlaveryWeight < 0) {settings->eventSimpleSlaveryWeight = 0;}
		settings->eventSSMinRestraints = static_cast<int>(j.value("eventSSMinRestraints", 6));
		if (settings->eventSSMinRestraints < 0) {settings->eventSSMinRestraints = 0;}
		settings->eventAbadonWeight = static_cast<int>(j.value("eventAbadonWeight", 5));
		if (settings->eventAbadonWeight < 0) {settings->eventAbadonWeight = 0;}
		settings->eventAbadonWarriorWeight = static_cast<int>(j.value("eventAbadonWarriorWeight", 10));
		if (settings->eventAbadonWarriorWeight < 0) {settings->eventAbadonWarriorWeight = 0;}
		settings->eventAbadonWarriorCount = static_cast<int>(j.value("eventAbadonWarriorCount", 5));
		if (settings->eventAbadonWarriorCount < 0) {settings->eventAbadonWarriorCount = 0;}
		settings->eventAbadonScoutWeight = static_cast<int>(j.value("eventAbadonScoutWeight", 10));
		if (settings->eventAbadonScoutWeight < 0) {settings->eventAbadonScoutWeight = 0;}
		settings->eventAbadonScoutCount = static_cast<int>(j.value("eventAbadonScoutCount", 5));
		if (settings->eventAbadonScoutCount < 0) {settings->eventAbadonScoutCount = 0;}
		settings->eventAbadonWitchWeight = static_cast<int>(j.value("eventAbadonWitchWeight", 10));
		if (settings->eventAbadonWitchWeight < 0) {settings->eventAbadonWitchWeight = 0;}
		settings->eventAbadonWitchCount = static_cast<int>(j.value("eventAbadonWitchCount", 5));
		if (settings->eventAbadonWitchCount < 0) {settings->eventAbadonWitchCount = 0;}
		settings->followerDeviceModifier = static_cast<int>(j.value("followerDeviceModifier", 0));
		settings->LMAllureWeight = static_cast<int>(j.value("LMAllureWeight", 10));
		if (settings->LMAllureWeight < 0) {settings->LMAllureWeight = 0;}
		settings->LMAllureMod = static_cast<int>(j.value("LMAllureMod", 5));
		if (settings->LMAllureMod < 0) {settings->LMAllureMod = 0;}
		settings->LMAllureSex = static_cast<int>(j.value("LMAllureSex", 25));
		if (settings->LMAllureSex < 0) {settings->LMAllureSex = 0;}
		settings->LMHeatWeight = static_cast<int>(j.value("LMHeatWeight", 10));
		if (settings->LMHeatWeight < 0) {settings->LMHeatWeight = 0;}
		settings->LMHeatMod = static_cast<int>(j.value("LMHeatMod", 30));
		if (settings->LMHeatMod < 0) {settings->LMHeatMod = 0;}
		settings->LMHeatContainerCount = static_cast<int>(j.value("LMHeatContainerCount", 50));
		if (settings->LMHeatContainerCount < 0) {settings->LMHeatContainerCount = 0;}
		settings->LMBrandingWeight = static_cast<int>(j.value("LMBrandingWeight", 10));
		if (settings->LMBrandingWeight < 0) {settings->LMBrandingWeight = 0;}
		settings->LMBrandingTotal = static_cast<int>(j.value("LMBrandingTotal", 12));
		if (settings->LMBrandingTotal < 0) {settings->LMBrandingTotal = 0;}
		settings->LMHealslutWeight = static_cast<int>(j.value("LMHealslutWeight", 5));
		if (settings->LMHealslutWeight < 0) {settings->LMHealslutWeight = 0;}
		settings->LMHealslutHealing = static_cast<int>(j.value("LMHealslutHealing", 4000));
		if (settings->LMHealslutHealing < 0) {settings->LMHealslutHealing = 0;}
		settings->LMHealslutArousal = static_cast<int>(j.value("LMHealslutArousal", 90));
		if (settings->LMHealslutArousal < 0) {settings->LMHealslutArousal = 0;}
		settings->LMHealslutDuration = static_cast<int>(j.value("LMHealslutDuration", 300));
		if (settings->LMHealslutDuration < 0) {settings->LMHealslutDuration = 0;}
		settings->LMBondageWeight = static_cast<int>(j.value("LMBondageWeight", 5));
		if (settings->LMBondageWeight < 0) {settings->LMBondageWeight = 0;}
		settings->LMBondageDeviceCapAdd = static_cast<int>(j.value("LMBondageDeviceCapAdd", 3));
		if (settings->LMBondageDeviceCapAdd < 0) {settings->LMBondageDeviceCapAdd = 0;}
		settings->LMBondageDeviceCount = static_cast<int>(j.value("LMBondageDeviceCount", 25));
		if (settings->LMBondageDeviceCount < 0) {settings->LMBondageDeviceCount = 0;}
		settings->LMNudityWeight = static_cast<int>(j.value("LMNudityWeight", 15));
		if (settings->LMNudityWeight < 0) {settings->LMNudityWeight = 0;}
		settings->LMNudityTalkTimes = static_cast<int>(j.value("LMNudityTalkTimes", 35));
		if (settings->LMNudityTalkTimes < 0) {settings->LMNudityTalkTimes = 0;}
		settings->LMPainslutWeight = static_cast<int>(j.value("LMPainslutWeight", 10));
		if (settings->LMPainslutWeight < 0) {settings->LMPainslutWeight = 0;}
		settings->LMPainslutMod = static_cast<int>(j.value("LMPainslutMod", 5));
		if (settings->LMPainslutMod < 0) {settings->LMPainslutMod = 0;}
		settings->LMPainslutCount = static_cast<int>(j.value("LMPainslutCount", 25));
		if (settings->LMPainslutCount < 0) {settings->LMPainslutCount = 0;}
		settings->LMPainslutArousal = static_cast<int>(j.value("LMPainslutArousal", 90));
		if (settings->LMPainslutArousal < 0) {settings->LMPainslutArousal = 0;}
		settings->oppSummonerCollarWeight = static_cast<int>(j.value("oppSummonerCollarWeight", 20));
		if (settings->oppSummonerCollarWeight < 0) {settings->oppSummonerCollarWeight = 0;}
		settings->oppSummonerSexCount = static_cast<int>(j.value("oppSummonerSexCount", 15));
		if (settings->oppSummonerSexCount < 0) {settings->oppSummonerSexCount = 0;}
		settings->oppDwarvenCuirassWeight = static_cast<int>(j.value("oppDwarvenCuirassWeight", 15));
		if (settings->oppDwarvenCuirassWeight < 0) {settings->oppDwarvenCuirassWeight = 0;}
		settings->oppDwarvenValueNeeded = static_cast<int>(j.value("oppDwarvenValueNeeded", 200));
		if (settings->oppDwarvenValueNeeded < 0) {settings->oppDwarvenValueNeeded = 0;}
		settings->oppDwarvenArousal = static_cast<int>(j.value("oppDwarvenArousal", 25));
		if (settings->oppDwarvenArousal < 0) {settings->oppDwarvenArousal = 0;}
		settings->oppNocturnalWeight = static_cast<int>(j.value("oppNocturnalWeight", 10));
		if (settings->oppNocturnalWeight < 0) {settings->oppNocturnalWeight = 0;}
		settings->oppNocturnalSexCount = static_cast<int>(j.value("oppNocturnalSexCount", 3));
		if (settings->oppNocturnalSexCount < 0) {settings->oppNocturnalSexCount = 0;}
		settings->oppNocturnalPeople = static_cast<int>(j.value("oppNocturnalPeople", 7));
		if (settings->oppNocturnalPeople < 0) {settings->oppNocturnalPeople = 0;}
		settings->oppNocturnalArousal = static_cast<int>(j.value("oppNocturnalArousal", 35));
		if (settings->oppNocturnalArousal < 0) {settings->oppNocturnalArousal = 0;}
		settings->oppLivingLatexWeight = static_cast<int>(j.value("oppLivingLatexWeight", 20));
		if (settings->oppLivingLatexWeight < 0) {settings->oppLivingLatexWeight = 0;}
		settings->oppLivingLatexStartTime = static_cast<int>(j.value("oppLivingLatexStartTime", 5));
		if (settings->oppLivingLatexStartTime < 0) {settings->oppLivingLatexStartTime = 0;}
		settings->oppMadnessPlugWeight = static_cast<int>(j.value("oppMadnessPlugWeight", 25));
		if (settings->oppMadnessPlugWeight < 0) {settings->oppMadnessPlugWeight = 0;}
		settings->oppMadnessPlugIterations = static_cast<int>(j.value("oppMadnessPlugIterations", 5));
		if (settings->oppMadnessPlugIterations < 0) {settings->oppMadnessPlugIterations = 0;}
		settings->oppMadnessplugOrgasms = static_cast<int>(j.value("oppMadnessplugOrgasms", 6));
		if (settings->oppMadnessplugOrgasms < 0) {settings->oppMadnessplugOrgasms = 0;}
		settings->consScanRadius = static_cast<int>(j.value("consScanRadius", 650));
		if (settings->consScanRadius < 0) {settings->consScanRadius = 0;}
		settings->consScanInterval = static_cast<int>(j.value("consScanInterval", 5));
		if (settings->consScanInterval < 0) {settings->consScanInterval = 0;}
		settings->consFineForSex = static_cast<int>(j.value("consFineForSex", 250));
		if (settings->consFineForSex < 0) {settings->consFineForSex = 0;}
		settings->consFineForNudity = static_cast<int>(j.value("consFineForNudity", 100));
		if (settings->consFineForNudity < 0) {settings->consFineForNudity = 0;}
		settings->consFineForRestraints = static_cast<int>(j.value("consFineForRestraints", 100));
		if (settings->consFineForRestraints < 0) {settings->consFineForRestraints = 0;}
		settings->consFineForTattoos = static_cast<int>(j.value("consFineForTattoos", 100));
		if (settings->consFineForTattoos < 0) {settings->consFineForTattoos = 0;}
		settings->consRestrainedCount = static_cast<int>(j.value("consRestrainedCount", 5));
		if (settings->consRestrainedCount < 0) {settings->consRestrainedCount = 0;}
		settings->consTattooCount = static_cast<int>(j.value("consTattooCount", 8));
		if (settings->consTattooCount < 0) {settings->consTattooCount = 0;}
		settings->consSexWeight = static_cast<int>(j.value("consSexWeight", 15));
		if (settings->consSexWeight < 0) {settings->consSexWeight = 0;}
		settings->consRandomBondageWeight = static_cast<int>(j.value("consRandomBondageWeight", 10));
		if (settings->consRandomBondageWeight < 0) {settings->consRandomBondageWeight = 0;}
		settings->consTattooWeight = static_cast<int>(j.value("consTattooWeight", 10));
		if (settings->consTattooWeight < 0) {settings->consTattooWeight = 0;}
		settings->consMercyWeight = static_cast<int>(j.value("consMercyWeight", 5));
		if (settings->consMercyWeight < 0) {settings->consMercyWeight = 0;}
		settings->sexCooldown = static_cast<int>(j.value("sexCooldown", 60));
		if (settings->sexCooldown < 0) {settings->sexCooldown = 0;}
		settings->sexArousalModifierCutoff = static_cast<int>(j.value("sexArousalModifierCutoff", 80));
		if (settings->sexArousalModifierCutoff < 0) {settings->sexArousalModifierCutoff = 0;}
		settings->sexScalingModifier = static_cast<int>(j.value("sexScalingModifier", 5));
		if (settings->sexScalingModifier < 0) {settings->sexScalingModifier = 0;}
		settings->sexSearchRadius = static_cast<int>(j.value("sexSearchRadius", 2000));
		if (settings->sexSearchRadius < 0) {settings->sexSearchRadius = 0;}
		settings->sexSearchInterval = static_cast<int>(j.value("sexSearchInterval", 15));
		if (settings->sexSearchInterval < 0) {settings->sexSearchInterval = 0;}
		settings->sexRequiredPlayerBindings = static_cast<int>(j.value("sexRequiredPlayerBindings", 2));
		settings->sexRequiredPlayerArousal = static_cast<int>(j.value("sexRequiredPlayerArousal", 85));
		settings->sexRequiredPlayerTattoos = static_cast<int>(j.value("sexRequiredPlayerTattoos", -1));
		settings->sexRequiredNPCBindings = static_cast<int>(j.value("sexRequiredNPCBindings", -1));
		settings->sexRequiredNPCArousal = static_cast<int>(j.value("sexRequiredNPCArousal", 85));
		settings->sexRequiredNPCTattoos = static_cast<int>(j.value("sexRequiredNPCTattoos", -1));
		settings->sexRequiredCreatureArousal = static_cast<int>(j.value("sexRequiredCreatureArousal", 75));
		settings->LMAllureColor = static_cast<int>(j.value("LMAllureColor", 0xcf11c5));
		settings->LMHeatColor = static_cast<int>(j.value("LMHeatColor", 0xe3143a));
		settings->LMBrandingColor = static_cast<int>(j.value("LMBrandingColor", 0x220022));
		settings->LMHealslutColor = static_cast<int>(j.value("LMHealslutColor", 0xffbb45));
		settings->LMBondageColor = static_cast<int>(j.value("LMBondageColor", 0x7908cf));
		settings->LMNudityColor = static_cast<int>(j.value("LMNudityColor", 0xd676cb));
		settings->LMPainslutColor = static_cast<int>(j.value("LMPainslutColor", 0x911300));
		settings->setDebugKey = static_cast<int>(j.value("setDebugKey", -1));
		settings->oppSummonerNPC = static_cast<int>(j.value("oppSummonerNPC", 0));
		settings->baseChance = static_cast<float>(j.value("baseChance", 6.5));
		if (settings->baseChance < 0) {settings->baseChance = 0;}
		settings->containerModifier = static_cast<float>(j.value("containerModifier", 1.0));
		if (settings->containerModifier < 0) {settings->containerModifier = 0;}
		settings->bossContainerModifier = static_cast<float>(j.value("bossContainerModifier", 2.0));
		if (settings->bossContainerModifier < 0) {settings->bossContainerModifier = 0;}
		settings->deadBodyModifier = static_cast<float>(j.value("deadBodyModifier", 1.3));
		if (settings->deadBodyModifier < 0) {settings->deadBodyModifier = 0;}
		settings->pickpocketModifier = static_cast<float>(j.value("pickpocketModifier", 1.3));
		if (settings->pickpocketModifier < 0) {settings->pickpocketModifier = 0;}
		settings->doorModifier = static_cast<float>(j.value("doorModifier", 1.0));
		if (settings->doorModifier < 0) {settings->doorModifier = 0;}
		settings->lockedModifier = static_cast<float>(j.value("lockedModifier", 2.0));
		if (settings->lockedModifier < 0) {settings->lockedModifier = 0;}
		settings->lockDifficultyModifier = static_cast<float>(j.value("lockDifficultyModifier", 1.3));
		if (settings->lockDifficultyModifier < 0) {settings->lockDifficultyModifier = 0;}
		settings->arousalModifier = static_cast<float>(j.value("arousalModifier", 1.4));
		if (settings->arousalModifier < 0) {settings->arousalModifier = 0;}
		settings->arousalHalfLife = static_cast<float>(j.value("arousalHalfLife", 12.0));
		if (settings->arousalHalfLife < 0) {settings->arousalHalfLife = 0;}
		settings->keyLossChance = static_cast<float>(j.value("keyLossChance", 80.0));
		if (settings->keyLossChance < 0) {settings->keyLossChance = 0;}
		settings->keyChance = static_cast<float>(j.value("keyChance", 7.0));
		if (settings->keyChance < 0) {settings->keyChance = 0;}
		settings->keyBonus = static_cast<float>(j.value("keyBonus", 1.0));
		if (settings->keyBonus < 0) {settings->keyBonus = 0;}
		settings->keyPickpocketBonus = static_cast<float>(j.value("keyPickpocketBonus", 2.0));
		if (settings->keyPickpocketBonus < 0) {settings->keyPickpocketBonus = 0;}
		settings->magicKeyChance = static_cast<float>(j.value("magicKeyChance", 20.0));
		if (settings->magicKeyChance < 0) {settings->magicKeyChance = 0;}
		settings->keyLuckyBonus = static_cast<float>(j.value("keyLuckyBonus", 1.75));
		if (settings->keyLuckyBonus < 0) {settings->keyLuckyBonus = 0;}
		settings->eventContraptionTime = static_cast<float>(j.value("eventContraptionTime", 4.0));
		if (settings->eventContraptionTime < 0) {settings->eventContraptionTime = 0;}
		settings->followerDeviceMult = static_cast<float>(j.value("followerDeviceMult", 0.75));
		if (settings->followerDeviceMult < 0) {settings->followerDeviceMult = 0;}
		settings->LMBrandingChance = static_cast<float>(j.value("LMBrandingChance", 1.5));
		if (settings->LMBrandingChance < 0) {settings->LMBrandingChance = 0;}
		settings->LMBondageModifier = static_cast<float>(j.value("LMBondageModifier", 5.0));
		if (settings->LMBondageModifier < 0) {settings->LMBondageModifier = 0;}
		settings->LMPainslutSexChance = static_cast<float>(j.value("LMPainslutSexChance", 15.0));
		if (settings->LMPainslutSexChance < 0) {settings->LMPainslutSexChance = 0;}
		settings->oppSummonChance = static_cast<float>(j.value("oppSummonChance", 1.5));
		if (settings->oppSummonChance < 0) {settings->oppSummonChance = 0;}
		settings->oppSummonSexChance = static_cast<float>(j.value("oppSummonSexChance", 25.0));
		if (settings->oppSummonSexChance < 0) {settings->oppSummonSexChance = 0;}
		settings->oppNocturnalHandChance = static_cast<float>(j.value("oppNocturnalHandChance", 3.5));
		if (settings->oppNocturnalHandChance < 0) {settings->oppNocturnalHandChance = 0;}
		settings->oppNocturnalRecastChance = static_cast<float>(j.value("oppNocturnalRecastChance", 33.3));
		if (settings->oppNocturnalRecastChance < 0) {settings->oppNocturnalRecastChance = 0;}
		settings->oppLivingLatexGem = static_cast<float>(j.value("oppLivingLatexGem", 6.0));
		if (settings->oppLivingLatexGem < 0) {settings->oppLivingLatexGem = 0;}
		settings->oppLivingLatexMore = static_cast<float>(j.value("oppLivingLatexMore", 0.0));
		if (settings->oppLivingLatexMore < 0) {settings->oppLivingLatexMore = 0;}
		settings->oppMadnessMultiplier = static_cast<float>(j.value("oppMadnessMultiplier", 1.0));
		if (settings->oppMadnessMultiplier < 0) {settings->oppMadnessMultiplier = 0;}
		settings->lockedLocationBypass = static_cast<float>(j.value("lockedLocationBypass", 0.6));
		if (settings->lockedLocationBypass < 0) {settings->lockedLocationBypass = 0;}
		settings->theftLocationBypass = static_cast<float>(j.value("theftLocationBypass", 1.1));
		if (settings->theftLocationBypass < 0) {settings->theftLocationBypass = 0;}
		settings->playerHomeModifier = static_cast<float>(j.value("playerHomeModifier", 0.0));
		if (settings->playerHomeModifier < 0) {settings->playerHomeModifier = 0;}
		settings->cityModifier = static_cast<float>(j.value("cityModifier", 0.0));
		if (settings->cityModifier < 0) {settings->cityModifier = 0;}
		settings->townModifier = static_cast<float>(j.value("townModifier", 0.0));
		if (settings->townModifier < 0) {settings->townModifier = 0;}
		settings->banditModifier = static_cast<float>(j.value("banditModifier", 1.1));
		if (settings->banditModifier < 0) {settings->banditModifier = 0;}
		settings->draugrModifier = static_cast<float>(j.value("draugrModifier", 1.2));
		if (settings->draugrModifier < 0) {settings->draugrModifier = 0;}
		settings->soulCairnModifier = static_cast<float>(j.value("soulCairnModifier", 1.5));
		if (settings->soulCairnModifier < 0) {settings->soulCairnModifier = 0;}
		settings->dwarvenModifier = static_cast<float>(j.value("dwarvenModifier", 1.2));
		if (settings->dwarvenModifier < 0) {settings->dwarvenModifier = 0;}
		settings->falmerModifier = static_cast<float>(j.value("falmerModifier", 1.3));
		if (settings->falmerModifier < 0) {settings->falmerModifier = 0;}
		settings->forswornModifier = static_cast<float>(j.value("forswornModifier", 1.1));
		if (settings->forswornModifier < 0) {settings->forswornModifier = 0;}
		settings->vampireModifier = static_cast<float>(j.value("vampireModifier", 1.5));
		if (settings->vampireModifier < 0) {settings->vampireModifier = 0;}
		settings->warlockModifier = static_cast<float>(j.value("warlockModifier", 1.5));
		if (settings->warlockModifier < 0) {settings->warlockModifier = 0;}
		settings->dragonLairModifier = static_cast<float>(j.value("dragonLairModifier", 2.0));
		if (settings->dragonLairModifier < 0) {settings->dragonLairModifier = 0;}
		settings->apocryphaModifier = static_cast<float>(j.value("apocryphaModifier", 2.0));
		if (settings->apocryphaModifier < 0) {settings->apocryphaModifier = 0;}
		settings->wildernessModifier = static_cast<float>(j.value("wildernessModifier", 0.9));
		if (settings->wildernessModifier < 0) {settings->wildernessModifier = 0;}
		settings->rDeviceBaseChance = static_cast<float>(j.value("rDeviceBaseChance", 1.5));
		if (settings->rDeviceBaseChance < 0) {settings->rDeviceBaseChance = 0;}
		settings->rDeviceLuckyBonus = static_cast<float>(j.value("rDeviceLuckyBonus", 2.5));
		if (settings->rDeviceLuckyBonus < 0) {settings->rDeviceLuckyBonus = 0;}
		settings->wardingCharmChance = static_cast<float>(j.value("wardingCharmChance", 0.5));
		if (settings->wardingCharmChance < 0) {settings->wardingCharmChance = 0;}
		settings->tatSolventChance = static_cast<float>(j.value("tatSolventChance", 0.5));
		if (settings->tatSolventChance < 0) {settings->tatSolventChance = 0;}
		settings->arousalPotionChance = static_cast<float>(j.value("arousalPotionChance", 5.0));
		if (settings->arousalPotionChance < 0) {settings->arousalPotionChance = 0;}
		settings->arousalPotionLibido = static_cast<float>(j.value("arousalPotionLibido", 10.0));
		if (settings->arousalPotionLibido < 0) {settings->arousalPotionLibido = 0;}
		settings->arousalPotionDuration = static_cast<float>(j.value("arousalPotionDuration", 24.0));
		if (settings->arousalPotionDuration < 0) {settings->arousalPotionDuration = 0;}
		settings->consTalkNude = static_cast<float>(j.value("consTalkNude", 35.0));
		if (settings->consTalkNude < 0) {settings->consTalkNude = 0;}
		settings->consTalkRestrained = static_cast<float>(j.value("consTalkRestrained", 10.0));
		if (settings->consTalkRestrained < 0) {settings->consTalkRestrained = 0;}
		settings->consTalkHeavy = static_cast<float>(j.value("consTalkHeavy", 50.0));
		if (settings->consTalkHeavy < 0) {settings->consTalkHeavy = 0;}
		settings->consTalkTattoo = static_cast<float>(j.value("consTalkTattoo", 25.0));
		if (settings->consTalkTattoo < 0) {settings->consTalkTattoo = 0;}
		settings->consScanSex = static_cast<float>(j.value("consScanSex", 25.0));
		if (settings->consScanSex < 0) {settings->consScanSex = 0;}
		settings->consScanNude = static_cast<float>(j.value("consScanNude", 10.0));
		if (settings->consScanNude < 0) {settings->consScanNude = 0;}
		settings->consScanRestrained = static_cast<float>(j.value("consScanRestrained", 0.0));
		if (settings->consScanRestrained < 0) {settings->consScanRestrained = 0;}
		settings->consScanHeavy = static_cast<float>(j.value("consScanHeavy", 25.0));
		if (settings->consScanHeavy < 0) {settings->consScanHeavy = 0;}
		settings->consScanTattoo = static_cast<float>(j.value("consScanTattoo", 5.0));
		if (settings->consScanTattoo < 0) {settings->consScanTattoo = 0;}
		settings->consScanTrespass = static_cast<float>(j.value("consScanTrespass", 100.0));
		if (settings->consScanTrespass < 0) {settings->consScanTrespass = 0;}
		settings->consGuardScanSex = static_cast<float>(j.value("consGuardScanSex", 50.0));
		if (settings->consGuardScanSex < 0) {settings->consGuardScanSex = 0;}
		settings->consGuardScanNude = static_cast<float>(j.value("consGuardScanNude", 0.0));
		if (settings->consGuardScanNude < 0) {settings->consGuardScanNude = 0;}
		settings->consGuardScanRestrained = static_cast<float>(j.value("consGuardScanRestrained", 0.0));
		if (settings->consGuardScanRestrained < 0) {settings->consGuardScanRestrained = 0;}
		settings->consGuardScanTattoo = static_cast<float>(j.value("consGuardScanTattoo", 0.0));
		if (settings->consGuardScanTattoo < 0) {settings->consGuardScanTattoo = 0;}
		settings->consGuardTalkMult = static_cast<float>(j.value("consGuardTalkMult", 5.0));
		if (settings->consGuardTalkMult < 0) {settings->consGuardTalkMult = 0;}
		settings->consGaggedMultiplier = static_cast<float>(j.value("consGaggedMultiplier", 1.2));
		if (settings->consGaggedMultiplier < 0) {settings->consGaggedMultiplier = 0;}
		settings->sexBaseChance = static_cast<float>(j.value("sexBaseChance", 1.0));
		if (settings->sexBaseChance < 0) {settings->sexBaseChance = 0;}
		settings->sexCreatureModifier = static_cast<float>(j.value("sexCreatureModifier", 0.9));
		if (settings->sexCreatureModifier < 0) {settings->sexCreatureModifier = 0;}
		settings->sexFollowerModifier = static_cast<float>(j.value("sexFollowerModifier", 1.0));
		if (settings->sexFollowerModifier < 0) {settings->sexFollowerModifier = 0;}
		settings->sexSummonModifier = static_cast<float>(j.value("sexSummonModifier", 2.0));
		if (settings->sexSummonModifier < 0) {settings->sexSummonModifier = 0;}
		settings->sexRestraintsModifier = static_cast<float>(j.value("sexRestraintsModifier", 1.1));
		if (settings->sexRestraintsModifier < 0) {settings->sexRestraintsModifier = 0;}
		settings->sexSafeLocationModifier = static_cast<float>(j.value("sexSafeLocationModifier", 1.3));
		if (settings->sexSafeLocationModifier < 0) {settings->sexSafeLocationModifier = 0;}
		settings->sexUnsafeLocationModifier = static_cast<float>(j.value("sexUnsafeLocationModifier", 0.1));
		if (settings->sexUnsafeLocationModifier < 0) {settings->sexUnsafeLocationModifier = 0;}
		settings->sexPlayerNudityModifier = static_cast<float>(j.value("sexPlayerNudityModifier", 2.0));
		if (settings->sexPlayerNudityModifier < 0) {settings->sexPlayerNudityModifier = 0;}
		settings->sexNPCNudityModifier = static_cast<float>(j.value("sexNPCNudityModifier", 2.0));
		if (settings->sexNPCNudityModifier < 0) {settings->sexNPCNudityModifier = 0;}
		settings->sexPlayerArousalModifier = static_cast<float>(j.value("sexPlayerArousalModifier", 2.5));
		if (settings->sexPlayerArousalModifier < 0) {settings->sexPlayerArousalModifier = 0;}
		settings->sexNPCArousalModifier = static_cast<float>(j.value("sexNPCArousalModifier", 2.5));
		if (settings->sexNPCArousalModifier < 0) {settings->sexNPCArousalModifier = 0;}
		settings->sexAllureModifier = static_cast<float>(j.value("sexAllureModifier", 5.0));
		if (settings->sexAllureModifier < 0) {settings->sexAllureModifier = 0;}
		settings->sexSummonerModifier = static_cast<float>(j.value("sexSummonerModifier", 7.5));
		if (settings->sexSummonerModifier < 0) {settings->sexSummonerModifier = 0;}
		settings->onlyLockedDoors = static_cast<bool>(j.value("onlyLockedDoors", true));
		settings->eventScaling = static_cast<bool>(j.value("eventScaling", true));
		settings->bossOnlyHeavy = static_cast<bool>(j.value("bossOnlyHeavy", true));
		settings->stripPlayerOnEvent = static_cast<bool>(j.value("stripPlayerOnEvent", true));
		settings->stripOnlyKeywords = static_cast<bool>(j.value("stripOnlyKeywords", true));
		settings->beltPlugs = static_cast<bool>(j.value("beltPlugs", true));
		settings->useGenderedChastity = static_cast<bool>(j.value("useGenderedChastity", true));
		settings->plugsDontCount = static_cast<bool>(j.value("plugsDontCount", true));
		settings->useGenderedPlugs = static_cast<bool>(j.value("useGenderedPlugs", false));
		settings->allowLegShackles = static_cast<bool>(j.value("allowLegShackles", false));
		settings->keyForgiveness = static_cast<bool>(j.value("keyForgiveness", true));
		settings->magicKeyOppressive = static_cast<bool>(j.value("magicKeyOppressive", false));
		settings->preferRelevantKeys = static_cast<bool>(j.value("preferRelevantKeys", true));
		settings->vanishingKeys = static_cast<bool>(j.value("vanishingKeys", true));
		settings->eventContDevices = static_cast<bool>(j.value("eventContDevices", true));
		settings->eventContAllDevices = static_cast<bool>(j.value("eventContAllDevices", false));
		settings->followerBondageEvent = static_cast<bool>(j.value("followerBondageEvent", false));
		settings->followerTattooEvent = static_cast<bool>(j.value("followerTattooEvent", false));
		settings->onlyFemaleFollowers = static_cast<bool>(j.value("onlyFemaleFollowers", true));
		settings->followerHeavyRestraints = static_cast<bool>(j.value("followerHeavyRestraints", false));
		settings->followerGags = static_cast<bool>(j.value("followerGags", false));
		settings->onlyUseUnforgivingDevices = static_cast<bool>(j.value("onlyUseUnforgivingDevices", true));
		settings->udUseAbadon = static_cast<bool>(j.value("udUseAbadon", false));
		settings->udUseAbadonSets = static_cast<bool>(j.value("udUseAbadonSets", false));
		settings->udUseMisc = static_cast<bool>(j.value("udUseMisc", true));
		settings->LMAllureSkipRequirements = static_cast<bool>(j.value("LMAllureSkipRequirements", true));
		settings->LMBrandingPunish = static_cast<bool>(j.value("LMBrandingPunish", true));
		settings->LMNudityChestOnly = static_cast<bool>(j.value("LMNudityChestOnly", false));
		settings->LMNuditySkipGagged = static_cast<bool>(j.value("LMNuditySkipGagged", true));
		settings->oppOneAtATime = static_cast<bool>(j.value("oppOneAtATime", true));
		settings->oppSCollarDrainsMagicka = static_cast<bool>(j.value("oppSCollarDrainsMagicka", true));
		settings->oppSCollarAutoRemove = static_cast<bool>(j.value("oppSCollarAutoRemove", true));
		settings->oppDwarvenHeavyRestraint = static_cast<bool>(j.value("oppDwarvenHeavyRestraint", false));
		settings->oppDwarvenRequireLoc = static_cast<bool>(j.value("oppDwarvenRequireLoc", true));
		settings->oppLivingLatexGemAll = static_cast<bool>(j.value("oppLivingLatexGemAll", false));
		settings->oppLivingLatexHeavy = static_cast<bool>(j.value("oppLivingLatexHeavy", false));
		settings->oppLivingLatexRequireRem = static_cast<bool>(j.value("oppLivingLatexRequireRem", true));
		settings->oppLivingLatexOpen = static_cast<bool>(j.value("oppLivingLatexOpen", false));
		settings->oppMadnessOnlyDevices = static_cast<bool>(j.value("oppMadnessOnlyDevices", true));
		settings->oppMadnessBeltFilter = static_cast<bool>(j.value("oppMadnessBeltFilter", true));
		settings->oppMadnessChaos = static_cast<bool>(j.value("oppMadnessChaos", false));
		settings->enableQuestInteractions = static_cast<bool>(j.value("enableQuestInteractions", true));
		settings->enableQIMalkoran = static_cast<bool>(j.value("enableQIMalkoran", true));
		settings->enableQISanguine = static_cast<bool>(j.value("enableQISanguine", true));
		settings->enableQIBlackStar = static_cast<bool>(j.value("enableQIBlackStar", true));
		settings->enableQIMindOfMadness = static_cast<bool>(j.value("enableQIMindOfMadness", true));
		settings->enableQILaidToRest = static_cast<bool>(j.value("enableQILaidToRest", true));
		settings->enableQICriedWolf = static_cast<bool>(j.value("enableQICriedWolf", true));
		settings->enableQIDelivery = static_cast<bool>(j.value("enableQIDelivery", true));
		settings->enableQIBarenziah = static_cast<bool>(j.value("enableQIBarenziah", true));
		settings->enableQIProvingHonor = static_cast<bool>(j.value("enableQIProvingHonor", true));
		settings->enableQISaarthal = static_cast<bool>(j.value("enableQISaarthal", true));
		settings->enableQIBrelyna = static_cast<bool>(j.value("enableQIBrelyna", true));
		settings->enableQIDwemerMuseum = static_cast<bool>(j.value("enableQIDwemerMuseum", true));
		settings->enableQITrinityRestored = static_cast<bool>(j.value("enableQITrinityRestored", true));
		settings->enableQIThalmorEmbassy = static_cast<bool>(j.value("enableQIThalmorEmbassy", true));
		settings->enableQIRingmaker = static_cast<bool>(j.value("enableQIRingmaker", true));
		settings->oppDeviceAntiCheat = static_cast<bool>(j.value("oppDeviceAntiCheat", true));
		settings->generalDeviceAntiCheat = static_cast<bool>(j.value("generalDeviceAntiCheat", false));
		settings->noMessageBoxes = static_cast<bool>(j.value("noMessageBoxes", false));
		settings->eventVisualEffects = static_cast<bool>(j.value("eventVisualEffects", true));
		settings->surpriseMode = static_cast<bool>(j.value("surpriseMode", false));
		settings->dragonHoard = static_cast<bool>(j.value("dragonHoard", true));
		settings->bossExtraGold = static_cast<bool>(j.value("bossExtraGold", true));
		settings->useThemes = static_cast<bool>(j.value("useThemes", false));
		settings->disableForce3rdPerson = static_cast<bool>(j.value("disableForce3rdPerson", true));
		settings->enableSlowStrip = static_cast<bool>(j.value("enableSlowStrip", false));
		settings->restrictFastTravel = static_cast<bool>(j.value("restrictFastTravel", true));
		settings->restrictFastTravelPart = static_cast<bool>(j.value("restrictFastTravelPart", false));
		settings->restrictFastTravelFull = static_cast<bool>(j.value("restrictFastTravelFull", false));
		settings->useDeviceNamesInThemes = static_cast<bool>(j.value("useDeviceNamesInThemes", true));
		settings->saveDependentSettings = static_cast<bool>(j.value("saveDependentSettings", false));
		settings->ANDSexTopless = static_cast<bool>(j.value("ANDSexTopless", true));
		settings->ANDSexBottomless = static_cast<bool>(j.value("ANDSexBottomless", true));
		settings->ANDSexShowingChest = static_cast<bool>(j.value("ANDSexShowingChest", true));
		settings->ANDSexShowingGenitals = static_cast<bool>(j.value("ANDSexShowingGenitals", true));
		settings->ANDSexShowingAss = static_cast<bool>(j.value("ANDSexShowingAss", false));
		settings->ANDSexShowingBra = static_cast<bool>(j.value("ANDSexShowingBra", false));
		settings->ANDSexShowingUnderwear = static_cast<bool>(j.value("ANDSexShowingUnderwear", false));
		settings->ANDConsTopless = static_cast<bool>(j.value("ANDConsTopless", true));
		settings->ANDConsBottomless = static_cast<bool>(j.value("ANDConsBottomless", true));
		settings->ANDConsShowingChest = static_cast<bool>(j.value("ANDConsShowingChest", true));
		settings->ANDConsShowingGenitals = static_cast<bool>(j.value("ANDConsShowingGenitals", true));
		settings->ANDConsShowingAss = static_cast<bool>(j.value("ANDConsShowingAss", false));
		settings->ANDConsShowingBra = static_cast<bool>(j.value("ANDConsShowingBra", false));
		settings->ANDConsShowingUnderwear = static_cast<bool>(j.value("ANDConsShowingUnderwear", false));
		settings->consEnable = static_cast<bool>(j.value("consEnable", true));
		settings->consGuardNormal = static_cast<bool>(j.value("consGuardNormal", false));
		settings->consFineGuardFaction = static_cast<bool>(j.value("consFineGuardFaction", true));
		settings->consFallthrough = static_cast<bool>(j.value("consFallthrough", false));
		settings->consAllowFollowers = static_cast<bool>(j.value("consAllowFollowers", false));
		settings->consFollowersBlock = static_cast<bool>(j.value("consFollowersBlock", false));
		settings->consTolerableChastity = static_cast<bool>(j.value("consTolerableChastity", true));
		settings->consUseRelationships = static_cast<bool>(j.value("consUseRelationships", true));
		settings->consRelationBondage = static_cast<bool>(j.value("consRelationBondage", false));
		settings->consRandomHeavyBondage = static_cast<bool>(j.value("consRandomHeavyBondage", false));
		settings->consBondageIgnoreMax = static_cast<bool>(j.value("consBondageIgnoreMax", false));
		settings->consTattooCurseNumbers = static_cast<bool>(j.value("consTattooCurseNumbers", false));
		settings->sexEnabled = static_cast<bool>(j.value("sexEnabled", true));
		settings->sexAggressiveAnims = static_cast<bool>(j.value("sexAggressiveAnims", false));
		settings->sexFilterDevices = static_cast<bool>(j.value("sexFilterDevices", true));
		settings->sexFilterFuta = static_cast<bool>(j.value("sexFilterFuta", true));
		settings->sexRandomEnabled = static_cast<bool>(j.value("sexRandomEnabled", false));
		settings->sexRandAllowFollowers = static_cast<bool>(j.value("sexRandAllowFollowers", true));
		settings->sexRandAllowNPCs = static_cast<bool>(j.value("sexRandAllowNPCs", false));
		settings->sexRandAllowSummons = static_cast<bool>(j.value("sexRandAllowSummons", true));
		settings->sexRandAllowCreatures = static_cast<bool>(j.value("sexRandAllowCreatures", false));
		settings->sexAllowMale = static_cast<bool>(j.value("sexAllowMale", true));
		settings->sexAllowFemale = static_cast<bool>(j.value("sexAllowFemale", true));
		settings->sexAllowFuta = static_cast<bool>(j.value("sexAllowFuta", true));
		settings->sexAllowCreature = static_cast<bool>(j.value("sexAllowCreature", false));
		settings->sexAllowFarmAnimals = static_cast<bool>(j.value("sexAllowFarmAnimals", false));
		settings->sexRequireAllPlayer = static_cast<bool>(j.value("sexRequireAllPlayer", false));
		settings->sexRequirePlayerCollar = static_cast<bool>(j.value("sexRequirePlayerCollar", false));
		settings->sexRequirePlayerHeavy = static_cast<bool>(j.value("sexRequirePlayerHeavy", false));
		settings->sexRequirePlayerNude = static_cast<bool>(j.value("sexRequirePlayerNude", true));
		settings->sexRequireAllNPC = static_cast<bool>(j.value("sexRequireAllNPC", false));
		settings->sexRequireNPCCollar = static_cast<bool>(j.value("sexRequireNPCCollar", false));
		settings->sexRequireNPCNude = static_cast<bool>(j.value("sexRequireNPCNude", true));
		settings->sexRequireBoth = static_cast<bool>(j.value("sexRequireBoth", true));
		settings->sexFollowerBypassPlayer = static_cast<bool>(j.value("sexFollowerBypassPlayer", false));
		settings->sexFollowerBypassNPC = static_cast<bool>(j.value("sexFollowerBypassNPC", true));
		settings->sexSpouseBypassPlayer = static_cast<bool>(j.value("sexSpouseBypassPlayer", true));
		settings->sexSpouseBypassNPC = static_cast<bool>(j.value("sexSpouseBypassNPC", true));
		settings->sexSummonBypassPlayer = static_cast<bool>(j.value("sexSummonBypassPlayer", false));
		settings->sexSummonBypassNPC = static_cast<bool>(j.value("sexSummonBypassNPC", false));
		settings->ModSuspended = static_cast<bool>(j.value("ModSuspended", false));
		settings->setAllDefaultSettings = static_cast<bool>(j.value("setAllDefaultSettings", false));
		settings->helpSlaveTatsNGInstalled = static_cast<bool>(j.value("helpSlaveTatsNGInstalled", false));
		settings->helpLewdMarksInstalled = static_cast<bool>(j.value("helpLewdMarksInstalled", false));
		settings->helpRapeTattoosInstalled = static_cast<bool>(j.value("helpRapeTattoosInstalled", false));
		settings->helpOSLArousedInstalled = static_cast<bool>(j.value("helpOSLArousedInstalled", false));
		settings->helpSLOArousedInstalled = static_cast<bool>(j.value("helpSLOArousedInstalled", false));
		settings->helpUDInstalled = static_cast<bool>(j.value("helpUDInstalled", false));
		settings->helpQuickLootInstalled = static_cast<bool>(j.value("helpQuickLootInstalled", false));
		settings->helpDWDInstalled = static_cast<bool>(j.value("helpDWDInstalled", false));
		settings->helpANDInstalled = static_cast<bool>(j.value("helpANDInstalled", false));
		settings->helpCockCagesWorking = static_cast<bool>(j.value("helpCockCagesWorking", false));
		settings->debugMode = static_cast<bool>(j.value("debugMode", false));
		settings->excludedFollowers = j.value("excludedFollowers", "");
		settings->followerOverrideTheme = j.value("followerOverrideTheme", "");
		settings->followerSkipKeywords = j.value("followerSkipKeywords", "");
		settings->LMNudityAditionalForms = j.value("LMNudityAditionalForms", "");
		settings->lastLoadedVersion = j.value("lastLoadedVersion", DCURSES_VERSION);
		//CODEGEN_END_FROMJSON

		log::trace("Read settings from file {}", SETTINGS_FILE);

		return true;
	}
	void MCMSetHelpPageOptions() {
		SetMCMBool("helpSlaveTatsNGInstalled", slavetats_ng::iface != nullptr);
		SetMCMBool("helpLewdMarksInstalled", CheckLewdMarksInstalled());
		SetMCMBool("helpRapeTattoosInstalled", CheckRapeTattoos());
		SetMCMBool("helpOSLArousedInstalled", CheckOSL());
		SetMCMBool("helpSLOArousedInstalled", CheckSLOAroused());
		SetMCMBool("helpUDInstalled", CheckUD());
		SetMCMBool("helpQuickLootInstalled", QuickLoot::API3::QuickLootAPI::IsReady() || QuickLoot::API4::QuickLootAPI::IsReady());
		SetMCMBool("helpDWDInstalled", CheckWickedDevices());
		SetMCMBool("helpANDInstalled", CheckAND());
		SetMCMBool("helpCockCagesWorking", (Devices::GetSingleton()->beltsCages.list.size() > 0));

		log::trace("Setting MCM Help Menu");
	}
	void PushSettingsToMCM() {
		auto settings = Settings::GetSingleton();

		//CODEGEN_START_PUSHMCM
		SetMCMInt("minGoldRequired",settings->minGoldRequired);
		SetMCMInt("eventScalingMod",settings->eventScalingMod);
		SetMCMInt("minRestraints",settings->minRestraints);
		SetMCMInt("maxRestraints",settings->maxRestraints);
		SetMCMInt("bossAditionalRestraints",settings->bossAditionalRestraints);
		SetMCMInt("restraintCap",settings->restraintCap);
		SetMCMInt("minArousal",settings->minArousal);
		SetMCMInt("beltWeight",settings->beltWeight);
		SetMCMInt("chastityPiercingWeight",settings->chastityPiercingWeight);
		SetMCMInt("braWeight",settings->braWeight);
		SetMCMInt("plugsWeight",settings->plugsWeight);
		SetMCMInt("lockingPlugsWeight",settings->lockingPlugsWeight);
		SetMCMInt("inflatablePlugsWeight",settings->inflatablePlugsWeight);
		SetMCMInt("nipplePiercingsWeight",settings->nipplePiercingsWeight);
		SetMCMInt("vaginalPiercingsWeight",settings->vaginalPiercingsWeight);
		SetMCMInt("corsetWeight",settings->corsetWeight);
		SetMCMInt("beltedCorsetsWeight",settings->beltedCorsetsWeight);
		SetMCMInt("slaveHarnessWeight",settings->slaveHarnessWeight);
		SetMCMInt("chastityHarnessWeight",settings->chastityHarnessWeight);
		SetMCMInt("armbinderWeight",settings->armbinderWeight);
		SetMCMInt("elbowbinderWeight",settings->elbowbinderWeight);
		SetMCMInt("yokeWeight",settings->yokeWeight);
		SetMCMInt("shacklesWeight",settings->shacklesWeight);
		SetMCMInt("straitjacketWeight",settings->straitjacketWeight);
		SetMCMInt("straitjacketLegbinderWeight",settings->straitjacketLegbinderWeight);
		SetMCMInt("petSuitWeight",settings->petSuitWeight);
		SetMCMInt("collarWeight",settings->collarWeight);
		SetMCMInt("armCuffsWeight",settings->armCuffsWeight);
		SetMCMInt("legCuffsWeight",settings->legCuffsWeight);
		SetMCMInt("gagWeight",settings->gagWeight);
		SetMCMInt("ringGagWeight",settings->ringGagWeight);
		SetMCMInt("largeGagWeight",settings->largeGagWeight);
		SetMCMInt("largeRingGagWeight",settings->largeRingGagWeight);
		SetMCMInt("blindfoldWeight",settings->blindfoldWeight);
		SetMCMInt("hoodBothWeight",settings->hoodBothWeight);
		SetMCMInt("hoodGagWeight",settings->hoodGagWeight);
		SetMCMInt("hoodBlindWeight",settings->hoodBlindWeight);
		SetMCMInt("hoodNoneWeight",settings->hoodNoneWeight);
		SetMCMInt("catsuitWeight",settings->catsuitWeight);
		SetMCMInt("hobbleSkirtWeight",settings->hobbleSkirtWeight);
		SetMCMInt("hobbleSkirtDifficulty",settings->hobbleSkirtDifficulty);
		SetMCMInt("bootsWeight",settings->bootsWeight);
		SetMCMInt("glovesWeight",settings->glovesWeight);
		SetMCMInt("mittensWeight",settings->mittensWeight);
		SetMCMInt("minKeysLooted",settings->minKeysLooted);
		SetMCMInt("maxKeysLooted",settings->maxKeysLooted);
		SetMCMInt("maxHeldKeys",settings->maxHeldKeys);
		SetMCMInt("restraintsKeyWeight",settings->restraintsKeyWeight);
		SetMCMInt("chastityKeyWeight",settings->chastityKeyWeight);
		SetMCMInt("piercingToolWeight",settings->piercingToolWeight);
		SetMCMInt("eventStandardWeight",settings->eventStandardWeight);
		SetMCMInt("eventStandardBossReduction",settings->eventStandardBossReduction);
		SetMCMInt("eventOppressiveWeight",settings->eventOppressiveWeight);
		SetMCMInt("eventContraptionWeight",settings->eventContraptionWeight);
		SetMCMInt("eventContDeviceOverride",settings->eventContDeviceOverride);
		SetMCMInt("eventWickedWeight",settings->eventWickedWeight);
		SetMCMInt("eventTattooWeight",settings->eventTattooWeight);
		SetMCMInt("eventTattooMin",settings->eventTattooMin);
		SetMCMInt("eventTattooMax",settings->eventTattooMax);
		SetMCMInt("eventTattooCap",settings->eventTattooCap);
		SetMCMInt("eventLewdMarkWeight",settings->eventLewdMarkWeight);
		SetMCMInt("eventSimpleSlaveryWeight",settings->eventSimpleSlaveryWeight);
		SetMCMInt("eventSSMinRestraints",settings->eventSSMinRestraints);
		SetMCMInt("eventAbadonWeight",settings->eventAbadonWeight);
		SetMCMInt("eventAbadonWarriorWeight",settings->eventAbadonWarriorWeight);
		SetMCMInt("eventAbadonWarriorCount",settings->eventAbadonWarriorCount);
		SetMCMInt("eventAbadonScoutWeight",settings->eventAbadonScoutWeight);
		SetMCMInt("eventAbadonScoutCount",settings->eventAbadonScoutCount);
		SetMCMInt("eventAbadonWitchWeight",settings->eventAbadonWitchWeight);
		SetMCMInt("eventAbadonWitchCount",settings->eventAbadonWitchCount);
		SetMCMInt("followerDeviceModifier",settings->followerDeviceModifier);
		SetMCMInt("LMAllureWeight",settings->LMAllureWeight);
		SetMCMInt("LMAllureMod",settings->LMAllureMod);
		SetMCMInt("LMAllureSex",settings->LMAllureSex);
		SetMCMInt("LMHeatWeight",settings->LMHeatWeight);
		SetMCMInt("LMHeatMod",settings->LMHeatMod);
		SetMCMInt("LMHeatContainerCount",settings->LMHeatContainerCount);
		SetMCMInt("LMBrandingWeight",settings->LMBrandingWeight);
		SetMCMInt("LMBrandingTotal",settings->LMBrandingTotal);
		SetMCMInt("LMHealslutWeight",settings->LMHealslutWeight);
		SetMCMInt("LMHealslutHealing",settings->LMHealslutHealing);
		SetMCMInt("LMHealslutArousal",settings->LMHealslutArousal);
		SetMCMInt("LMHealslutDuration",settings->LMHealslutDuration);
		SetMCMInt("LMBondageWeight",settings->LMBondageWeight);
		SetMCMInt("LMBondageDeviceCapAdd",settings->LMBondageDeviceCapAdd);
		SetMCMInt("LMBondageDeviceCount",settings->LMBondageDeviceCount);
		SetMCMInt("LMNudityWeight",settings->LMNudityWeight);
		SetMCMInt("LMNudityTalkTimes",settings->LMNudityTalkTimes);
		SetMCMInt("LMPainslutWeight",settings->LMPainslutWeight);
		SetMCMInt("LMPainslutMod",settings->LMPainslutMod);
		SetMCMInt("LMPainslutCount",settings->LMPainslutCount);
		SetMCMInt("LMPainslutArousal",settings->LMPainslutArousal);
		SetMCMInt("oppSummonerCollarWeight",settings->oppSummonerCollarWeight);
		SetMCMInt("oppSummonerSexCount",settings->oppSummonerSexCount);
		SetMCMInt("oppDwarvenCuirassWeight",settings->oppDwarvenCuirassWeight);
		SetMCMInt("oppDwarvenValueNeeded",settings->oppDwarvenValueNeeded);
		SetMCMInt("oppDwarvenArousal",settings->oppDwarvenArousal);
		SetMCMInt("oppNocturnalWeight",settings->oppNocturnalWeight);
		SetMCMInt("oppNocturnalSexCount",settings->oppNocturnalSexCount);
		SetMCMInt("oppNocturnalPeople",settings->oppNocturnalPeople);
		SetMCMInt("oppNocturnalArousal",settings->oppNocturnalArousal);
		SetMCMInt("oppLivingLatexWeight",settings->oppLivingLatexWeight);
		SetMCMInt("oppLivingLatexStartTime",settings->oppLivingLatexStartTime);
		SetMCMInt("oppMadnessPlugWeight",settings->oppMadnessPlugWeight);
		SetMCMInt("oppMadnessPlugIterations",settings->oppMadnessPlugIterations);
		SetMCMInt("oppMadnessplugOrgasms",settings->oppMadnessplugOrgasms);
		SetMCMInt("consScanRadius",settings->consScanRadius);
		SetMCMInt("consScanInterval",settings->consScanInterval);
		SetMCMInt("consFineForSex",settings->consFineForSex);
		SetMCMInt("consFineForNudity",settings->consFineForNudity);
		SetMCMInt("consFineForRestraints",settings->consFineForRestraints);
		SetMCMInt("consFineForTattoos",settings->consFineForTattoos);
		SetMCMInt("consRestrainedCount",settings->consRestrainedCount);
		SetMCMInt("consTattooCount",settings->consTattooCount);
		SetMCMInt("consSexWeight",settings->consSexWeight);
		SetMCMInt("consRandomBondageWeight",settings->consRandomBondageWeight);
		SetMCMInt("consTattooWeight",settings->consTattooWeight);
		SetMCMInt("consMercyWeight",settings->consMercyWeight);
		SetMCMInt("sexCooldown",settings->sexCooldown);
		SetMCMInt("sexArousalModifierCutoff",settings->sexArousalModifierCutoff);
		SetMCMInt("sexScalingModifier",settings->sexScalingModifier);
		SetMCMInt("sexSearchRadius",settings->sexSearchRadius);
		SetMCMInt("sexSearchInterval",settings->sexSearchInterval);
		SetMCMInt("sexRequiredPlayerBindings",settings->sexRequiredPlayerBindings);
		SetMCMInt("sexRequiredPlayerArousal",settings->sexRequiredPlayerArousal);
		SetMCMInt("sexRequiredPlayerTattoos",settings->sexRequiredPlayerTattoos);
		SetMCMInt("sexRequiredNPCBindings",settings->sexRequiredNPCBindings);
		SetMCMInt("sexRequiredNPCArousal",settings->sexRequiredNPCArousal);
		SetMCMInt("sexRequiredNPCTattoos",settings->sexRequiredNPCTattoos);
		SetMCMInt("sexRequiredCreatureArousal",settings->sexRequiredCreatureArousal);
		SetMCMInt("LMAllureColor",settings->LMAllureColor);
		SetMCMInt("LMHeatColor",settings->LMHeatColor);
		SetMCMInt("LMBrandingColor",settings->LMBrandingColor);
		SetMCMInt("LMHealslutColor",settings->LMHealslutColor);
		SetMCMInt("LMBondageColor",settings->LMBondageColor);
		SetMCMInt("LMNudityColor",settings->LMNudityColor);
		SetMCMInt("LMPainslutColor",settings->LMPainslutColor);
		SetMCMInt("setDebugKey",settings->setDebugKey);
		SetMCMInt("oppSummonerNPC",settings->oppSummonerNPC);
		SetMCMFloat("baseChance",settings->baseChance);
		SetMCMFloat("containerModifier",settings->containerModifier);
		SetMCMFloat("bossContainerModifier",settings->bossContainerModifier);
		SetMCMFloat("deadBodyModifier",settings->deadBodyModifier);
		SetMCMFloat("pickpocketModifier",settings->pickpocketModifier);
		SetMCMFloat("doorModifier",settings->doorModifier);
		SetMCMFloat("lockedModifier",settings->lockedModifier);
		SetMCMFloat("lockDifficultyModifier",settings->lockDifficultyModifier);
		SetMCMFloat("arousalModifier",settings->arousalModifier);
		SetMCMFloat("arousalHalfLife",settings->arousalHalfLife);
		SetMCMFloat("keyLossChance",settings->keyLossChance);
		SetMCMFloat("keyChance",settings->keyChance);
		SetMCMFloat("keyBonus",settings->keyBonus);
		SetMCMFloat("keyPickpocketBonus",settings->keyPickpocketBonus);
		SetMCMFloat("magicKeyChance",settings->magicKeyChance);
		SetMCMFloat("keyLuckyBonus",settings->keyLuckyBonus);
		SetMCMFloat("eventContraptionTime",settings->eventContraptionTime);
		SetMCMFloat("followerDeviceMult",settings->followerDeviceMult);
		SetMCMFloat("LMBrandingChance",settings->LMBrandingChance);
		SetMCMFloat("LMBondageModifier",settings->LMBondageModifier);
		SetMCMFloat("LMPainslutSexChance",settings->LMPainslutSexChance);
		SetMCMFloat("oppSummonChance",settings->oppSummonChance);
		SetMCMFloat("oppSummonSexChance",settings->oppSummonSexChance);
		SetMCMFloat("oppNocturnalHandChance",settings->oppNocturnalHandChance);
		SetMCMFloat("oppNocturnalRecastChance",settings->oppNocturnalRecastChance);
		SetMCMFloat("oppLivingLatexGem",settings->oppLivingLatexGem);
		SetMCMFloat("oppLivingLatexMore",settings->oppLivingLatexMore);
		SetMCMFloat("oppMadnessMultiplier",settings->oppMadnessMultiplier);
		SetMCMFloat("lockedLocationBypass",settings->lockedLocationBypass);
		SetMCMFloat("theftLocationBypass",settings->theftLocationBypass);
		SetMCMFloat("playerHomeModifier",settings->playerHomeModifier);
		SetMCMFloat("cityModifier",settings->cityModifier);
		SetMCMFloat("townModifier",settings->townModifier);
		SetMCMFloat("banditModifier",settings->banditModifier);
		SetMCMFloat("draugrModifier",settings->draugrModifier);
		SetMCMFloat("soulCairnModifier",settings->soulCairnModifier);
		SetMCMFloat("dwarvenModifier",settings->dwarvenModifier);
		SetMCMFloat("falmerModifier",settings->falmerModifier);
		SetMCMFloat("forswornModifier",settings->forswornModifier);
		SetMCMFloat("vampireModifier",settings->vampireModifier);
		SetMCMFloat("warlockModifier",settings->warlockModifier);
		SetMCMFloat("dragonLairModifier",settings->dragonLairModifier);
		SetMCMFloat("apocryphaModifier",settings->apocryphaModifier);
		SetMCMFloat("wildernessModifier",settings->wildernessModifier);
		SetMCMFloat("rDeviceBaseChance",settings->rDeviceBaseChance);
		SetMCMFloat("rDeviceLuckyBonus",settings->rDeviceLuckyBonus);
		SetMCMFloat("wardingCharmChance",settings->wardingCharmChance);
		SetMCMFloat("tatSolventChance",settings->tatSolventChance);
		SetMCMFloat("arousalPotionChance",settings->arousalPotionChance);
		SetMCMFloat("arousalPotionLibido",settings->arousalPotionLibido);
		SetMCMFloat("arousalPotionDuration",settings->arousalPotionDuration);
		SetMCMFloat("consTalkNude",settings->consTalkNude);
		SetMCMFloat("consTalkRestrained",settings->consTalkRestrained);
		SetMCMFloat("consTalkHeavy",settings->consTalkHeavy);
		SetMCMFloat("consTalkTattoo",settings->consTalkTattoo);
		SetMCMFloat("consScanSex",settings->consScanSex);
		SetMCMFloat("consScanNude",settings->consScanNude);
		SetMCMFloat("consScanRestrained",settings->consScanRestrained);
		SetMCMFloat("consScanHeavy",settings->consScanHeavy);
		SetMCMFloat("consScanTattoo",settings->consScanTattoo);
		SetMCMFloat("consScanTrespass",settings->consScanTrespass);
		SetMCMFloat("consGuardScanSex",settings->consGuardScanSex);
		SetMCMFloat("consGuardScanNude",settings->consGuardScanNude);
		SetMCMFloat("consGuardScanRestrained",settings->consGuardScanRestrained);
		SetMCMFloat("consGuardScanTattoo",settings->consGuardScanTattoo);
		SetMCMFloat("consGuardTalkMult",settings->consGuardTalkMult);
		SetMCMFloat("consGaggedMultiplier",settings->consGaggedMultiplier);
		SetMCMFloat("sexBaseChance",settings->sexBaseChance);
		SetMCMFloat("sexCreatureModifier",settings->sexCreatureModifier);
		SetMCMFloat("sexFollowerModifier",settings->sexFollowerModifier);
		SetMCMFloat("sexSummonModifier",settings->sexSummonModifier);
		SetMCMFloat("sexRestraintsModifier",settings->sexRestraintsModifier);
		SetMCMFloat("sexSafeLocationModifier",settings->sexSafeLocationModifier);
		SetMCMFloat("sexUnsafeLocationModifier",settings->sexUnsafeLocationModifier);
		SetMCMFloat("sexPlayerNudityModifier",settings->sexPlayerNudityModifier);
		SetMCMFloat("sexNPCNudityModifier",settings->sexNPCNudityModifier);
		SetMCMFloat("sexPlayerArousalModifier",settings->sexPlayerArousalModifier);
		SetMCMFloat("sexNPCArousalModifier",settings->sexNPCArousalModifier);
		SetMCMFloat("sexAllureModifier",settings->sexAllureModifier);
		SetMCMFloat("sexSummonerModifier",settings->sexSummonerModifier);
		SetMCMBool("onlyLockedDoors",settings->onlyLockedDoors);
		SetMCMBool("eventScaling",settings->eventScaling);
		SetMCMBool("bossOnlyHeavy",settings->bossOnlyHeavy);
		SetMCMBool("stripPlayerOnEvent",settings->stripPlayerOnEvent);
		SetMCMBool("stripOnlyKeywords",settings->stripOnlyKeywords);
		SetMCMBool("beltPlugs",settings->beltPlugs);
		SetMCMBool("useGenderedChastity",settings->useGenderedChastity);
		SetMCMBool("plugsDontCount",settings->plugsDontCount);
		SetMCMBool("useGenderedPlugs",settings->useGenderedPlugs);
		SetMCMBool("allowLegShackles",settings->allowLegShackles);
		SetMCMBool("keyForgiveness",settings->keyForgiveness);
		SetMCMBool("magicKeyOppressive",settings->magicKeyOppressive);
		SetMCMBool("preferRelevantKeys",settings->preferRelevantKeys);
		SetMCMBool("vanishingKeys",settings->vanishingKeys);
		SetMCMBool("eventContDevices",settings->eventContDevices);
		SetMCMBool("eventContAllDevices",settings->eventContAllDevices);
		SetMCMBool("followerBondageEvent",settings->followerBondageEvent);
		SetMCMBool("followerTattooEvent",settings->followerTattooEvent);
		SetMCMBool("onlyFemaleFollowers",settings->onlyFemaleFollowers);
		SetMCMBool("followerHeavyRestraints",settings->followerHeavyRestraints);
		SetMCMBool("followerGags",settings->followerGags);
		SetMCMBool("onlyUseUnforgivingDevices",settings->onlyUseUnforgivingDevices);
		SetMCMBool("udUseAbadon",settings->udUseAbadon);
		SetMCMBool("udUseAbadonSets",settings->udUseAbadonSets);
		SetMCMBool("udUseMisc",settings->udUseMisc);
		SetMCMBool("LMAllureSkipRequirements",settings->LMAllureSkipRequirements);
		SetMCMBool("LMBrandingPunish",settings->LMBrandingPunish);
		SetMCMBool("LMNudityChestOnly",settings->LMNudityChestOnly);
		SetMCMBool("LMNuditySkipGagged",settings->LMNuditySkipGagged);
		SetMCMBool("oppOneAtATime",settings->oppOneAtATime);
		SetMCMBool("oppSCollarDrainsMagicka",settings->oppSCollarDrainsMagicka);
		SetMCMBool("oppSCollarAutoRemove",settings->oppSCollarAutoRemove);
		SetMCMBool("oppDwarvenHeavyRestraint",settings->oppDwarvenHeavyRestraint);
		SetMCMBool("oppDwarvenRequireLoc",settings->oppDwarvenRequireLoc);
		SetMCMBool("oppLivingLatexGemAll",settings->oppLivingLatexGemAll);
		SetMCMBool("oppLivingLatexHeavy",settings->oppLivingLatexHeavy);
		SetMCMBool("oppLivingLatexRequireRem",settings->oppLivingLatexRequireRem);
		SetMCMBool("oppLivingLatexOpen",settings->oppLivingLatexOpen);
		SetMCMBool("oppMadnessOnlyDevices",settings->oppMadnessOnlyDevices);
		SetMCMBool("oppMadnessBeltFilter",settings->oppMadnessBeltFilter);
		SetMCMBool("oppMadnessChaos",settings->oppMadnessChaos);
		SetMCMBool("enableQuestInteractions",settings->enableQuestInteractions);
		SetMCMBool("enableQIMalkoran",settings->enableQIMalkoran);
		SetMCMBool("enableQISanguine",settings->enableQISanguine);
		SetMCMBool("enableQIBlackStar",settings->enableQIBlackStar);
		SetMCMBool("enableQIMindOfMadness",settings->enableQIMindOfMadness);
		SetMCMBool("enableQILaidToRest",settings->enableQILaidToRest);
		SetMCMBool("enableQICriedWolf",settings->enableQICriedWolf);
		SetMCMBool("enableQIDelivery",settings->enableQIDelivery);
		SetMCMBool("enableQIBarenziah",settings->enableQIBarenziah);
		SetMCMBool("enableQIProvingHonor",settings->enableQIProvingHonor);
		SetMCMBool("enableQISaarthal",settings->enableQISaarthal);
		SetMCMBool("enableQIBrelyna",settings->enableQIBrelyna);
		SetMCMBool("enableQIDwemerMuseum",settings->enableQIDwemerMuseum);
		SetMCMBool("enableQITrinityRestored",settings->enableQITrinityRestored);
		SetMCMBool("enableQIThalmorEmbassy",settings->enableQIThalmorEmbassy);
		SetMCMBool("enableQIRingmaker",settings->enableQIRingmaker);
		SetMCMBool("oppDeviceAntiCheat",settings->oppDeviceAntiCheat);
		SetMCMBool("generalDeviceAntiCheat",settings->generalDeviceAntiCheat);
		SetMCMBool("noMessageBoxes",settings->noMessageBoxes);
		SetMCMBool("eventVisualEffects",settings->eventVisualEffects);
		SetMCMBool("surpriseMode",settings->surpriseMode);
		SetMCMBool("dragonHoard",settings->dragonHoard);
		SetMCMBool("bossExtraGold",settings->bossExtraGold);
		SetMCMBool("useThemes",settings->useThemes);
		SetMCMBool("disableForce3rdPerson",settings->disableForce3rdPerson);
		SetMCMBool("enableSlowStrip",settings->enableSlowStrip);
		SetMCMBool("restrictFastTravel",settings->restrictFastTravel);
		SetMCMBool("restrictFastTravelPart",settings->restrictFastTravelPart);
		SetMCMBool("restrictFastTravelFull",settings->restrictFastTravelFull);
		SetMCMBool("useDeviceNamesInThemes",settings->useDeviceNamesInThemes);
		SetMCMBool("saveDependentSettings",settings->saveDependentSettings);
		SetMCMBool("ANDSexTopless",settings->ANDSexTopless);
		SetMCMBool("ANDSexBottomless",settings->ANDSexBottomless);
		SetMCMBool("ANDSexShowingChest",settings->ANDSexShowingChest);
		SetMCMBool("ANDSexShowingGenitals",settings->ANDSexShowingGenitals);
		SetMCMBool("ANDSexShowingAss",settings->ANDSexShowingAss);
		SetMCMBool("ANDSexShowingBra",settings->ANDSexShowingBra);
		SetMCMBool("ANDSexShowingUnderwear",settings->ANDSexShowingUnderwear);
		SetMCMBool("ANDConsTopless",settings->ANDConsTopless);
		SetMCMBool("ANDConsBottomless",settings->ANDConsBottomless);
		SetMCMBool("ANDConsShowingChest",settings->ANDConsShowingChest);
		SetMCMBool("ANDConsShowingGenitals",settings->ANDConsShowingGenitals);
		SetMCMBool("ANDConsShowingAss",settings->ANDConsShowingAss);
		SetMCMBool("ANDConsShowingBra",settings->ANDConsShowingBra);
		SetMCMBool("ANDConsShowingUnderwear",settings->ANDConsShowingUnderwear);
		SetMCMBool("consEnable",settings->consEnable);
		SetMCMBool("consGuardNormal",settings->consGuardNormal);
		SetMCMBool("consFineGuardFaction",settings->consFineGuardFaction);
		SetMCMBool("consFallthrough",settings->consFallthrough);
		SetMCMBool("consAllowFollowers",settings->consAllowFollowers);
		SetMCMBool("consFollowersBlock",settings->consFollowersBlock);
		SetMCMBool("consTolerableChastity",settings->consTolerableChastity);
		SetMCMBool("consUseRelationships",settings->consUseRelationships);
		SetMCMBool("consRelationBondage",settings->consRelationBondage);
		SetMCMBool("consRandomHeavyBondage",settings->consRandomHeavyBondage);
		SetMCMBool("consBondageIgnoreMax",settings->consBondageIgnoreMax);
		SetMCMBool("consTattooCurseNumbers",settings->consTattooCurseNumbers);
		SetMCMBool("sexEnabled",settings->sexEnabled);
		SetMCMBool("sexAggressiveAnims",settings->sexAggressiveAnims);
		SetMCMBool("sexFilterDevices",settings->sexFilterDevices);
		SetMCMBool("sexFilterFuta",settings->sexFilterFuta);
		SetMCMBool("sexRandomEnabled",settings->sexRandomEnabled);
		SetMCMBool("sexRandAllowFollowers",settings->sexRandAllowFollowers);
		SetMCMBool("sexRandAllowNPCs",settings->sexRandAllowNPCs);
		SetMCMBool("sexRandAllowSummons",settings->sexRandAllowSummons);
		SetMCMBool("sexRandAllowCreatures",settings->sexRandAllowCreatures);
		SetMCMBool("sexAllowMale",settings->sexAllowMale);
		SetMCMBool("sexAllowFemale",settings->sexAllowFemale);
		SetMCMBool("sexAllowFuta",settings->sexAllowFuta);
		SetMCMBool("sexAllowCreature",settings->sexAllowCreature);
		SetMCMBool("sexAllowFarmAnimals",settings->sexAllowFarmAnimals);
		SetMCMBool("sexRequireAllPlayer",settings->sexRequireAllPlayer);
		SetMCMBool("sexRequirePlayerCollar",settings->sexRequirePlayerCollar);
		SetMCMBool("sexRequirePlayerHeavy",settings->sexRequirePlayerHeavy);
		SetMCMBool("sexRequirePlayerNude",settings->sexRequirePlayerNude);
		SetMCMBool("sexRequireAllNPC",settings->sexRequireAllNPC);
		SetMCMBool("sexRequireNPCCollar",settings->sexRequireNPCCollar);
		SetMCMBool("sexRequireNPCNude",settings->sexRequireNPCNude);
		SetMCMBool("sexRequireBoth",settings->sexRequireBoth);
		SetMCMBool("sexFollowerBypassPlayer",settings->sexFollowerBypassPlayer);
		SetMCMBool("sexFollowerBypassNPC",settings->sexFollowerBypassNPC);
		SetMCMBool("sexSpouseBypassPlayer",settings->sexSpouseBypassPlayer);
		SetMCMBool("sexSpouseBypassNPC",settings->sexSpouseBypassNPC);
		SetMCMBool("sexSummonBypassPlayer",settings->sexSummonBypassPlayer);
		SetMCMBool("sexSummonBypassNPC",settings->sexSummonBypassNPC);
		SetMCMBool("ModSuspended",settings->ModSuspended);
		SetMCMBool("setAllDefaultSettings",settings->setAllDefaultSettings);
		SetMCMBool("helpSlaveTatsNGInstalled",settings->helpSlaveTatsNGInstalled);
		SetMCMBool("helpLewdMarksInstalled",settings->helpLewdMarksInstalled);
		SetMCMBool("helpRapeTattoosInstalled",settings->helpRapeTattoosInstalled);
		SetMCMBool("helpOSLArousedInstalled",settings->helpOSLArousedInstalled);
		SetMCMBool("helpSLOArousedInstalled",settings->helpSLOArousedInstalled);
		SetMCMBool("helpUDInstalled",settings->helpUDInstalled);
		SetMCMBool("helpQuickLootInstalled",settings->helpQuickLootInstalled);
		SetMCMBool("helpDWDInstalled",settings->helpDWDInstalled);
		SetMCMBool("helpANDInstalled",settings->helpANDInstalled);
		SetMCMBool("helpCockCagesWorking",settings->helpCockCagesWorking);
		SetMCMString("excludedFollowers",settings->excludedFollowers);
		SetMCMString("followerOverrideTheme",settings->followerOverrideTheme);
		SetMCMString("followerSkipKeywords",settings->followerSkipKeywords);
		SetMCMString("LMNudityAditionalForms",settings->LMNudityAditionalForms);
		//CODEGEN_END_PUSHMCM

		log::trace("Updated MCM settings");
	}

	void P_UpdateSKSE(RE::StaticFunctionTag*) {
		auto settings = Settings::GetSingleton();

		//CODEGEN_START_UPDATE
		bool recalculate = false;
		if (settings->maxHeldKeys != MCMSetting("maxHeldKeys")->GetSInt()) {recalculate = true;}
		if (settings->useGenderedChastity != MCMSetting("useGenderedChastity")->GetBool()) {recalculate = true;}
		if (settings->onlyUseUnforgivingDevices != MCMSetting("onlyUseUnforgivingDevices")->GetBool()) {recalculate = true;}
		if (settings->udUseAbadon != MCMSetting("udUseAbadon")->GetBool()) {recalculate = true;}
		if (settings->udUseAbadonSets != MCMSetting("udUseAbadonSets")->GetBool()) {recalculate = true;}
		if (settings->udUseMisc != MCMSetting("udUseMisc")->GetBool()) {recalculate = true;}
		settings->minGoldRequired = MCMSetting("minGoldRequired")->GetSInt();
		if (settings->minGoldRequired < 0) {settings->minGoldRequired = 0;}
		settings->eventScalingMod = MCMSetting("eventScalingMod")->GetSInt();
		if (settings->eventScalingMod < 0) {settings->eventScalingMod = 0;}
		settings->minRestraints = MCMSetting("minRestraints")->GetSInt();
		if (settings->minRestraints < 0) {settings->minRestraints = 0;}
		settings->maxRestraints = MCMSetting("maxRestraints")->GetSInt();
		if (settings->maxRestraints < 0) {settings->maxRestraints = 0;}
		settings->bossAditionalRestraints = MCMSetting("bossAditionalRestraints")->GetSInt();
		if (settings->bossAditionalRestraints < 0) {settings->bossAditionalRestraints = 0;}
		settings->restraintCap = MCMSetting("restraintCap")->GetSInt();
		if (settings->restraintCap < 0) {settings->restraintCap = 0;}
		settings->minArousal = MCMSetting("minArousal")->GetSInt();
		if (settings->minArousal < 0) {settings->minArousal = 0;}
		settings->beltWeight = MCMSetting("beltWeight")->GetSInt();
		if (settings->beltWeight < 0) {settings->beltWeight = 0;}
		settings->chastityPiercingWeight = MCMSetting("chastityPiercingWeight")->GetSInt();
		if (settings->chastityPiercingWeight < 0) {settings->chastityPiercingWeight = 0;}
		settings->braWeight = MCMSetting("braWeight")->GetSInt();
		if (settings->braWeight < 0) {settings->braWeight = 0;}
		settings->plugsWeight = MCMSetting("plugsWeight")->GetSInt();
		if (settings->plugsWeight < 0) {settings->plugsWeight = 0;}
		settings->lockingPlugsWeight = MCMSetting("lockingPlugsWeight")->GetSInt();
		if (settings->lockingPlugsWeight < 0) {settings->lockingPlugsWeight = 0;}
		settings->inflatablePlugsWeight = MCMSetting("inflatablePlugsWeight")->GetSInt();
		if (settings->inflatablePlugsWeight < 0) {settings->inflatablePlugsWeight = 0;}
		settings->nipplePiercingsWeight = MCMSetting("nipplePiercingsWeight")->GetSInt();
		if (settings->nipplePiercingsWeight < 0) {settings->nipplePiercingsWeight = 0;}
		settings->vaginalPiercingsWeight = MCMSetting("vaginalPiercingsWeight")->GetSInt();
		if (settings->vaginalPiercingsWeight < 0) {settings->vaginalPiercingsWeight = 0;}
		settings->corsetWeight = MCMSetting("corsetWeight")->GetSInt();
		if (settings->corsetWeight < 0) {settings->corsetWeight = 0;}
		settings->beltedCorsetsWeight = MCMSetting("beltedCorsetsWeight")->GetSInt();
		if (settings->beltedCorsetsWeight < 0) {settings->beltedCorsetsWeight = 0;}
		settings->slaveHarnessWeight = MCMSetting("slaveHarnessWeight")->GetSInt();
		if (settings->slaveHarnessWeight < 0) {settings->slaveHarnessWeight = 0;}
		settings->chastityHarnessWeight = MCMSetting("chastityHarnessWeight")->GetSInt();
		if (settings->chastityHarnessWeight < 0) {settings->chastityHarnessWeight = 0;}
		settings->armbinderWeight = MCMSetting("armbinderWeight")->GetSInt();
		if (settings->armbinderWeight < 0) {settings->armbinderWeight = 0;}
		settings->elbowbinderWeight = MCMSetting("elbowbinderWeight")->GetSInt();
		if (settings->elbowbinderWeight < 0) {settings->elbowbinderWeight = 0;}
		settings->yokeWeight = MCMSetting("yokeWeight")->GetSInt();
		if (settings->yokeWeight < 0) {settings->yokeWeight = 0;}
		settings->shacklesWeight = MCMSetting("shacklesWeight")->GetSInt();
		if (settings->shacklesWeight < 0) {settings->shacklesWeight = 0;}
		settings->straitjacketWeight = MCMSetting("straitjacketWeight")->GetSInt();
		if (settings->straitjacketWeight < 0) {settings->straitjacketWeight = 0;}
		settings->straitjacketLegbinderWeight = MCMSetting("straitjacketLegbinderWeight")->GetSInt();
		if (settings->straitjacketLegbinderWeight < 0) {settings->straitjacketLegbinderWeight = 0;}
		settings->petSuitWeight = MCMSetting("petSuitWeight")->GetSInt();
		if (settings->petSuitWeight < 0) {settings->petSuitWeight = 0;}
		settings->collarWeight = MCMSetting("collarWeight")->GetSInt();
		if (settings->collarWeight < 0) {settings->collarWeight = 0;}
		settings->armCuffsWeight = MCMSetting("armCuffsWeight")->GetSInt();
		if (settings->armCuffsWeight < 0) {settings->armCuffsWeight = 0;}
		settings->legCuffsWeight = MCMSetting("legCuffsWeight")->GetSInt();
		if (settings->legCuffsWeight < 0) {settings->legCuffsWeight = 0;}
		settings->gagWeight = MCMSetting("gagWeight")->GetSInt();
		if (settings->gagWeight < 0) {settings->gagWeight = 0;}
		settings->ringGagWeight = MCMSetting("ringGagWeight")->GetSInt();
		if (settings->ringGagWeight < 0) {settings->ringGagWeight = 0;}
		settings->largeGagWeight = MCMSetting("largeGagWeight")->GetSInt();
		if (settings->largeGagWeight < 0) {settings->largeGagWeight = 0;}
		settings->largeRingGagWeight = MCMSetting("largeRingGagWeight")->GetSInt();
		if (settings->largeRingGagWeight < 0) {settings->largeRingGagWeight = 0;}
		settings->blindfoldWeight = MCMSetting("blindfoldWeight")->GetSInt();
		if (settings->blindfoldWeight < 0) {settings->blindfoldWeight = 0;}
		settings->hoodBothWeight = MCMSetting("hoodBothWeight")->GetSInt();
		if (settings->hoodBothWeight < 0) {settings->hoodBothWeight = 0;}
		settings->hoodGagWeight = MCMSetting("hoodGagWeight")->GetSInt();
		if (settings->hoodGagWeight < 0) {settings->hoodGagWeight = 0;}
		settings->hoodBlindWeight = MCMSetting("hoodBlindWeight")->GetSInt();
		if (settings->hoodBlindWeight < 0) {settings->hoodBlindWeight = 0;}
		settings->hoodNoneWeight = MCMSetting("hoodNoneWeight")->GetSInt();
		if (settings->hoodNoneWeight < 0) {settings->hoodNoneWeight = 0;}
		settings->catsuitWeight = MCMSetting("catsuitWeight")->GetSInt();
		if (settings->catsuitWeight < 0) {settings->catsuitWeight = 0;}
		settings->hobbleSkirtWeight = MCMSetting("hobbleSkirtWeight")->GetSInt();
		if (settings->hobbleSkirtWeight < 0) {settings->hobbleSkirtWeight = 0;}
		settings->hobbleSkirtDifficulty = MCMSetting("hobbleSkirtDifficulty")->GetSInt();
		if (settings->hobbleSkirtDifficulty < 0) {settings->hobbleSkirtDifficulty = 0;}
		settings->bootsWeight = MCMSetting("bootsWeight")->GetSInt();
		if (settings->bootsWeight < 0) {settings->bootsWeight = 0;}
		settings->glovesWeight = MCMSetting("glovesWeight")->GetSInt();
		if (settings->glovesWeight < 0) {settings->glovesWeight = 0;}
		settings->mittensWeight = MCMSetting("mittensWeight")->GetSInt();
		if (settings->mittensWeight < 0) {settings->mittensWeight = 0;}
		settings->minKeysLooted = MCMSetting("minKeysLooted")->GetSInt();
		if (settings->minKeysLooted < 0) {settings->minKeysLooted = 0;}
		settings->maxKeysLooted = MCMSetting("maxKeysLooted")->GetSInt();
		if (settings->maxKeysLooted < 0) {settings->maxKeysLooted = 0;}
		settings->maxHeldKeys = MCMSetting("maxHeldKeys")->GetSInt();
		if (settings->maxHeldKeys < 0) {settings->maxHeldKeys = 0;}
		settings->restraintsKeyWeight = MCMSetting("restraintsKeyWeight")->GetSInt();
		if (settings->restraintsKeyWeight < 0) {settings->restraintsKeyWeight = 0;}
		settings->chastityKeyWeight = MCMSetting("chastityKeyWeight")->GetSInt();
		if (settings->chastityKeyWeight < 0) {settings->chastityKeyWeight = 0;}
		settings->piercingToolWeight = MCMSetting("piercingToolWeight")->GetSInt();
		if (settings->piercingToolWeight < 0) {settings->piercingToolWeight = 0;}
		settings->eventStandardWeight = MCMSetting("eventStandardWeight")->GetSInt();
		if (settings->eventStandardWeight < 0) {settings->eventStandardWeight = 0;}
		settings->eventStandardBossReduction = MCMSetting("eventStandardBossReduction")->GetSInt();
		if (settings->eventStandardBossReduction < 0) {settings->eventStandardBossReduction = 0;}
		settings->eventOppressiveWeight = MCMSetting("eventOppressiveWeight")->GetSInt();
		if (settings->eventOppressiveWeight < 0) {settings->eventOppressiveWeight = 0;}
		settings->eventContraptionWeight = MCMSetting("eventContraptionWeight")->GetSInt();
		if (settings->eventContraptionWeight < 0) {settings->eventContraptionWeight = 0;}
		settings->eventContDeviceOverride = MCMSetting("eventContDeviceOverride")->GetSInt();
		if (settings->eventContDeviceOverride < 0) {settings->eventContDeviceOverride = 0;}
		settings->eventWickedWeight = MCMSetting("eventWickedWeight")->GetSInt();
		if (settings->eventWickedWeight < 0) {settings->eventWickedWeight = 0;}
		settings->eventTattooWeight = MCMSetting("eventTattooWeight")->GetSInt();
		if (settings->eventTattooWeight < 0) {settings->eventTattooWeight = 0;}
		settings->eventTattooMin = MCMSetting("eventTattooMin")->GetSInt();
		if (settings->eventTattooMin < 0) {settings->eventTattooMin = 0;}
		settings->eventTattooMax = MCMSetting("eventTattooMax")->GetSInt();
		if (settings->eventTattooMax < 0) {settings->eventTattooMax = 0;}
		settings->eventTattooCap = MCMSetting("eventTattooCap")->GetSInt();
		if (settings->eventTattooCap < 0) {settings->eventTattooCap = 0;}
		settings->eventLewdMarkWeight = MCMSetting("eventLewdMarkWeight")->GetSInt();
		if (settings->eventLewdMarkWeight < 0) {settings->eventLewdMarkWeight = 0;}
		settings->eventSimpleSlaveryWeight = MCMSetting("eventSimpleSlaveryWeight")->GetSInt();
		if (settings->eventSimpleSlaveryWeight < 0) {settings->eventSimpleSlaveryWeight = 0;}
		settings->eventSSMinRestraints = MCMSetting("eventSSMinRestraints")->GetSInt();
		if (settings->eventSSMinRestraints < 0) {settings->eventSSMinRestraints = 0;}
		settings->eventAbadonWeight = MCMSetting("eventAbadonWeight")->GetSInt();
		if (settings->eventAbadonWeight < 0) {settings->eventAbadonWeight = 0;}
		settings->eventAbadonWarriorWeight = MCMSetting("eventAbadonWarriorWeight")->GetSInt();
		if (settings->eventAbadonWarriorWeight < 0) {settings->eventAbadonWarriorWeight = 0;}
		settings->eventAbadonWarriorCount = MCMSetting("eventAbadonWarriorCount")->GetSInt();
		if (settings->eventAbadonWarriorCount < 0) {settings->eventAbadonWarriorCount = 0;}
		settings->eventAbadonScoutWeight = MCMSetting("eventAbadonScoutWeight")->GetSInt();
		if (settings->eventAbadonScoutWeight < 0) {settings->eventAbadonScoutWeight = 0;}
		settings->eventAbadonScoutCount = MCMSetting("eventAbadonScoutCount")->GetSInt();
		if (settings->eventAbadonScoutCount < 0) {settings->eventAbadonScoutCount = 0;}
		settings->eventAbadonWitchWeight = MCMSetting("eventAbadonWitchWeight")->GetSInt();
		if (settings->eventAbadonWitchWeight < 0) {settings->eventAbadonWitchWeight = 0;}
		settings->eventAbadonWitchCount = MCMSetting("eventAbadonWitchCount")->GetSInt();
		if (settings->eventAbadonWitchCount < 0) {settings->eventAbadonWitchCount = 0;}
		settings->followerDeviceModifier = MCMSetting("followerDeviceModifier")->GetSInt();
		settings->LMAllureWeight = MCMSetting("LMAllureWeight")->GetSInt();
		if (settings->LMAllureWeight < 0) {settings->LMAllureWeight = 0;}
		settings->LMAllureMod = MCMSetting("LMAllureMod")->GetSInt();
		if (settings->LMAllureMod < 0) {settings->LMAllureMod = 0;}
		settings->LMAllureSex = MCMSetting("LMAllureSex")->GetSInt();
		if (settings->LMAllureSex < 0) {settings->LMAllureSex = 0;}
		settings->LMHeatWeight = MCMSetting("LMHeatWeight")->GetSInt();
		if (settings->LMHeatWeight < 0) {settings->LMHeatWeight = 0;}
		settings->LMHeatMod = MCMSetting("LMHeatMod")->GetSInt();
		if (settings->LMHeatMod < 0) {settings->LMHeatMod = 0;}
		settings->LMHeatContainerCount = MCMSetting("LMHeatContainerCount")->GetSInt();
		if (settings->LMHeatContainerCount < 0) {settings->LMHeatContainerCount = 0;}
		settings->LMBrandingWeight = MCMSetting("LMBrandingWeight")->GetSInt();
		if (settings->LMBrandingWeight < 0) {settings->LMBrandingWeight = 0;}
		settings->LMBrandingTotal = MCMSetting("LMBrandingTotal")->GetSInt();
		if (settings->LMBrandingTotal < 0) {settings->LMBrandingTotal = 0;}
		settings->LMHealslutWeight = MCMSetting("LMHealslutWeight")->GetSInt();
		if (settings->LMHealslutWeight < 0) {settings->LMHealslutWeight = 0;}
		settings->LMHealslutHealing = MCMSetting("LMHealslutHealing")->GetSInt();
		if (settings->LMHealslutHealing < 0) {settings->LMHealslutHealing = 0;}
		settings->LMHealslutArousal = MCMSetting("LMHealslutArousal")->GetSInt();
		if (settings->LMHealslutArousal < 0) {settings->LMHealslutArousal = 0;}
		settings->LMHealslutDuration = MCMSetting("LMHealslutDuration")->GetSInt();
		if (settings->LMHealslutDuration < 0) {settings->LMHealslutDuration = 0;}
		settings->LMBondageWeight = MCMSetting("LMBondageWeight")->GetSInt();
		if (settings->LMBondageWeight < 0) {settings->LMBondageWeight = 0;}
		settings->LMBondageDeviceCapAdd = MCMSetting("LMBondageDeviceCapAdd")->GetSInt();
		if (settings->LMBondageDeviceCapAdd < 0) {settings->LMBondageDeviceCapAdd = 0;}
		settings->LMBondageDeviceCount = MCMSetting("LMBondageDeviceCount")->GetSInt();
		if (settings->LMBondageDeviceCount < 0) {settings->LMBondageDeviceCount = 0;}
		settings->LMNudityWeight = MCMSetting("LMNudityWeight")->GetSInt();
		if (settings->LMNudityWeight < 0) {settings->LMNudityWeight = 0;}
		settings->LMNudityTalkTimes = MCMSetting("LMNudityTalkTimes")->GetSInt();
		if (settings->LMNudityTalkTimes < 0) {settings->LMNudityTalkTimes = 0;}
		settings->LMPainslutWeight = MCMSetting("LMPainslutWeight")->GetSInt();
		if (settings->LMPainslutWeight < 0) {settings->LMPainslutWeight = 0;}
		settings->LMPainslutMod = MCMSetting("LMPainslutMod")->GetSInt();
		if (settings->LMPainslutMod < 0) {settings->LMPainslutMod = 0;}
		settings->LMPainslutCount = MCMSetting("LMPainslutCount")->GetSInt();
		if (settings->LMPainslutCount < 0) {settings->LMPainslutCount = 0;}
		settings->LMPainslutArousal = MCMSetting("LMPainslutArousal")->GetSInt();
		if (settings->LMPainslutArousal < 0) {settings->LMPainslutArousal = 0;}
		settings->oppSummonerCollarWeight = MCMSetting("oppSummonerCollarWeight")->GetSInt();
		if (settings->oppSummonerCollarWeight < 0) {settings->oppSummonerCollarWeight = 0;}
		settings->oppSummonerSexCount = MCMSetting("oppSummonerSexCount")->GetSInt();
		if (settings->oppSummonerSexCount < 0) {settings->oppSummonerSexCount = 0;}
		settings->oppDwarvenCuirassWeight = MCMSetting("oppDwarvenCuirassWeight")->GetSInt();
		if (settings->oppDwarvenCuirassWeight < 0) {settings->oppDwarvenCuirassWeight = 0;}
		settings->oppDwarvenValueNeeded = MCMSetting("oppDwarvenValueNeeded")->GetSInt();
		if (settings->oppDwarvenValueNeeded < 0) {settings->oppDwarvenValueNeeded = 0;}
		settings->oppDwarvenArousal = MCMSetting("oppDwarvenArousal")->GetSInt();
		if (settings->oppDwarvenArousal < 0) {settings->oppDwarvenArousal = 0;}
		settings->oppNocturnalWeight = MCMSetting("oppNocturnalWeight")->GetSInt();
		if (settings->oppNocturnalWeight < 0) {settings->oppNocturnalWeight = 0;}
		settings->oppNocturnalSexCount = MCMSetting("oppNocturnalSexCount")->GetSInt();
		if (settings->oppNocturnalSexCount < 0) {settings->oppNocturnalSexCount = 0;}
		settings->oppNocturnalPeople = MCMSetting("oppNocturnalPeople")->GetSInt();
		if (settings->oppNocturnalPeople < 0) {settings->oppNocturnalPeople = 0;}
		settings->oppNocturnalArousal = MCMSetting("oppNocturnalArousal")->GetSInt();
		if (settings->oppNocturnalArousal < 0) {settings->oppNocturnalArousal = 0;}
		settings->oppLivingLatexWeight = MCMSetting("oppLivingLatexWeight")->GetSInt();
		if (settings->oppLivingLatexWeight < 0) {settings->oppLivingLatexWeight = 0;}
		settings->oppLivingLatexStartTime = MCMSetting("oppLivingLatexStartTime")->GetSInt();
		if (settings->oppLivingLatexStartTime < 0) {settings->oppLivingLatexStartTime = 0;}
		settings->oppMadnessPlugWeight = MCMSetting("oppMadnessPlugWeight")->GetSInt();
		if (settings->oppMadnessPlugWeight < 0) {settings->oppMadnessPlugWeight = 0;}
		settings->oppMadnessPlugIterations = MCMSetting("oppMadnessPlugIterations")->GetSInt();
		if (settings->oppMadnessPlugIterations < 0) {settings->oppMadnessPlugIterations = 0;}
		settings->oppMadnessplugOrgasms = MCMSetting("oppMadnessplugOrgasms")->GetSInt();
		if (settings->oppMadnessplugOrgasms < 0) {settings->oppMadnessplugOrgasms = 0;}
		settings->consScanRadius = MCMSetting("consScanRadius")->GetSInt();
		if (settings->consScanRadius < 0) {settings->consScanRadius = 0;}
		settings->consScanInterval = MCMSetting("consScanInterval")->GetSInt();
		if (settings->consScanInterval < 0) {settings->consScanInterval = 0;}
		settings->consFineForSex = MCMSetting("consFineForSex")->GetSInt();
		if (settings->consFineForSex < 0) {settings->consFineForSex = 0;}
		settings->consFineForNudity = MCMSetting("consFineForNudity")->GetSInt();
		if (settings->consFineForNudity < 0) {settings->consFineForNudity = 0;}
		settings->consFineForRestraints = MCMSetting("consFineForRestraints")->GetSInt();
		if (settings->consFineForRestraints < 0) {settings->consFineForRestraints = 0;}
		settings->consFineForTattoos = MCMSetting("consFineForTattoos")->GetSInt();
		if (settings->consFineForTattoos < 0) {settings->consFineForTattoos = 0;}
		settings->consRestrainedCount = MCMSetting("consRestrainedCount")->GetSInt();
		if (settings->consRestrainedCount < 0) {settings->consRestrainedCount = 0;}
		settings->consTattooCount = MCMSetting("consTattooCount")->GetSInt();
		if (settings->consTattooCount < 0) {settings->consTattooCount = 0;}
		settings->consSexWeight = MCMSetting("consSexWeight")->GetSInt();
		if (settings->consSexWeight < 0) {settings->consSexWeight = 0;}
		settings->consRandomBondageWeight = MCMSetting("consRandomBondageWeight")->GetSInt();
		if (settings->consRandomBondageWeight < 0) {settings->consRandomBondageWeight = 0;}
		settings->consTattooWeight = MCMSetting("consTattooWeight")->GetSInt();
		if (settings->consTattooWeight < 0) {settings->consTattooWeight = 0;}
		settings->consMercyWeight = MCMSetting("consMercyWeight")->GetSInt();
		if (settings->consMercyWeight < 0) {settings->consMercyWeight = 0;}
		settings->sexCooldown = MCMSetting("sexCooldown")->GetSInt();
		if (settings->sexCooldown < 0) {settings->sexCooldown = 0;}
		settings->sexArousalModifierCutoff = MCMSetting("sexArousalModifierCutoff")->GetSInt();
		if (settings->sexArousalModifierCutoff < 0) {settings->sexArousalModifierCutoff = 0;}
		settings->sexScalingModifier = MCMSetting("sexScalingModifier")->GetSInt();
		if (settings->sexScalingModifier < 0) {settings->sexScalingModifier = 0;}
		settings->sexSearchRadius = MCMSetting("sexSearchRadius")->GetSInt();
		if (settings->sexSearchRadius < 0) {settings->sexSearchRadius = 0;}
		settings->sexSearchInterval = MCMSetting("sexSearchInterval")->GetSInt();
		if (settings->sexSearchInterval < 0) {settings->sexSearchInterval = 0;}
		settings->sexRequiredPlayerBindings = MCMSetting("sexRequiredPlayerBindings")->GetSInt();
		settings->sexRequiredPlayerArousal = MCMSetting("sexRequiredPlayerArousal")->GetSInt();
		settings->sexRequiredPlayerTattoos = MCMSetting("sexRequiredPlayerTattoos")->GetSInt();
		settings->sexRequiredNPCBindings = MCMSetting("sexRequiredNPCBindings")->GetSInt();
		settings->sexRequiredNPCArousal = MCMSetting("sexRequiredNPCArousal")->GetSInt();
		settings->sexRequiredNPCTattoos = MCMSetting("sexRequiredNPCTattoos")->GetSInt();
		settings->sexRequiredCreatureArousal = MCMSetting("sexRequiredCreatureArousal")->GetSInt();
		settings->LMAllureColor = MCMSetting("LMAllureColor")->GetSInt();
		settings->LMHeatColor = MCMSetting("LMHeatColor")->GetSInt();
		settings->LMBrandingColor = MCMSetting("LMBrandingColor")->GetSInt();
		settings->LMHealslutColor = MCMSetting("LMHealslutColor")->GetSInt();
		settings->LMBondageColor = MCMSetting("LMBondageColor")->GetSInt();
		settings->LMNudityColor = MCMSetting("LMNudityColor")->GetSInt();
		settings->LMPainslutColor = MCMSetting("LMPainslutColor")->GetSInt();
		settings->setDebugKey = MCMSetting("setDebugKey")->GetSInt();
		settings->oppSummonerNPC = MCMSetting("oppSummonerNPC")->GetSInt();
		settings->baseChance = MCMSetting("baseChance")->GetFloat();
		if (settings->baseChance < 0) {settings->baseChance = 0;}
		settings->containerModifier = MCMSetting("containerModifier")->GetFloat();
		if (settings->containerModifier < 0) {settings->containerModifier = 0;}
		settings->bossContainerModifier = MCMSetting("bossContainerModifier")->GetFloat();
		if (settings->bossContainerModifier < 0) {settings->bossContainerModifier = 0;}
		settings->deadBodyModifier = MCMSetting("deadBodyModifier")->GetFloat();
		if (settings->deadBodyModifier < 0) {settings->deadBodyModifier = 0;}
		settings->pickpocketModifier = MCMSetting("pickpocketModifier")->GetFloat();
		if (settings->pickpocketModifier < 0) {settings->pickpocketModifier = 0;}
		settings->doorModifier = MCMSetting("doorModifier")->GetFloat();
		if (settings->doorModifier < 0) {settings->doorModifier = 0;}
		settings->lockedModifier = MCMSetting("lockedModifier")->GetFloat();
		if (settings->lockedModifier < 0) {settings->lockedModifier = 0;}
		settings->lockDifficultyModifier = MCMSetting("lockDifficultyModifier")->GetFloat();
		if (settings->lockDifficultyModifier < 0) {settings->lockDifficultyModifier = 0;}
		settings->arousalModifier = MCMSetting("arousalModifier")->GetFloat();
		if (settings->arousalModifier < 0) {settings->arousalModifier = 0;}
		settings->arousalHalfLife = MCMSetting("arousalHalfLife")->GetFloat();
		if (settings->arousalHalfLife < 0) {settings->arousalHalfLife = 0;}
		settings->keyLossChance = MCMSetting("keyLossChance")->GetFloat();
		if (settings->keyLossChance < 0) {settings->keyLossChance = 0;}
		settings->keyChance = MCMSetting("keyChance")->GetFloat();
		if (settings->keyChance < 0) {settings->keyChance = 0;}
		settings->keyBonus = MCMSetting("keyBonus")->GetFloat();
		if (settings->keyBonus < 0) {settings->keyBonus = 0;}
		settings->keyPickpocketBonus = MCMSetting("keyPickpocketBonus")->GetFloat();
		if (settings->keyPickpocketBonus < 0) {settings->keyPickpocketBonus = 0;}
		settings->magicKeyChance = MCMSetting("magicKeyChance")->GetFloat();
		if (settings->magicKeyChance < 0) {settings->magicKeyChance = 0;}
		settings->keyLuckyBonus = MCMSetting("keyLuckyBonus")->GetFloat();
		if (settings->keyLuckyBonus < 0) {settings->keyLuckyBonus = 0;}
		settings->eventContraptionTime = MCMSetting("eventContraptionTime")->GetFloat();
		if (settings->eventContraptionTime < 0) {settings->eventContraptionTime = 0;}
		settings->followerDeviceMult = MCMSetting("followerDeviceMult")->GetFloat();
		if (settings->followerDeviceMult < 0) {settings->followerDeviceMult = 0;}
		settings->LMBrandingChance = MCMSetting("LMBrandingChance")->GetFloat();
		if (settings->LMBrandingChance < 0) {settings->LMBrandingChance = 0;}
		settings->LMBondageModifier = MCMSetting("LMBondageModifier")->GetFloat();
		if (settings->LMBondageModifier < 0) {settings->LMBondageModifier = 0;}
		settings->LMPainslutSexChance = MCMSetting("LMPainslutSexChance")->GetFloat();
		if (settings->LMPainslutSexChance < 0) {settings->LMPainslutSexChance = 0;}
		settings->oppSummonChance = MCMSetting("oppSummonChance")->GetFloat();
		if (settings->oppSummonChance < 0) {settings->oppSummonChance = 0;}
		settings->oppSummonSexChance = MCMSetting("oppSummonSexChance")->GetFloat();
		if (settings->oppSummonSexChance < 0) {settings->oppSummonSexChance = 0;}
		settings->oppNocturnalHandChance = MCMSetting("oppNocturnalHandChance")->GetFloat();
		if (settings->oppNocturnalHandChance < 0) {settings->oppNocturnalHandChance = 0;}
		settings->oppNocturnalRecastChance = MCMSetting("oppNocturnalRecastChance")->GetFloat();
		if (settings->oppNocturnalRecastChance < 0) {settings->oppNocturnalRecastChance = 0;}
		settings->oppLivingLatexGem = MCMSetting("oppLivingLatexGem")->GetFloat();
		if (settings->oppLivingLatexGem < 0) {settings->oppLivingLatexGem = 0;}
		settings->oppLivingLatexMore = MCMSetting("oppLivingLatexMore")->GetFloat();
		if (settings->oppLivingLatexMore < 0) {settings->oppLivingLatexMore = 0;}
		settings->oppMadnessMultiplier = MCMSetting("oppMadnessMultiplier")->GetFloat();
		if (settings->oppMadnessMultiplier < 0) {settings->oppMadnessMultiplier = 0;}
		settings->lockedLocationBypass = MCMSetting("lockedLocationBypass")->GetFloat();
		if (settings->lockedLocationBypass < 0) {settings->lockedLocationBypass = 0;}
		settings->theftLocationBypass = MCMSetting("theftLocationBypass")->GetFloat();
		if (settings->theftLocationBypass < 0) {settings->theftLocationBypass = 0;}
		settings->playerHomeModifier = MCMSetting("playerHomeModifier")->GetFloat();
		if (settings->playerHomeModifier < 0) {settings->playerHomeModifier = 0;}
		settings->cityModifier = MCMSetting("cityModifier")->GetFloat();
		if (settings->cityModifier < 0) {settings->cityModifier = 0;}
		settings->townModifier = MCMSetting("townModifier")->GetFloat();
		if (settings->townModifier < 0) {settings->townModifier = 0;}
		settings->banditModifier = MCMSetting("banditModifier")->GetFloat();
		if (settings->banditModifier < 0) {settings->banditModifier = 0;}
		settings->draugrModifier = MCMSetting("draugrModifier")->GetFloat();
		if (settings->draugrModifier < 0) {settings->draugrModifier = 0;}
		settings->soulCairnModifier = MCMSetting("soulCairnModifier")->GetFloat();
		if (settings->soulCairnModifier < 0) {settings->soulCairnModifier = 0;}
		settings->dwarvenModifier = MCMSetting("dwarvenModifier")->GetFloat();
		if (settings->dwarvenModifier < 0) {settings->dwarvenModifier = 0;}
		settings->falmerModifier = MCMSetting("falmerModifier")->GetFloat();
		if (settings->falmerModifier < 0) {settings->falmerModifier = 0;}
		settings->forswornModifier = MCMSetting("forswornModifier")->GetFloat();
		if (settings->forswornModifier < 0) {settings->forswornModifier = 0;}
		settings->vampireModifier = MCMSetting("vampireModifier")->GetFloat();
		if (settings->vampireModifier < 0) {settings->vampireModifier = 0;}
		settings->warlockModifier = MCMSetting("warlockModifier")->GetFloat();
		if (settings->warlockModifier < 0) {settings->warlockModifier = 0;}
		settings->dragonLairModifier = MCMSetting("dragonLairModifier")->GetFloat();
		if (settings->dragonLairModifier < 0) {settings->dragonLairModifier = 0;}
		settings->apocryphaModifier = MCMSetting("apocryphaModifier")->GetFloat();
		if (settings->apocryphaModifier < 0) {settings->apocryphaModifier = 0;}
		settings->wildernessModifier = MCMSetting("wildernessModifier")->GetFloat();
		if (settings->wildernessModifier < 0) {settings->wildernessModifier = 0;}
		settings->rDeviceBaseChance = MCMSetting("rDeviceBaseChance")->GetFloat();
		if (settings->rDeviceBaseChance < 0) {settings->rDeviceBaseChance = 0;}
		settings->rDeviceLuckyBonus = MCMSetting("rDeviceLuckyBonus")->GetFloat();
		if (settings->rDeviceLuckyBonus < 0) {settings->rDeviceLuckyBonus = 0;}
		settings->wardingCharmChance = MCMSetting("wardingCharmChance")->GetFloat();
		if (settings->wardingCharmChance < 0) {settings->wardingCharmChance = 0;}
		settings->tatSolventChance = MCMSetting("tatSolventChance")->GetFloat();
		if (settings->tatSolventChance < 0) {settings->tatSolventChance = 0;}
		settings->arousalPotionChance = MCMSetting("arousalPotionChance")->GetFloat();
		if (settings->arousalPotionChance < 0) {settings->arousalPotionChance = 0;}
		settings->arousalPotionLibido = MCMSetting("arousalPotionLibido")->GetFloat();
		if (settings->arousalPotionLibido < 0) {settings->arousalPotionLibido = 0;}
		settings->arousalPotionDuration = MCMSetting("arousalPotionDuration")->GetFloat();
		if (settings->arousalPotionDuration < 0) {settings->arousalPotionDuration = 0;}
		settings->consTalkNude = MCMSetting("consTalkNude")->GetFloat();
		if (settings->consTalkNude < 0) {settings->consTalkNude = 0;}
		settings->consTalkRestrained = MCMSetting("consTalkRestrained")->GetFloat();
		if (settings->consTalkRestrained < 0) {settings->consTalkRestrained = 0;}
		settings->consTalkHeavy = MCMSetting("consTalkHeavy")->GetFloat();
		if (settings->consTalkHeavy < 0) {settings->consTalkHeavy = 0;}
		settings->consTalkTattoo = MCMSetting("consTalkTattoo")->GetFloat();
		if (settings->consTalkTattoo < 0) {settings->consTalkTattoo = 0;}
		settings->consScanSex = MCMSetting("consScanSex")->GetFloat();
		if (settings->consScanSex < 0) {settings->consScanSex = 0;}
		settings->consScanNude = MCMSetting("consScanNude")->GetFloat();
		if (settings->consScanNude < 0) {settings->consScanNude = 0;}
		settings->consScanRestrained = MCMSetting("consScanRestrained")->GetFloat();
		if (settings->consScanRestrained < 0) {settings->consScanRestrained = 0;}
		settings->consScanHeavy = MCMSetting("consScanHeavy")->GetFloat();
		if (settings->consScanHeavy < 0) {settings->consScanHeavy = 0;}
		settings->consScanTattoo = MCMSetting("consScanTattoo")->GetFloat();
		if (settings->consScanTattoo < 0) {settings->consScanTattoo = 0;}
		settings->consScanTrespass = MCMSetting("consScanTrespass")->GetFloat();
		if (settings->consScanTrespass < 0) {settings->consScanTrespass = 0;}
		settings->consGuardScanSex = MCMSetting("consGuardScanSex")->GetFloat();
		if (settings->consGuardScanSex < 0) {settings->consGuardScanSex = 0;}
		settings->consGuardScanNude = MCMSetting("consGuardScanNude")->GetFloat();
		if (settings->consGuardScanNude < 0) {settings->consGuardScanNude = 0;}
		settings->consGuardScanRestrained = MCMSetting("consGuardScanRestrained")->GetFloat();
		if (settings->consGuardScanRestrained < 0) {settings->consGuardScanRestrained = 0;}
		settings->consGuardScanTattoo = MCMSetting("consGuardScanTattoo")->GetFloat();
		if (settings->consGuardScanTattoo < 0) {settings->consGuardScanTattoo = 0;}
		settings->consGuardTalkMult = MCMSetting("consGuardTalkMult")->GetFloat();
		if (settings->consGuardTalkMult < 0) {settings->consGuardTalkMult = 0;}
		settings->consGaggedMultiplier = MCMSetting("consGaggedMultiplier")->GetFloat();
		if (settings->consGaggedMultiplier < 0) {settings->consGaggedMultiplier = 0;}
		settings->sexBaseChance = MCMSetting("sexBaseChance")->GetFloat();
		if (settings->sexBaseChance < 0) {settings->sexBaseChance = 0;}
		settings->sexCreatureModifier = MCMSetting("sexCreatureModifier")->GetFloat();
		if (settings->sexCreatureModifier < 0) {settings->sexCreatureModifier = 0;}
		settings->sexFollowerModifier = MCMSetting("sexFollowerModifier")->GetFloat();
		if (settings->sexFollowerModifier < 0) {settings->sexFollowerModifier = 0;}
		settings->sexSummonModifier = MCMSetting("sexSummonModifier")->GetFloat();
		if (settings->sexSummonModifier < 0) {settings->sexSummonModifier = 0;}
		settings->sexRestraintsModifier = MCMSetting("sexRestraintsModifier")->GetFloat();
		if (settings->sexRestraintsModifier < 0) {settings->sexRestraintsModifier = 0;}
		settings->sexSafeLocationModifier = MCMSetting("sexSafeLocationModifier")->GetFloat();
		if (settings->sexSafeLocationModifier < 0) {settings->sexSafeLocationModifier = 0;}
		settings->sexUnsafeLocationModifier = MCMSetting("sexUnsafeLocationModifier")->GetFloat();
		if (settings->sexUnsafeLocationModifier < 0) {settings->sexUnsafeLocationModifier = 0;}
		settings->sexPlayerNudityModifier = MCMSetting("sexPlayerNudityModifier")->GetFloat();
		if (settings->sexPlayerNudityModifier < 0) {settings->sexPlayerNudityModifier = 0;}
		settings->sexNPCNudityModifier = MCMSetting("sexNPCNudityModifier")->GetFloat();
		if (settings->sexNPCNudityModifier < 0) {settings->sexNPCNudityModifier = 0;}
		settings->sexPlayerArousalModifier = MCMSetting("sexPlayerArousalModifier")->GetFloat();
		if (settings->sexPlayerArousalModifier < 0) {settings->sexPlayerArousalModifier = 0;}
		settings->sexNPCArousalModifier = MCMSetting("sexNPCArousalModifier")->GetFloat();
		if (settings->sexNPCArousalModifier < 0) {settings->sexNPCArousalModifier = 0;}
		settings->sexAllureModifier = MCMSetting("sexAllureModifier")->GetFloat();
		if (settings->sexAllureModifier < 0) {settings->sexAllureModifier = 0;}
		settings->sexSummonerModifier = MCMSetting("sexSummonerModifier")->GetFloat();
		if (settings->sexSummonerModifier < 0) {settings->sexSummonerModifier = 0;}
		settings->onlyLockedDoors = MCMSetting("onlyLockedDoors")->GetBool();
		settings->eventScaling = MCMSetting("eventScaling")->GetBool();
		settings->bossOnlyHeavy = MCMSetting("bossOnlyHeavy")->GetBool();
		settings->stripPlayerOnEvent = MCMSetting("stripPlayerOnEvent")->GetBool();
		settings->stripOnlyKeywords = MCMSetting("stripOnlyKeywords")->GetBool();
		settings->beltPlugs = MCMSetting("beltPlugs")->GetBool();
		settings->useGenderedChastity = MCMSetting("useGenderedChastity")->GetBool();
		settings->plugsDontCount = MCMSetting("plugsDontCount")->GetBool();
		settings->useGenderedPlugs = MCMSetting("useGenderedPlugs")->GetBool();
		settings->allowLegShackles = MCMSetting("allowLegShackles")->GetBool();
		settings->keyForgiveness = MCMSetting("keyForgiveness")->GetBool();
		settings->magicKeyOppressive = MCMSetting("magicKeyOppressive")->GetBool();
		settings->preferRelevantKeys = MCMSetting("preferRelevantKeys")->GetBool();
		settings->vanishingKeys = MCMSetting("vanishingKeys")->GetBool();
		settings->eventContDevices = MCMSetting("eventContDevices")->GetBool();
		settings->eventContAllDevices = MCMSetting("eventContAllDevices")->GetBool();
		settings->followerBondageEvent = MCMSetting("followerBondageEvent")->GetBool();
		settings->followerTattooEvent = MCMSetting("followerTattooEvent")->GetBool();
		settings->onlyFemaleFollowers = MCMSetting("onlyFemaleFollowers")->GetBool();
		settings->followerHeavyRestraints = MCMSetting("followerHeavyRestraints")->GetBool();
		settings->followerGags = MCMSetting("followerGags")->GetBool();
		settings->onlyUseUnforgivingDevices = MCMSetting("onlyUseUnforgivingDevices")->GetBool();
		settings->udUseAbadon = MCMSetting("udUseAbadon")->GetBool();
		settings->udUseAbadonSets = MCMSetting("udUseAbadonSets")->GetBool();
		settings->udUseMisc = MCMSetting("udUseMisc")->GetBool();
		settings->LMAllureSkipRequirements = MCMSetting("LMAllureSkipRequirements")->GetBool();
		settings->LMBrandingPunish = MCMSetting("LMBrandingPunish")->GetBool();
		settings->LMNudityChestOnly = MCMSetting("LMNudityChestOnly")->GetBool();
		settings->LMNuditySkipGagged = MCMSetting("LMNuditySkipGagged")->GetBool();
		settings->oppOneAtATime = MCMSetting("oppOneAtATime")->GetBool();
		settings->oppSCollarDrainsMagicka = MCMSetting("oppSCollarDrainsMagicka")->GetBool();
		settings->oppSCollarAutoRemove = MCMSetting("oppSCollarAutoRemove")->GetBool();
		settings->oppDwarvenHeavyRestraint = MCMSetting("oppDwarvenHeavyRestraint")->GetBool();
		settings->oppDwarvenRequireLoc = MCMSetting("oppDwarvenRequireLoc")->GetBool();
		settings->oppLivingLatexGemAll = MCMSetting("oppLivingLatexGemAll")->GetBool();
		settings->oppLivingLatexHeavy = MCMSetting("oppLivingLatexHeavy")->GetBool();
		settings->oppLivingLatexRequireRem = MCMSetting("oppLivingLatexRequireRem")->GetBool();
		settings->oppLivingLatexOpen = MCMSetting("oppLivingLatexOpen")->GetBool();
		settings->oppMadnessOnlyDevices = MCMSetting("oppMadnessOnlyDevices")->GetBool();
		settings->oppMadnessBeltFilter = MCMSetting("oppMadnessBeltFilter")->GetBool();
		settings->oppMadnessChaos = MCMSetting("oppMadnessChaos")->GetBool();
		settings->enableQuestInteractions = MCMSetting("enableQuestInteractions")->GetBool();
		settings->enableQIMalkoran = MCMSetting("enableQIMalkoran")->GetBool();
		settings->enableQISanguine = MCMSetting("enableQISanguine")->GetBool();
		settings->enableQIBlackStar = MCMSetting("enableQIBlackStar")->GetBool();
		settings->enableQIMindOfMadness = MCMSetting("enableQIMindOfMadness")->GetBool();
		settings->enableQILaidToRest = MCMSetting("enableQILaidToRest")->GetBool();
		settings->enableQICriedWolf = MCMSetting("enableQICriedWolf")->GetBool();
		settings->enableQIDelivery = MCMSetting("enableQIDelivery")->GetBool();
		settings->enableQIBarenziah = MCMSetting("enableQIBarenziah")->GetBool();
		settings->enableQIProvingHonor = MCMSetting("enableQIProvingHonor")->GetBool();
		settings->enableQISaarthal = MCMSetting("enableQISaarthal")->GetBool();
		settings->enableQIBrelyna = MCMSetting("enableQIBrelyna")->GetBool();
		settings->enableQIDwemerMuseum = MCMSetting("enableQIDwemerMuseum")->GetBool();
		settings->enableQITrinityRestored = MCMSetting("enableQITrinityRestored")->GetBool();
		settings->enableQIThalmorEmbassy = MCMSetting("enableQIThalmorEmbassy")->GetBool();
		settings->enableQIRingmaker = MCMSetting("enableQIRingmaker")->GetBool();
		settings->oppDeviceAntiCheat = MCMSetting("oppDeviceAntiCheat")->GetBool();
		settings->generalDeviceAntiCheat = MCMSetting("generalDeviceAntiCheat")->GetBool();
		settings->noMessageBoxes = MCMSetting("noMessageBoxes")->GetBool();
		settings->eventVisualEffects = MCMSetting("eventVisualEffects")->GetBool();
		settings->surpriseMode = MCMSetting("surpriseMode")->GetBool();
		settings->dragonHoard = MCMSetting("dragonHoard")->GetBool();
		settings->bossExtraGold = MCMSetting("bossExtraGold")->GetBool();
		settings->useThemes = MCMSetting("useThemes")->GetBool();
		settings->disableForce3rdPerson = MCMSetting("disableForce3rdPerson")->GetBool();
		settings->enableSlowStrip = MCMSetting("enableSlowStrip")->GetBool();
		settings->restrictFastTravel = MCMSetting("restrictFastTravel")->GetBool();
		settings->restrictFastTravelPart = MCMSetting("restrictFastTravelPart")->GetBool();
		settings->restrictFastTravelFull = MCMSetting("restrictFastTravelFull")->GetBool();
		settings->useDeviceNamesInThemes = MCMSetting("useDeviceNamesInThemes")->GetBool();
		settings->saveDependentSettings = MCMSetting("saveDependentSettings")->GetBool();
		settings->ANDSexTopless = MCMSetting("ANDSexTopless")->GetBool();
		settings->ANDSexBottomless = MCMSetting("ANDSexBottomless")->GetBool();
		settings->ANDSexShowingChest = MCMSetting("ANDSexShowingChest")->GetBool();
		settings->ANDSexShowingGenitals = MCMSetting("ANDSexShowingGenitals")->GetBool();
		settings->ANDSexShowingAss = MCMSetting("ANDSexShowingAss")->GetBool();
		settings->ANDSexShowingBra = MCMSetting("ANDSexShowingBra")->GetBool();
		settings->ANDSexShowingUnderwear = MCMSetting("ANDSexShowingUnderwear")->GetBool();
		settings->ANDConsTopless = MCMSetting("ANDConsTopless")->GetBool();
		settings->ANDConsBottomless = MCMSetting("ANDConsBottomless")->GetBool();
		settings->ANDConsShowingChest = MCMSetting("ANDConsShowingChest")->GetBool();
		settings->ANDConsShowingGenitals = MCMSetting("ANDConsShowingGenitals")->GetBool();
		settings->ANDConsShowingAss = MCMSetting("ANDConsShowingAss")->GetBool();
		settings->ANDConsShowingBra = MCMSetting("ANDConsShowingBra")->GetBool();
		settings->ANDConsShowingUnderwear = MCMSetting("ANDConsShowingUnderwear")->GetBool();
		settings->consEnable = MCMSetting("consEnable")->GetBool();
		settings->consGuardNormal = MCMSetting("consGuardNormal")->GetBool();
		settings->consFineGuardFaction = MCMSetting("consFineGuardFaction")->GetBool();
		settings->consFallthrough = MCMSetting("consFallthrough")->GetBool();
		settings->consAllowFollowers = MCMSetting("consAllowFollowers")->GetBool();
		settings->consFollowersBlock = MCMSetting("consFollowersBlock")->GetBool();
		settings->consTolerableChastity = MCMSetting("consTolerableChastity")->GetBool();
		settings->consUseRelationships = MCMSetting("consUseRelationships")->GetBool();
		settings->consRelationBondage = MCMSetting("consRelationBondage")->GetBool();
		settings->consRandomHeavyBondage = MCMSetting("consRandomHeavyBondage")->GetBool();
		settings->consBondageIgnoreMax = MCMSetting("consBondageIgnoreMax")->GetBool();
		settings->consTattooCurseNumbers = MCMSetting("consTattooCurseNumbers")->GetBool();
		settings->sexEnabled = MCMSetting("sexEnabled")->GetBool();
		settings->sexAggressiveAnims = MCMSetting("sexAggressiveAnims")->GetBool();
		settings->sexFilterDevices = MCMSetting("sexFilterDevices")->GetBool();
		settings->sexFilterFuta = MCMSetting("sexFilterFuta")->GetBool();
		settings->sexRandomEnabled = MCMSetting("sexRandomEnabled")->GetBool();
		settings->sexRandAllowFollowers = MCMSetting("sexRandAllowFollowers")->GetBool();
		settings->sexRandAllowNPCs = MCMSetting("sexRandAllowNPCs")->GetBool();
		settings->sexRandAllowSummons = MCMSetting("sexRandAllowSummons")->GetBool();
		settings->sexRandAllowCreatures = MCMSetting("sexRandAllowCreatures")->GetBool();
		settings->sexAllowMale = MCMSetting("sexAllowMale")->GetBool();
		settings->sexAllowFemale = MCMSetting("sexAllowFemale")->GetBool();
		settings->sexAllowFuta = MCMSetting("sexAllowFuta")->GetBool();
		settings->sexAllowCreature = MCMSetting("sexAllowCreature")->GetBool();
		settings->sexAllowFarmAnimals = MCMSetting("sexAllowFarmAnimals")->GetBool();
		settings->sexRequireAllPlayer = MCMSetting("sexRequireAllPlayer")->GetBool();
		settings->sexRequirePlayerCollar = MCMSetting("sexRequirePlayerCollar")->GetBool();
		settings->sexRequirePlayerHeavy = MCMSetting("sexRequirePlayerHeavy")->GetBool();
		settings->sexRequirePlayerNude = MCMSetting("sexRequirePlayerNude")->GetBool();
		settings->sexRequireAllNPC = MCMSetting("sexRequireAllNPC")->GetBool();
		settings->sexRequireNPCCollar = MCMSetting("sexRequireNPCCollar")->GetBool();
		settings->sexRequireNPCNude = MCMSetting("sexRequireNPCNude")->GetBool();
		settings->sexRequireBoth = MCMSetting("sexRequireBoth")->GetBool();
		settings->sexFollowerBypassPlayer = MCMSetting("sexFollowerBypassPlayer")->GetBool();
		settings->sexFollowerBypassNPC = MCMSetting("sexFollowerBypassNPC")->GetBool();
		settings->sexSpouseBypassPlayer = MCMSetting("sexSpouseBypassPlayer")->GetBool();
		settings->sexSpouseBypassNPC = MCMSetting("sexSpouseBypassNPC")->GetBool();
		settings->sexSummonBypassPlayer = MCMSetting("sexSummonBypassPlayer")->GetBool();
		settings->sexSummonBypassNPC = MCMSetting("sexSummonBypassNPC")->GetBool();
		settings->ModSuspended = MCMSetting("ModSuspended")->GetBool();
		settings->setAllDefaultSettings = MCMSetting("setAllDefaultSettings")->GetBool();
		settings->helpSlaveTatsNGInstalled = MCMSetting("helpSlaveTatsNGInstalled")->GetBool();
		settings->helpLewdMarksInstalled = MCMSetting("helpLewdMarksInstalled")->GetBool();
		settings->helpRapeTattoosInstalled = MCMSetting("helpRapeTattoosInstalled")->GetBool();
		settings->helpOSLArousedInstalled = MCMSetting("helpOSLArousedInstalled")->GetBool();
		settings->helpSLOArousedInstalled = MCMSetting("helpSLOArousedInstalled")->GetBool();
		settings->helpUDInstalled = MCMSetting("helpUDInstalled")->GetBool();
		settings->helpQuickLootInstalled = MCMSetting("helpQuickLootInstalled")->GetBool();
		settings->helpDWDInstalled = MCMSetting("helpDWDInstalled")->GetBool();
		settings->helpANDInstalled = MCMSetting("helpANDInstalled")->GetBool();
		settings->helpCockCagesWorking = MCMSetting("helpCockCagesWorking")->GetBool();
		settings->excludedFollowers = MCMSetting("excludedFollowers")->GetString();
		settings->followerOverrideTheme = MCMSetting("followerOverrideTheme")->GetString();
		settings->followerSkipKeywords = MCMSetting("followerSkipKeywords")->GetString();
		settings->LMNudityAditionalForms = MCMSetting("LMNudityAditionalForms")->GetString();
		//CODEGEN_END_UPDATE

		if (settings->setAllDefaultSettings) {
			ResetMCMSettings();
		}

		EventsCheckModIntergations();

		Quest::OnSettingsChanged();

		SaveMCMSettings();

		if (recalculate) {
			log::info("Recalculating device lists due to settings changes.");
			RecalculateDeviceLists();
			MCMSetHelpPageOptions();
		}
		if (NeedUpdateForExclusions()) {
			log::info("Recalculating device lists due to exclusions changes.");
			RecalculateDeviceLists();
			MCMSetHelpPageOptions();
		}
	}

	void GetSettingsFromMCM() {
		P_UpdateSKSE(nullptr);
	}

	void P_OnMCMOpened(RE::StaticFunctionTag*) {
		MCMSetHelpPageOptions();
	}

	bool PapyrusFunctionsSettigns(RE::BSScript::IVirtualMachine* ivm) {
		ivm->RegisterFunction("UpdateSKSE", "DCurses_MCM", P_UpdateSKSE);
		ivm->RegisterFunction("OnMCMOpened", "DCurses_MCM", P_OnMCMOpened);
		return true;
	}
}