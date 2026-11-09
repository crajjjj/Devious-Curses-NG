#pragma once

#include "form_ids.h"
#include "Translation.h"

typedef int color;
typedef int keycode;
typedef std::string text;
typedef int menu;

namespace DCURSES {

	class Settings {
	private:		
		Settings() {}
	public:
		std::filesystem::file_time_type lastSettingsEditTime = std::chrono::clock_cast<std::filesystem::file_time_type::clock>(std::chrono::system_clock::now());

		Settings(Settings const&) = delete;
		void operator=(Settings const&) = delete;

		static Settings* GetSingleton() {
			static Settings instance;
			return &instance;
		};

		bool debugMode = false;
		std::string lastLoadedVersion = DCURSES_VERSION;

		//MCM_START
		//Flag flag_SlaveTats					//CheckSTNG()
		//Flag flag_LewdMarks					//CheckLM() && CheckSTNG()
		//Flag flag_RapeTats					//ESP:RapeTattoos.esp && CheckSTNG()
		//Flag flag_RT_LM						//ESP:RapeTattoos.esp && CheckSTNG() && CheckLM()
		//Flag flag_SimpleSlavery				//ESP:SimpleSlavery.esp
		//Flag flag_UnforgivingDevices			//ESP:UnforgivingDevices.esp
		//Flag flag_SGO							//ESP:Sgo4IF.esp
		//Flag flag_LockOppDevice_Menu			//!oppDeviceAntiCheat || !WearingOppressiveDevice() 
		//Flag flag_WickedDevices				//ESP:Devious Wicked Devices.esp
		//Flag flag_AND							//ESP:Advanced Nudity Detection.esp
		
		//Page Main								
		//Header Chances
		float baseChance = 6.5f;				//Base Event Chance//How likely are you to trigger a trap before modifiers.//{1}%//(0,100,0.1)					
		float containerModifier = 1.0f;			//Container Modifier//Modifier for containers.//{1}x//(0,10,0.1)
		float bossContainerModifier = 2.0f;		//Boss Chest Modifier//Modifier for boss chests.\nIs applied with the container modifier.//{1}x//(0,10,0.1)
		float deadBodyModifier = 1.3f;			//Dead Body Modifier//Modifier for corpses.//{1}x//(0,10,0.1)
		float pickpocketModifier = 1.3f;		//Pickpocket Modifier//Modifier for pickpocketing.//{1}x//(0,10,0.1)
		float doorModifier = 1.0f;				//Door Modifier//Modifier for doors.//{1}x//(0,10,0.1)
		bool onlyLockedDoors = true;			//Only Locked Doors//Only trigger events when the door is locked.
		float lockedModifier = 2.0f;			//Locked Modifier//Modifier for locked doors and chests.\nThis will not apply if the lock requires a key or you already have the key to the lock.//{1}x//(0,10,0.1)
		float lockDifficultyModifier = 1.3f;	//Lock Difficulty Modifier//If this is greater than 1, locked things will have a higher chance to cause a curse the harder the lock is to pick.\nIf set to 10 a master level lock will multiply the chance by 10 while an adept lock would multiply the chance by 5.//{1}//(1,10,0.1)
		int minGoldRequired = 20;				//Container Gold Value//Requires a container to have at least this total value of items in it to trigger any events.//{0}//(0,5000,10)
		bool eventScaling = true;				//Event Scaling//Make events less likely right after triggering one, and more likely if not triggered in a while.
		int eventScalingMod = 15;				//Event Scaling Target//The number of events before traps start becoming more likely.//{0}//(1,50,1)
		//Column
		//Header Parameters
		int minRestraints = 1;					//Min Restraints//Minimum number of restraints.//{0}//(1,20,1)
		int maxRestraints = 3;					//Max Restraints//Maximum number of restraints.//{0}//(1,20,1)
		int bossAditionalRestraints = 2;		//Boss Restraints//Added restraints when opening a boss chest.//{0}//(0,10,1)
		bool bossOnlyHeavy = true;				//Boss Heavy Restraints//Heavy restraints can only be applied from boss chests.
		int restraintCap = 7;					//Restraints Cap//Events won't happen if you have more than this many restraints.//{0}//(1,20,1)
		bool stripPlayerOnEvent = true;			//Strip Player//Toggle to choose if the player should be stripped on any event.\nIf a heavy bondage device is equipped the player will be stripped anyway.
		bool stripOnlyKeywords = true;			//Strip Armor Keywords//This will only remove items that are tagged as armor or clothing through keywords.\nThis may cause some modded items to not be unequipped.
		//Header Arousal
		float arousalModifier = 1.4f;			//Arousal Modifier//If this is greater than 1, curses will me more likely the more aroused the player is.\nIf set to 10 with full arousal will multiply the chance by 10 while half arousal would multiply the chance by 5.//{1}//(1,10,0.1)
		int minArousal = 10;					//Minimum Arousal//Curses won't trigger if the player is below this arousal level.//{0}//(0,100,1)
		
		//Page Devices
		//Header Chastity
		int beltWeight = 40;					//Belts Weight//Chance to be equipped with a chastity belt.//{0}//(0,100,1)
		bool beltPlugs = true;					//Belt Plugs//Equip plugs with all belts.
		int chastityPiercingWeight = 20;		//Chastity Piercing Weight//Chance to be equipped with a chastity piercing.\nWill only matter if you have a mod like Devious Chastity Piercings.//{0}//(0,100,1)
		bool useGenderedChastity = true;		//Use Gendered Chastity//Equip futa characters with cock cages instead of regular belts. Will also prevent female characters from getting cages.\nWill do nothing without Vivis Cockcages Devious Patch.		**RECALC
		int braWeight = 30;						//Bras Weight//Chance to be equipped with a chastity bra.//{0}//(0,100,1)
		int plugsWeight = 50;					//Plugs Weight//Chance to be equipped with plugs when no belt is worn.//{0}//(0,100,1)
		int lockingPlugsWeight = 30;			//Locking Plugs Weight//Chance to be equipped with plugs that can lock.//{0}//(0,100,1)
		int inflatablePlugsWeight = 15;			//Inflatable Plugs Weight//Chance to be equipped with plugs that are inflatable.//{0}//(0,100,1)
		bool plugsDontCount = true;				//Free Plugs//Plugs won't be considered for device limits.\nSo if a plug is rolled and the max devices is 1, another device can still be added.
		bool useGenderedPlugs = false;			//Use Gendered plugs//Vaginal plugs wil only be equipped on characters that don't have a penis.\nThis will prevent futa characters from being equipped with vaginal plugs.
		//Header Piercings
		int nipplePiercingsWeight = 50;			//Nipple Piercings Weight//Chance to be equipped with nipple piercings.//{0}//(0,100,1)
		int vaginalPiercingsWeight = 50;		//Vaginal Piercing Weight//Chance to be equipped with a vaginal piercing.//{0}//(0,100,1)
		//Header Corsets & Harnesses
		int corsetWeight = 40;					//Corset Weight//Chance to be equipped with a restrictive corset.//{0}//(0,100,1)
		//bool corsetBelts = true;				//Corset Belts//Allow corsets that also function as belts to be equipped.
		int beltedCorsetsWeight = 30;			//Belted Corset Weight//Chance to be equipped with a restrictive corset that has an attached belt.//{0}//(0,100,1)
		int slaveHarnessWeight = 50;			//Open Harness Weight//Chance to be equipped with an open harness.\nThis will not act as a chastity belt.//{0}//(0,100,1)
		int chastityHarnessWeight = 40;			//Chastity Harness Weight//Chance to be equipped with an open harness.\nThis will act as a chastity belt.//{0}//(0,100,1)
		//Header Heavy Bondage
		int armbinderWeight = 30;				//Armbinder Weight//Chance to be equipped with an armbinder.//{0}//(0,100,1)
		int elbowbinderWeight = 20;				//Elbowbinder Weight//Chance to be equipped with an elbowbinder.//{0}//(0,100,1)
		int yokeWeight = 10;					//Yoke Weight//Chance to be equipped with a yoke.//{0}//(0,100,1)
		int shacklesWeight = 20;				//Shackles Weight//Chance to be equipped with restraining wrist shackles.//{0}//(0,100,1)
		int straitjacketWeight = 30;			//Straitjacket Weight//Chance to be equipped with a straitjacket.//{0}//(0,100,1)
		int straitjacketLegbinderWeight = 15;	//Hobbling Straitjacket Weight//Chance to be equipped with a straitjacket that has a legbinder.//{0}//(0,100,1)
		int petSuitWeight = 10;					//Pet Suit Weight//Chance to be equipped with a pet suit.//{0}//(0,100,1)
		//Column
		//Header Collars & Cuffs	
		int collarWeight = 60;					//Collar Weight//Chance to be equipped with a collar.//{0}//(0,100,1)
		int armCuffsWeight = 60;				//Arm Cuffs Weight//Chance to be equipped with arm cuffs.//{0}//(0,100,1)
		int legCuffsWeight = 60;				//Leg Cuffs Weight//Chance to be equipped with leg cuffs.//{0}//(0,100,1)
		bool allowLegShackles = false;			//Leg Shackles//Allow leg cuffs that slow the player.
		//Header Gags & Blindfolds
		int gagWeight = 35;						//Gag Weight//Chance to be equipped with a simple gag\nThese will always block oral.//{0}//(0,100,1)
		int ringGagWeight = 45;					//Ring Gag Weight//Chance to be equipped with a ring gag\nThese will never block oral.//{0}//(0,100,1)
		int largeGagWeight = 30;				//Large Gag Weight//Chance to be equipped with a large gag\nThese will always block speech.//{0}//(0,100,1)
		int largeRingGagWeight = 30;			//Large Ring Gag Weight//Chance to be equipped with a large ring gag\nThese will always block speech and never block oral.//{0}//(0,100,1)
		int blindfoldWeight = 20;				//Blindfold Weight//Chance to be equipped with a blindfold.//{0}//(0,100,1)
		int hoodBothWeight = 20;				//Hood Weight//Chance to be equipped with a hood.//{0}//(0,100,1)
		int hoodGagWeight = 20;					//Open Eyes Hood Weight//Chance to be equipped with a hood that only blocks the mouth.//{0}//(0,100,1)
		int hoodBlindWeight = 20;				//Open Mouth Hood Weight//Chance to be equipped with a hood that only blocks the eyes.//{0}//(0,100,1)
		int hoodNoneWeight = 20;				//Open Hood Weight//Chance to be equipped with a hood that doesn't block the mouth or eyes.//{0}//(0,100,1)
		//Header Body
		int catsuitWeight = 30;					//Catsuit Weight//Chance to be equipped with a catsuit.//{0}//(0,100,1)
		int hobbleSkirtWeight = 20;				//Hobble Dress Weight//Chance to be equipped with a hobble dress.//{0}//(0,100,1)
		int hobbleSkirtDifficulty = 50;			//Hobble Dress Difficulty//How restraining hobble dresses will be on average.\nThe higher the number the greater chance for a strict dress vs a relaxed dress.//{0}//(0,100,1)
		int bootsWeight = 30;					//Boots Weight//Chance to be equipped with restrictive boots.//{0}//(0,100,1)
		int glovesWeight = 30;					//Gloves Weight//Chance to be equipped with restrictive gloves.//{0}//(0,100,1)
		int mittensWeight = 0;					//Mittens Weight//Chance to be equipped with bondage mittens.//{0}//(0,100,1)
		
		//Page Keys
		float keyLossChance = 80.0f;			//Key Loss Chance//How likely you are to lose your keys during an event.//{1}%//(0,100,0.1)
		float keyChance = 7.0f;					//Key Find Chance//How likely you are to find a key in a container.//{1}%//(0,100,0.1)
		int minKeysLooted = 1;					//Min Keys//The minimum number of keys that can be found at a time.//{0}//(1,10,1)
		int maxKeysLooted = 1;					//Max Keys//The maximum number of keys that can be found at a time.//{0}//(1,10,1)
		float keyBonus = 1.0f;					//Bonus Chance Per Device//A bonus chance to get a key per locking device worn.//{1}%//(0,10,0.1)
		bool keyForgiveness = true;				//Key Forgiveness//Make keys more likely if it has been a while since you got any.
		float keyPickpocketBonus = 2.0f;		//Pickpocket Bonus//Multiplier to key chance when pickpocketing someone.\nSet to 0 to disable keys when pickpocketing.//{1}x//(0,10,0.1)
		int maxHeldKeys = 3;					//Max Held Keys//Sets the maximum allowed amount of keys that you can carry while still finding more.\nWith this enabled no devices will be equipped that require more keys than you can find.\nFor example if you have two restraint keys and one chastity key and this is set to three you will no longer find keys.\nSet to 0 to disable.//{0}//(0,100,1) **RECALC
		//Column
		int restraintsKeyWeight = 80;			//Restraints Key Weight//Chance to find a restraints key.//{0}//(0,100,1)
		int chastityKeyWeight = 50;				//Chastity Key Weight//Chance to find a chastity key.//{0}//(0,100,1)
		int piercingToolWeight = 20;			//Piercing Tool Weight//Chance to find a piercing removal tool.//{0}//(0,100,1)
		//Empty
		float magicKeyChance = 20.0f;			//Magic Key Chance//Chance that magic keys will be found in boss chests. They will destroy all restraints you are wearing.\nYou can only have a max of one at a time and they can never be lost.\nSet to 0 to disable//{1}//(0,50,0.1)
		bool magicKeyOppressive = false;		//Magic Keys Remove Oppressive//Magic keys will remove one random oppressive device when used.
		bool preferRelevantKeys = true;			//Prefer Relevant Keys//You will only find keys that would unlock items you are wearing.
		bool vanishingKeys = true;				//Vanishing Keys//Keys will be removed from containers after you close the menu.
		float keyLuckyBonus = 1.75f;			//Lucky Bonus//Keys chance will be multiplied by this value if you are lucky.//{2}//(1,10,0.01)


		//Page Events
		//Header Bondage Curse
		int eventStandardWeight = 100;			//Bondage Curse Weight//Chance to receive a bondage device event.//{0}//(0,500,1)
		int eventStandardBossReduction = 20;	//Standard Boss Reduction//If the container is a boss chest, the standard event weight will be reduced by this amount.//{0}//(0,500,1)
		//Header Oppresive Curse
		int eventOppressiveWeight = 15;			//Oppressive Curse Weight//Chance to receive an oppressive device event.//{0}//(0,500,1)
		//Header Contraption Curse
		int eventContraptionWeight = 25;		//Contraption Curse Weight//Chance to be bound in a contraption from DDC.//{0}//(0,500,1)																											
		float eventContraptionTime = 4.0f;		//Contraption Release Time//Will be automatically released after this many in game hours. Set to 0 to disable automatic release.//{1}//(0,24,0.1)													
		bool eventContDevices = true;			//Contraption Devices//Will allow certain devices such as collars, gags, cuffs, and plugs to be equipped when triggering a contraption event.\nWill follow all other rules set for devices.			
		bool eventContAllDevices = false;		//Use All Devices//Will allow any device other than heavy bondage to be equipped when triggering a contraption event.\nWill follow all other rules set for devices.									
		int eventContDeviceOverride = 0;		//Device Count Override//When equipping devices for contraption events this number will be used instead of the min / max on the main page.\nSet to 0 to use the default number of devices.//{0}//(0,10,1)
		//Header Wicked Curse
		int eventWickedWeight = 15;				//Wicked Curse Weight//Chance to be bound in a device from Devious Wicked Devices.//{0}//(0,500,1)							?:? flag_WickedDevices
		//Header Tattoo Curse
		int eventTattooWeight = 15;				//Tattoo Curse Weight//Chance to receive random tattoos.\nRequires Rape Tattoos.//{0}//(0,500,1)							?:? flag_RapeTats
		int eventTattooMin = 1;					//Tattoo Curse Min//Minimum number of tattoos that can be put on.//{0}//(1,10,1)											?:? flag_RapeTats
		int eventTattooMax = 3;					//Tattoo Curse Max//Maximum number of tattoos that can be put on.//{0}//(1,10,1)											?:? flag_RapeTats
		int eventTattooCap = 8;					//Tattoo Curse Cap//If you have this many tattoos already you won't get any more.//{0}//(0,20,1)							?:? flag_RapeTats
		//Column
		//Header Mark Curse
		int eventLewdMarkWeight = 10;			//Lewd Mark Weight//Chance to receive a lewd mark.//{0}//(0,500,1) ?:? flag_LewdMarks
		//Header Slavery Curse
		int eventSimpleSlaveryWeight = 0;		//Simple Slavery Weight//Chance to trigger a Simple Slavery auction.//{0}//(0,500,1) ?:? flag_SimpleSlavery					**RELOAD
		int eventSSMinRestraints = 6;			//Minimum Restraints//Minimum restraints that need to be equipped for a Simple Slavery auction to start.//{0}//(0,10,1)		?:? flag_SimpleSlavery
		//Header Abadon Curse
		int eventAbadonWeight = 5;				//Abadon Plug Weight//Chance to be equipped with an Abadon Plug. This will trigger exactly the same as the quest from UD.//{0}//(0,500,1)						?:? flag_UnforgivingDevices
		//Header Abadon Sets
		int eventAbadonWarriorWeight = 10;		//Abadon Warrior Weight//Chance to be equipped with devices from the Abadon Warrior set.//{0}//(0,500,1)														?:? flag_UnforgivingDevices
		int eventAbadonWarriorCount = 5;		//Abadon Warrior Count//Number of Warrior devices that will be equipped durring the event//{0}//(1,10,1)														?:? flag_UnforgivingDevices
		int eventAbadonScoutWeight = 10;		//Abadon Scout Weight//Chance to be equipped with devices from the Abadon Scout set.//{0}//(0,500,1)															?:? flag_UnforgivingDevices
		int eventAbadonScoutCount = 5;			//Abadon Scout Count//Number of Scout devices that will be equipped durring the event//{0}//(1,10,1)															?:? flag_UnforgivingDevices
		int eventAbadonWitchWeight = 10;		//Abadon Witch Weight//Chance to be equipped with devices from the Abadon Witch set.//{0}//(0,500,1)															?:? flag_UnforgivingDevices
		int eventAbadonWitchCount = 5;			//Abadon Witch Count//Number of Witch devices that will be equipped durring the event//{0}//(1,10,1)															?:? flag_UnforgivingDevices
		
		//Page Followers
		bool followerBondageEvent = false;		//Follower Bondage Curses//Allow followers to be affected by bondage curses.
		bool followerTattooEvent = false;		//Follower Tattoo Curses//Allow followers to be affected by tattoo curses.
		bool onlyFemaleFollowers = true;		//Only Female Followers//Follower events will only trigger if the follower uses a female body (female & futa).
		text excludedFollowers = "";			//Excluded Followers//List of followers that are excluded from events. Names should be comma-separated.\nThis will check to see if any part of their name matches.\nExample: "Lydia, J'zargo"
		text followerOverrideTheme = "";		//Override Theme//This theme will be used for all events that affect followers.
		text followerSkipKeywords = "";			//Follower Skip Keywords//Any devices with these keywords will be skipped.\nThis is a comma-separated list of keywords.
		//Column
		bool followerHeavyRestraints = false;	//Follower Heavy Restraints//Will allow events to equip heavy restraints on followers.
		bool followerGags = false;				//Follower Gags//Will allow gags to be equipped on followers.
		float followerDeviceMult = 0.75f;		//Follower Device Multiplier//Will act as a multiplier on how many devices are equipped on followers.\nWill be applied before the modifier.//{2}//(0,3,0.02)
		int followerDeviceModifier = 0;			//Follower Device Modifier//Will add (or subtract) this many devices when equipping followers with devices.\nFollowers will always be equipped with at least one device.//{0}//(-10,10,1)

		//Page Unforgiving Devices
		bool onlyUseUnforgivingDevices = true;	//Only Unforgiving Devices//Only register devices to the mod that are converted to work with UD.\n(Will cause lag on menu close)													?:? flag_UnforgivingDevices  **RECALC
		bool udUseAbadon = false;				//Allow Abadon Devices//Allow Abadon devices to be enabled in the normal random device search. Will not equip the Abadon Plug even if enabled.\n(Will cause lag on menu close)		?:? flag_UnforgivingDevices  **RECALC
		bool udUseAbadonSets = false;			//Allow Abadon Sets//Allow Abadon Sets like Warrior and Witch to be enabled in the normal random device search.\n(Will cause lag on menu close)										?:? flag_UnforgivingDevices  **RECALC
		bool udUseMisc = true;					//Allow Misc Unforgiving Devices//Allow devices like chargable plugs, punisher items, etc. to be enabled in the normal random device search\n(Will cause lag on menu close)			?:? flag_UnforgivingDevices  **RECALC
		
		//Page Lewd Marks
		//Header Allure
		int LMAllureWeight = 10;				//Allure Mark Weight//This mark will make everyone around you horny all the time.//{0}//(0,500,1)							?:? flag_LewdMarks
		int LMAllureMod = 5;					//Allure Arousal//How much each nearby character's arousal will change per minute.//{0}//(0,200,1)							?:? flag_LewdMarks
		int LMAllureSex = 25;					//Sex Count//How many times you have to have sex before the mark will fade.//{0}//(3,500,1)									?:? flag_LewdMarks
		bool LMAllureSkipRequirements = true;	//Skip Sex Requirements//Requirements for both the player and NPC's will be skipped	for random sex.							?:? flag_LewdMarks
		color LMAllureColor = 0xcf11c5;			//Color//Color for mark.																									?:? flag_LewdMarks
		//Header Heat
		int LMHeatWeight = 10;					//Heat Mark Weight//This mark will constantly make you horny.//{0}//(0,500,1)												?:? flag_LewdMarks
		int LMHeatMod = 30;						//Heat Arousal//How much your arousal will change per minute.//{0}//(0,200,1)												?:? flag_LewdMarks
		int LMHeatContainerCount = 50;			//Container Count//How many containers you must open before the mark will fade.//{0}//(10,1000,1)							?:? flag_LewdMarks
		color LMHeatColor = 0xe3143a;			//Color//Color for mark.																									?:? flag_LewdMarks
		//Header Branding
		int LMBrandingWeight = 10;				//Branding Mark Weight//This mark will force you to have a certain number of tattoos!//{0}//(0,500,1)						?:? flag_RT_LM
		float LMBrandingChance = 1.5f;			//Chance//How likely you are to receive a random tattoo per 15 seconds.\nRequires Rape Tattoos//{1}%//(0,100,0.1)			?:? flag_RT_LM
		int LMBrandingTotal = 12;				//Total Tattoos//How many tattoos you need before the mark releases. The mark itself counts as 2.//{0}//(1,20,1)			?:? flag_RT_LM
		bool LMBrandingPunish = true;			//Punishment//You will be punished by loosing gold if your total tattoo count decreases.									?:? flag_RT_LM
		color LMBrandingColor = 0x220022;		//Color//Color for mark.																									?:? flag_RT_LM
		//Header Healslut
		int LMHealslutWeight = 5;				//Healslut Mark Weight//With this mark you won't be able to deal damage and must heal your allies instead.//{0}//(0,500,1)																		?:? flag_LewdMarks
		int LMHealslutHealing = 4000;			//Healing Amount//How much you have to heal your followers to remove the mark. This counts even if your followers are at full health.\nSpells cast on followers will also increase their arousal.//{0}//(100,100000,100)	?:? flag_LewdMarks
		int LMHealslutArousal = 90;				//Sex Arousal//How aroused your follower needs to be for them to have sex with you when you cast a healing spell on them.\nWill not trigger in combat. Set to 101 to disable.//{0}//(0,101,1)	?:? flag_LewdMarks
		int LMHealslutDuration = 300;			//Buff Spell Duration//How long the reward spell lasts after having sex with a follower.//{0}//(30,600,10)																						?:? flag_LewdMarks
		color LMHealslutColor = 0xffbb45;		//Color//Color for mark.																																										?:? flag_LewdMarks
		//Column
		//Header Bondage
		int LMBondageWeight = 5;				//Bondage Mark Weight//With this mark you will be cursed with bondage items more frequently!//{0}//(0,500,1)																					?:? flag_LewdMarks
		float LMBondageModifier = 5.0f;			//Event Modifier//Modifier to event chance with the mark.\nThis will only increase the chance of the Bondage Curse, and curses triggerd this way won't affect followers.//{1}X//(1,100,0.1)		?:? flag_LewdMarks
		//float LMBondageChance = 5.0f;			//Chance//How likely an item might be equipped from your inventory per 15 seconds.//{1}%//(0,100,0.1)																							?:? flag_LewdMarks
		int LMBondageDeviceCapAdd = 3;			//Restraints Cap Modifier//How much higher the Restraints Cap for events will be.//{0}//(0,10,1)																								?:? flag_LewdMarks
		int LMBondageDeviceCount = 25;			//Device Count//How many devices need to be equipped before the mark will fade.\nThis counts items equipped by the bondage curse and from your inventory.//{0}//(1,2000,1)						?:? flag_LewdMarks
		color LMBondageColor = 0x7908cf;		//Color//Color for mark.																																										?:? flag_LewdMarks
		//Header Nudity
		//Flag flag_LMStripBody					//LMNudityChestOnly && flag_LewdMarks == 0
		int LMNudityWeight = 15;				//Nudity Mark Weight//With this mark you will be unable to wear any clothes.//{0}//(0,500,1)								?:? flag_LewdMarks
		bool LMNudityChestOnly = false;			//Chest Only//With this enabled only chest armor will be checked and removed. Otherwise all armor will be unequipped.		?:? flag_LewdMarks **RELOAD
		text LMNudityAditionalForms = "";		//Strip Slots//A comma separated list of additional slots to strip. Will not strip devices.\nFor example: 46,47,49,52.		?:? flag_LMStripBody
		int LMNudityTalkTimes = 35;				//Dialogue Times//How many times you have to talk to different characters before the mark will fade.//{0}//(3,500,1)		?:? flag_LewdMarks
		bool LMNuditySkipGagged = true;			//Skip When Gagged//The Nudity Mark counter will not decrease when the player is gagged. You must actually talk to people.	?:? flag_LewdMarks
		color LMNudityColor = 0xd676cb;			//Color//Color for mark.																									?:? flag_LewdMarks
		//Header Painslut
		int LMPainslutWeight = 10;				//Painslut Mark Weight//This mark will make you more aroused whenever you are hit by an enemy!//{0}//(0,500,1)				?:? flag_LewdMarks
		int LMPainslutMod = 5;					//Arousal Change//How much arousal you will gain when getting hit.//{0}//(1,100,1)											?:? flag_LewdMarks
		int LMPainslutCount = 25;				//Enemy Count//How many enemies you must kill at high arousal.//{0}//(1,1000,1)												?:? flag_LewdMarks
		float LMPainslutSexChance = 15.0f;		//Sex Chance//Chance that your attacker will have sex with you when you are hit at high arousal.//{1}//(0,100,0.1)			?:? flag_LewdMarks
		int LMPainslutArousal = 90;				//High Arousal//What counts for "high arousal" for this mark.//{0}//(1,100,1)												?:? flag_LewdMarks
		color LMPainslutColor = 0x911300;		//Color//Color for mark.																									?:? flag_LewdMarks

		//Page Oppressive Devices
		bool oppOneAtATime = true;				//Only One//With this enabled you will not be equipped with an Oppressive Device if you are already wearing one\nThis restriction is ignored for Quest Interactions.
		//Header Summoner Collar
		int oppSummonerCollarWeight = 20;		//Weight//How likely that you will be equipped with a collar that makes you have sex with your summons.\nRequires creatures to be enabled for sex.//{0}//(0,500,1)
		int oppSummonerSexCount = 15;			//Sex Count//How may time you need to have sex with your summons before the collar will unlock.//{0}//(1,100,1)
		menu oppSummonerNPC = 0;				//Summoned NPC//Select which types of summons that are allowed for the collar.//[Familiar, Flame Atronach, Frost Atronach, Storm Atronach, Dremora Lord, Random Atronach, Random]
		bool oppSCollarDrainsMagicka = true;	//Magicka Drain//The collar will drain all of your magicka when summoning.
		bool oppSCollarAutoRemove = true;		//Auto Remove//Remove the collar as soon as the requirements are met instead of just giving the key for it.
		float oppSummonChance = 1.5f;			//Summon Chance//The chance that the collar summons an atronach.\nThis won't happen if you already have a different summon.\nThis chance is rolled every five seconds.//{1}%//(0,100,0.1)
		float oppSummonSexChance = 25.0f;		//Summon Sex Chance//The chance that one of your summons will try to have sex with you.\nThis chance is rolled every five seconds.//{1}%//(0,100,0.1)
		//Header Dwarven Cuirass
		int oppDwarvenCuirassWeight = 15;		//Weight//How likely that you will be equipped with a devious dwarven cuirass that will control your actions.//{0}//(0,500,1)
		int oppDwarvenValueNeeded = 200;		//Dwarven Parts Value//How much value of dwarven parts like gears and gyros the dwarven cuirass needs to take before it is removed.\nDoes not count ingots, centurion dynamos, or pots, pans, cups, etc.//{0}//(30,2000,10)
		bool oppDwarvenHeavyRestraint = false;	//Heavy Restraint//The cuirass will count as a heavy bondage like a straitjacket. This will make removing the device much harder.\nWill only affect the cuirass before it is equipped.
		bool oppDwarvenRequireLoc = true;		//Require Location//The cuirass can only be found in dwarven locations.
		int oppDwarvenArousal = 25;				//Masturbation Arousal//If your arousal is above this there is a chance that the dwarven cuirass will force you to masturbate!\nSet to 100 to disable//{0}//(10,100,1)
		//Header Nocturnal Piercings
		int oppNocturnalWeight = 10;			//Weight//How likely that you will be equipped with a piercing that can grant invisibility.//{0}//(0,500,1)
		int oppNocturnalSexCount = 3;			//Public Sex Count//How many times you need to have sex in front of people before the piercing is removed.//{0}//(1,100,1)
		int oppNocturnalPeople = 7;				//Spectators//How many people need to watch you have sex for it to count.\nFollowers are not counted for this.//{0}//(2,15,1)
		int oppNocturnalArousal = 35;			//Arousal//At what arousal will the invisibility automatically drop.//{0}//(10,95,1)
		float oppNocturnalHandChance = 3.5f;	//Shadow Hand Chance//The chance per second that an invisible hand will touch you.//{1}%//(0,50,0.1)
		float oppNocturnalRecastChance = 33.3f;	//Recast Chance//The chance that an invisible hand will touch you every time the spell is recast.//{1}%//(10,100,0.1)
		//Column
		//Empty
		//Header Living Latex
		int oppLivingLatexWeight = 20;			//Weight//How likely that you will be encased in latex that will bind you with ebonite.//{0}//(0,500,1)
		int oppLivingLatexStartTime = 5;		//Start Time//How long in minutes do you have to wear the latex before it isn't dormant.\n//{0}//(1,60,1)
		float oppLivingLatexGem = 6.0f;			//Volatile Gem Chance//The chance to find a gem on dead mages and warlocks that will shock the latex when hit with a weapon or activated.//{1}//(0,100,0.1)
		bool oppLivingLatexGemAll = false;		//Volatile Gems on All Characters//Volatile gems will drop from all humanoid NPCs, not just mages and warlocks.
		bool oppLivingLatexHeavy = false;		//Heavy Bondage//The latex will bind you with heavy bondage devices.\nWarning: this will happen in combat.
		float oppLivingLatexMore = 0.0f;		//Periodic Devices//How frequently in minutes the latex will bind you when active. Set to 0 to disable.\nWarning: this will happen in combat.//{1}//(0,10,0.1)
		bool oppLivingLatexRequireRem = true;	//Clinging//The latex will cling to your other devices, requiring you to remove all of them before it will dissapear.
		bool oppLivingLatexOpen = false;		//Use Open Catsuit//Will replace the default catsuit with the open variant. Will only work if you don't have the device yet.
		//Header Madness Plug
		int oppMadnessPlugWeight = 25;			//Weight//How likely you will be equipped with a plug that will try to drive you mad.//{0}//(0,500,1)
		int oppMadnessPlugIterations = 5;		//Number of Events//How many events the plug will do before it is removed.//{0}//(1,20,1)
		int oppMadnessplugOrgasms = 6;			//Orgasms Per Event//How many times the plug must make you orgasm per event.//{0}//(1,100,1)
		bool oppMadnessOnlyDevices = true;		//Only Device Orgasms//This will change the counter to only increase when a device causes an orgasm.
		bool oppMadnessBeltFilter = true;		//Belt Filter//The plug will only be equipped if you are not wearing a belt (or vaginal plug) that can't be removed.
		float oppMadnessMultiplier = 1.0f;		//Frequency Multiplier//This will adjust how frequently vibration events from the plug will happen.//{2}//(0,10,0.01)
		bool oppMadnessChaos = false;			//True Madness//Will remove the restrictions on what events can happen. These events may result in PERMANENT changes to your character (Lowering your health, magicka, or stamina).
		
		//Page Locations
		float lockedLocationBypass = 0.6f;		//Lock Bypass//Locked containers can't have a location modifier less than this setting.\nUseful to make locked containers still trigger traps in cities.//{1}x//(0,10,0.1)
		float theftLocationBypass = 1.1f;		//Theft Bypass//Stealing from containers can't have a location modifier less than this setting.\nUseful to make stealing from containers still trigger traps in cities.//{1}x//(0,10,0.1)
		//Empty
		float playerHomeModifier = 0.0f;		//Player Home Modifier//Modifier for events to happen inside of player homes.//{1}x//(0,10,0.1)						
		float cityModifier = 0.0f;				//City Modifier//Modifier for events to happen inside of walled cities.//{1}x//(0,10,0.1)							
		float townModifier = 0.0f;				//Town Modifier//Modifier for events to happen inside of towns.//{1}x//(0,10,0.1)									
		float banditModifier = 1.1f;			//Bandit Modifier//Modifier for events to happen in or near bandit hideouts.//{1}x//(0,10,0.1)						
		float draugrModifier = 1.2f;			//Draugr Modifier//Modifier for events to happen in draugr crypts.//{1}x//(0,10,0.1)								
		float soulCairnModifier = 1.5f;			//Soul Cairn Modifier//Modifier for events to happen in the soul cairn.//{1}x//(0,10,0.1)
		//Column
		//Empty
		//Empty
		//Empty
		float dwarvenModifier = 1.2f;			//Dwarven Modifier//Modifier for events to happen in dwarven ruins.//{1}x//(0,10,0.1)								
		float falmerModifier = 1.3f;			//Falmer Modifier//Modifier for events to happen in falmer hives.//{1}x//(0,10,0.1)									
		float forswornModifier = 1.1f;			//Forsworn Modifier//Modifier for events to happen in forsworn hideouts.//{1}x//(0,10,0.1)							
		float vampireModifier = 1.5f;			//Vampire Modifier//Modifier for events to happen in vampire lairs.//{1}x//(0,10,0.1)								
		float warlockModifier = 1.5f;			//Warlock Modifier//Modifier for events to happen in warlock lairs.//{1}x//(0,10,0.1)								
		float dragonLairModifier = 2.0f;		//Dragon Lair Modifier//Modifier for events to happen in dragon lairs.//{1}x//(0,10,0.1)							
		float apocryphaModifier = 2.0f;			//Apocrypha Modifier//Modifier for events to happen in apocrypha.//{1}x//(0,10,0.1)									
		float wildernessModifier = 0.9f;		//Wilderness Modifier//Modifier for events to happen in the wilderness.//{1}x//(0,10,0.1)							
		
		//Page Quest Interactions
		//Flag flag_enable_qi					//enableQuestInteractions
		bool enableQuestInteractions = true;	//Quest Interactions//Certain quests may have some additional events tied to them.\n The mod page has more information about each quest.		**RELOAD
		//Header Quest Toggles
		bool enableQIMalkoran = true;			//Malkoran//									?:? flag_enable_qi
		bool enableQISanguine = true;			//Sanguine//									?:? flag_enable_qi
		bool enableQIBlackStar = true;			//Black Star//									?:? flag_enable_qi
		bool enableQIMindOfMadness = true;		//Sheogorath//									?:? flag_enable_qi
		bool enableQILaidToRest = true;			//Laid To Rest//								?:? flag_enable_qi
		bool enableQICriedWolf = true;			//The Man Who Cried Wolf//						?:? flag_enable_qi
		bool enableQIDelivery = true;			//Delivery & Rare Gifts//						?:? flag_enable_qi
		bool enableQIBarenziah = true;			//No Stone Unturned//							?:? flag_enable_qi
		//Column
		//Empty
		//Empty
		bool enableQIProvingHonor = true;		//Proving Honor//								?:? flag_enable_qi
		bool enableQISaarthal = true;			//Saarthal//									?:? flag_enable_qi
		bool enableQIBrelyna = true;			//Brelyna's Practice//							?:? flag_enable_qi
		bool enableQIDwemerMuseum = true;		//Dwemer Museum//								?:? flag_enable_qi
		bool enableQITrinityRestored = true;	//Trinity Restored//							?:? flag_enable_qi
		bool enableQIThalmorEmbassy = true;		//Thalmor Embassy//								?:? flag_enable_qi
		bool enableQIRingmaker = true;			//Ringmaker//									?:? flag_enable_qi
		
		//Page Misc
		//Flag flag_ft_restricted				//restrictFastTravel
		//Flag flag_wearingOppDevice			//!WearingOppressiveDevice()
		bool oppDeviceAntiCheat = true;			//Lock Oppressive Devices Menu//Lock the Oppressive Devices menu when wearing an oppressive device.		?:? flag_wearingOppDevice
		bool generalDeviceAntiCheat = false;	//Lock Menus While Restrained//All menus will be disabled while you are wearing any devices.
		bool noMessageBoxes = false;			//Remove Message Boxes//No message boxes will be shown.
		bool eventVisualEffects = true;			//Event Visual Effects//Will apply a visual effect to the player when an event happens.
		bool surpriseMode = false;				//Surprise Mode//Will disable most message boxes when an event happens.\nYou need to figure out what changed!
		float rDeviceBaseChance = 1.5f;			//Device Base Chance//Chance to loot a random bondage item from a container or a dead body.//{1}%//(0,100,0.1)
		float rDeviceLuckyBonus = 2.5f;			//Device Lucky Bonus//Device base chance will be multiplied by this value if you are lucky.//{1}//(1,10,0.1)
		float wardingCharmChance = 0.5f;		//Warding Charm Chance//Chance to find a warding charm. These charms will stop a curse from affecting you!\nThe more you have, the less likely you are to find more.//{2}%//(0,10,0.01)
		bool dragonHoard = true;				//Dragon Hoards//Dragons will drop more gold, but the gold they carry is likely to be cursed.\nWith this enabled some settings might be ignored when looting dragons.
		bool bossExtraGold = true;				//Boss Chest Extra Gold//Boss chests will have extra gold.
		bool useThemes = false;					//Use Device Themes//Events that equip the player with devices will try to keep all devices equipped to a consistent theme.\nWARNING: this will increase the time taken to run each event and may cause lag spikes.
		//Column
		keycode setDebugKey = -1;				//Debug Menu Key//
		bool disableForce3rdPerson = true;		//Disable 3rd Person//This mod will not force the player into 3rd person.
		bool enableSlowStrip = false;			//Use Sexlab Strip//Replace the built in stripping algorithm with the one from sexlab.\nCan fix rare cases of crashing on stripping and also give more control over what gets stripped.
		float tatSolventChance = 0.5f;			//Universal Solvent Chance//Chance to find universal solvent when looting dead bodies. Universal solvent will remove all lewd marks and tattoos.\nHaving more tattoos will slightly increase the chance of finding one.\nSet to 0 to disable.//{2}%//(0,100,0.01)
		float arousalPotionChance = 5.0f;		//Frigid Wash Chance//Chance to find a potion that will reduce your arousal. The chance increases slightly with high arousal.\nYou can only cary a max of 3 at a time.//{2}%//(0,100,0.01)
		float arousalPotionLibido = 10.0f;		//Frigid Wash Libido//How much your libido will decrease when drinking a frigid wash. Only works if OSLAroused is installed.//{1}//(0,100,0.1)
		bool restrictFastTravel = true;			//Restrict Fast Travel//You will be unable to fast travel when wearing heavy bondage, hobbling devices, boots, and blindfolds.								**RELOAD
		bool restrictFastTravelPart = false;	//Partial Restrict Fast Travel//You will be unable to fast travel when wearing any visible devices.															?:? flag_ft_restricted
		bool restrictFastTravelFull = false;	//Fully Restrict Fast Travel//You will be unable to fast travel when wearing any devices.																	?:? flag_ft_restricted
		bool useDeviceNamesInThemes = true;		//Use Device Names in Themes//Device names will be used for themes along with editor ids. Should be disabled if using mods translated to languages other than english.
		bool saveDependentSettings = false;		//Save Dependent Settings//If enabled your settings for this save will not be synced with all of your other saves.

		//Page Advanced Nudity Detection
		//Header Nudity Sex
		bool ANDSexTopless = true;				//Topless Trigger//Character will be considered nude when topless.																?:? flag_AND
		bool ANDSexBottomless = true;			//Bottomless Trigger///Character will be considered nude when bottomless.														?:? flag_AND
		bool ANDSexShowingChest = true;			//Chest Trigger///Character will be considered nude when showing chest.															?:? flag_AND
		bool ANDSexShowingGenitals = true;		//Genitals Trigger///Character will be considered nude when showing genitals.													?:? flag_AND
		bool ANDSexShowingAss = false;			//Ass Trigger///Character will be considered nude when showing ass.																?:? flag_AND
		bool ANDSexShowingBra = false;			//Bra Trigger///Character will be considered nude when showing bra.																?:? flag_AND
		bool ANDSexShowingUnderwear = false;	//Underwear Trigger///Character will be considered nude when showing underwear.													?:? flag_AND
		//Column
		//Header Nudity Consequence
		bool ANDConsTopless = true;				//Topless Trigger///Character will be considered nude when topless.																?:? flag_AND
		bool ANDConsBottomless = true;			//Bottomless Trigger///Character will be considered nude when bottomless.														?:? flag_AND
		bool ANDConsShowingChest = true;		//Chest Trigger///Character will be considered nude when showing chest.															?:? flag_AND
		bool ANDConsShowingGenitals = true;		//Genitals Trigger///Character will be considered nude when showing genitals.													?:? flag_AND
		bool ANDConsShowingAss = false;			//Ass Trigger///Character will be considered nude when showing ass.																?:? flag_AND
		bool ANDConsShowingBra = false;			//Bra Trigger///Character will be considered nude when showing bra.																?:? flag_AND
		bool ANDConsShowingUnderwear = false;	//Underwear Trigger///Character will be considered nude when showing underwear.													?:? flag_AND

		//Page Consequences
		//Flag flag_enable_cons					//consEnable
		//Flag flag_enable_cons_rt				//consEnable && ESP:RapeTattoos.esp && CheckSTNG()
		bool consEnable = true;					//Enabled//Enables all consequences.																																										**RELOAD
		//Header Dialogue
		float consTalkNude = 35.0f;				//Nudity//Chance for a consequence when talking to someone while nude.//{1}%//(0,100,0.1)																													?:? flag_enable_cons
		float consTalkRestrained = 10.0f;		//Restrained//Chance for a consequence when talking to someone while you have visible restraints.//{1}%//(0,100,0.1)																						?:? flag_enable_cons
		float consTalkHeavy = 50.0f;			//Heavy Restraints//Chance for a consequence when talking to someone while in heavy restraints.//{1}%//(0,100,0.1)																							?:? flag_enable_cons
		float consTalkTattoo = 25.0f;			//Tattoos//Chance for a consequence when talking to someone while covered in tattoos.//{1}%//(0,100,0.1)																									?:? flag_enable_cons
		//Header Scans
		float consScanSex = 25.0f;				//Sex//Chance for a consequence after someone sees you having sex.\nThis is checked at the end of the scene, not on the actor scan.//{1}%//(0,100,0.1)														?:? flag_enable_cons
		float consScanNude = 10.0f;				//Nudity//Chance for a consequence when someone sees you naked.//{1}%//(0,100,0.1)																															?:? flag_enable_cons
		float consScanRestrained = 0.0f;		//Restrained//Chance for a consequence when someone sees you with visible restraints.//{1}%//(0,100,0.1)																									?:? flag_enable_cons
		float consScanHeavy = 25.0f;			//Heavy Restraints//Chance for a consequence when someone sees you wearing heavy restraints.//{1}%//(0,100,0.1)																								?:? flag_enable_cons
		float consScanTattoo = 5.0f;			//Tattoos//Chance for a consequence when someone sees you covered in tattoos.//{1}%//(0,100,0.1)																											?:? flag_enable_cons
		float consScanTrespass = 100.0f;		//Trespass//Chance for a consequence when someone catches you trespassing.//{1}%//(0,100,0.1)																												?:? flag_enable_cons
		int consScanRadius = 650;				//Scan Radius//How close someone must be to the player to see them.//{0}//(100,5000,50)																														?:? flag_enable_cons
		int consScanInterval = 5;				//Scan Interval//How often the actor search happens in seconds.//{0}//(1,120,1)																																?:? flag_enable_cons
		//Header Guards
		bool consGuardNormal = false;			//Guards are Normal//Include guards in the normal consequences above.\nOtherwise they will only be able to fine you.																						?:? flag_enable_cons
		bool consFineGuardFaction = true;		//Use Guard Faction//Use guard faction for guard selection.\nThis will treat more characters as guards, including people like Commander Caius and Kodlak.\nOtherwise it will only use actors with "Guard" in their name.	?:? flag_enable_cons
		float consGuardScanSex = 50.0f;			//Sex//Chance for a consequence after a guard sees you having sex.\nThis is checked at the end of the scene, not on the actor scan.\nSet to 0 to disable this fine.//{1}%//(0,100,0.1)						?:? flag_enable_cons
		float consGuardScanNude = 0.0f;			//Nudity//Chance for a consequence when a guard sees you naked.\nSet to 0 to disable this fine.//{1}%//(0,100,0.1)																							?:? flag_enable_cons
		float consGuardScanRestrained = 0.0f;	//Restrained//Chance for a consequence when a guard sees you with visible restraints or heavy restraints.\nSet to 0 to disable this fine.//{1}%//(0,100,0.1)												?:? flag_enable_cons
		float consGuardScanTattoo = 0.0f;		//Tattoos//Chance for a consequence when a guard sees you covered in tattoos.\nSet to 0 to disable this fine.//{1}%//(0,100,0.1)																			?:? flag_enable_cons
		float consGuardTalkMult = 5.0f;			//Talk Multiplier//Multiplier for chances if talking to a guard.\nSet this to 100 to guarantee a fine when talking to a guard while breaking a rule.//{1}//(0,100,0.1)										?:? flag_enable_cons
		int consFineForSex = 250;				//Sex Fine//What is the fine for having sex?//{0}//(50,1000,10)																																				?:? flag_enable_cons
		int consFineForNudity = 100;			//Nudity Fine//What is the fine for being naked?//{0}//(50,1000,10)																																			?:? flag_enable_cons
		int consFineForRestraints = 100;		//Restrained Fine//What is the fine for being restrained?//{0}//(50,1000,10)																																?:? flag_enable_cons
		int consFineForTattoos = 100;			//Tattoo Fine//What is the fine for being covered in tattoos?//{0}//(50,1000,10)																															?:? flag_enable_cons
		//Column
		bool consFallthrough = false;			//Try All Consequences//With this enabled if a consequence can't trigger it will try all the other consequences.\nThis will make it so that consequences are more likely to happen, but consequences with low weights might also be more common.		?:? flag_enable_cons
		//Header Toggles
		bool consAllowFollowers = false;		//Allow Followers//Followers can be the source of a consequence.																																			?:? flag_enable_cons
		bool consFollowersBlock = false;		//Followers Prevent Consequence//If you have any followers you will not be able to get any consequences.\nDesigned for mods like Devious Followers.															?:? flag_enable_cons
		bool consTolerableChastity = true;		//Tolerable Chastity//Chastity will be considered tolerable when checking for nudity.																														?:? flag_enable_cons
		bool consUseRelationships = true;		//Use Relationships//The relationship rank of the target actor will affect how they treat you. You are less likely to see all consequences except mercy when the relationship is better.					?:? flag_enable_cons
		bool consRelationBondage = false;		//Relationship Bondage//Bondage is treated as a good outcome for Relationships, making it more common with high disposition.																				?:? flag_enable_cons
		int consRestrainedCount = 5;			//Number of Restraints//How many visible restraints you must have for the Restrained consequence triggers.//{0}//(1,10,1)																							?:? flag_enable_cons
		int consTattooCount = 8;				//Number of Tattoos//How many tattoos you must have for the Tattoo consequence triggers.//{0}//(1,10,1)																							?:? flag_enable_cons
		float consGaggedMultiplier = 1.2f;		//Gagged Multiplier//Your consequence chance when talking to someone is multiplied by this if you are gagged.//{1}x//(0,5,0.1)																				?:? flag_enable_cons
		//Header Results
		int consSexWeight = 15;					//Sex Weight//Chance for an actor to have sex with the player.\nFollows the Allowed Actors set on the Sex page.//{0}//(0,100,1)																				?:? flag_enable_cons
		//Empty
		int consRandomBondageWeight = 10;		//Random Bondage Weight//Chance for a random item to be equipped.//{0}//(0,100,1)																		?:? flag_enable_cons
		bool consRandomHeavyBondage = false;	//Allow Heavy Bondage//Allow for heavy bondage to be equipped.																																				?:? flag_enable_cons
		bool consBondageIgnoreMax = false;		//Ignore Restraint Cap//Will ignore the restraint cap set on the main page.																																					
		//Empty
		int consTattooWeight = 10;				//Tattoo Weight//Chance for you to receive a tattoo.\nWill not trigger as a sex consequence.\nMaximum tattoos is the same as Tattoo Curse Cap.//{0}//(0,100,1)												?:? flag_enable_cons_rt
		bool consTattooCurseNumbers = false;	//Use Tattoo Curse//Will give the player multiple tattoos instead of one.\nThis uses the Min and Max set for the Tattoo Curse.																				?:? flag_enable_cons_rt
		//Empty
		int consMercyWeight = 5;				//Mercy Weight//Chance that a character will feel bad for you and unlock a device, give you a key, or give you something mildly useful.//{0}//(0,100,1)														?:? flag_enable_cons
		

		//Page Sex
		//Flag flag_enable_sex					//sexEnabled
		//Flag flag_enable_random_sex			//sexRandomEnabled
		//Flag flag_sex_slave_tats				//sexRandomEnabled && CheckSTNG()
		//Header General
		bool sexEnabled = true;					//Enabled//Toggles sex on or off.\nSex will only occur from friendly characters.																								**RELOAD
		bool sexAggressiveAnims = false;		//Prefer Aggressive Animations//Prefer using aggressive animations for all sex started by this mod.																				?:? flag_enable_sex
		bool sexFilterDevices = true;			//Filter Animations By Device//Will use animation tags to filter animations based on devices worn.																				?:? flag_enable_sex
		bool sexFilterFuta = true;				//Filter Futa Animations//This will prevent FF tagged animations for female & futa and prevent MF for female & female.\nOnly works for sexlab SE and not Sexlab P+				?:? flag_enable_sex
		//Empty
		//Empty
		//Header Random Sex
		bool sexRandomEnabled = false;			//Random Sex//Characters that you encounter on your journey might have sex with you!																							**RELOAD
		int sexCooldown = 60;					//Cooldown//How long in seconds after a scene ends before another can trigger.//{0}//(5,1000,5)																					?:? flag_enable_random_sex
		bool sexRandAllowFollowers = true;		//Allow Followers//Followers will be allowed in random scenes.																													?:? flag_enable_random_sex
		bool sexRandAllowNPCs = false;			//Allow NPCs//NPCs that are not followers will be allowed in random scenes.																										?:? flag_enable_random_sex
		bool sexRandAllowSummons = true;		//Allow Summons//Summons will be allowed in random scenes.\nThis includes summons of you and any of your followers.																?:? flag_enable_random_sex
		bool sexRandAllowCreatures = false;		//Allow Creatures//Creatures that are not summons or followers will be allowed in random scenes.																				?:? flag_enable_random_sex
		float sexBaseChance = 1.0f;				//Base Chance//How likely a potential actor will have sex with the player.\nRecommended to be less than 5, or else you might end up having a LOT of sex.//{1}%//(0,100,0.1)		?:? flag_enable_random_sex
		//Header Modifiers
		float sexCreatureModifier = 0.9f;		//Creature Modifier//Modifier for creatures.//{1}//(0,100,0.1)																													?:? flag_enable_random_sex
		float sexFollowerModifier = 1.0f;		//Follower Modifier//Modifier for followers.//{1}//(0,100,0.1)																													?:? flag_enable_random_sex
		float sexSummonModifier = 2.0f;			//Summon Modifier//Modifier for summons.//{1}//(0,100,0.1)																														?:? flag_enable_random_sex
		float sexRestraintsModifier = 1.1f;		//Restraints Modifier//Modifier for each visible restraint you have.\nThis is multiplicative, multiplying yur chance for each device.//{2}//(0,2,0.01)							?:? flag_enable_random_sex
		float sexSafeLocationModifier = 1.3f;	//Safe Location Modifier//Modifier for safe locations like a town or player home.//{1}//(0,100,0.1)																				?:? flag_enable_random_sex
		float sexUnsafeLocationModifier = 0.1f;	//Dangerous Location Modifier//Modifier for dangerous locations like a bandit camp or dwemer ruin.//{1}//(0,100,0.1)															?:? flag_enable_random_sex
		float sexPlayerNudityModifier = 2.0f;	//Player Nudity Modifier//Modifier if the player is nude.//{1}//(0,100,0.1)																										?:? flag_enable_random_sex
		float sexNPCNudityModifier = 2.0f;		//NPC Nudity Modifier//Modifier if the NPC is nude.//{1}//(0,100,0.1)																											?:? flag_enable_random_sex
		float sexPlayerArousalModifier = 2.5f;	//Player Arousal Modifier//Modifier for player arousal.\nWill have max effect at arousal 100, and no effect below the Arousal Cutoff//{1}//(0,100,0.1)							?:? flag_enable_random_sex
		float sexNPCArousalModifier = 2.5f;		//NPC Arousal Modifier//Modifier for npc arousal.\nWill have max effect at arousal 100, and no effect below the Arousal Cutoff//{1}//(0,100,0.1)								?:? flag_enable_random_sex
		int sexArousalModifierCutoff = 80;		//Arousal Cutoff//Lower bound for the Arousal Modifier.//{0}//(0,99,1)																											?:? flag_enable_random_sex
		float sexAllureModifier = 5.0f;			//Allure Modifier//Modifier if the player has the Allure Mark.//{1}//(0,100,0.1)																								?:? flag_enable_random_sex
		float sexSummonerModifier = 7.5f;		//Summoner Collar Modifier//Modifier for summons if the player is wearing the Summoner Collar.//{1}//(0,100,0.1)																?:? flag_enable_random_sex
		//Header Search	
		int sexScalingModifier = 5;				//Scaling Modifier//Will decrease the chance of a random scene if another has happened recently.\nThe higher the number, the longer (on average) between scenes.\nSet to 0 to disable.//{0}//(0,20,1)				?:? flag_enable_random_sex
		int sexSearchRadius = 2000;				//Search Radius//How far away can actors be from the player.//{0}//(100,10000,100)																								
		int sexSearchInterval = 15;				//Search Interval//How often the actor search happens in seconds.\nIncreasing this will result in sex less often.//{0}//(5,120,1)	
		
		//Column
		//Header Allowed Actors
		bool sexAllowMale = true;				//Allow Male Actors//Male actors will be allowed.																																?:? flag_enable_sex
		bool sexAllowFemale = true;				//Allow Female Actors//Female actors will be allowed.																															?:? flag_enable_sex
		bool sexAllowFuta = true;				//Allow Futa Actors//Futa actors will be allowed.																																?:? flag_enable_sex
		bool sexAllowCreature = false;			//Allow Creature Actors//Creature actors will be allowed.																														?:? flag_enable_sex
		bool sexAllowFarmAnimals = false;		//Allow Farm Animals//Creatures like chickens, cows, and goats will be available in scenes.																						?:? flag_enable_sex
		//Empty
		//Header Player Requirements
		bool sexRequireAllPlayer = false;		//Require All//Random sex will only happen if every player requirement is met.\nOtherwise random sex will happen if at least one player requirement is met.						?:? flag_enable_random_sex
		int sexRequiredPlayerBindings = 2;		//Bondage//Will allow sex if the player has at least this many visible restraints.\nSet to -1 to ignore this option.//{0}//(-1,100,1)											?:? flag_enable_random_sex
		bool sexRequirePlayerCollar = false;	//Collar//Will allow sex if the player is wearing a collar.																														?:? flag_enable_random_sex
		bool sexRequirePlayerHeavy = false;		//Heavy Restraints//Will allow sex if the player is wearing heavy restraints.																									?:? flag_enable_random_sex
		bool sexRequirePlayerNude = true;		//Nudity//Will allow sex if the player is naked.																																?:? flag_enable_random_sex
		int sexRequiredPlayerArousal = 85;		//Arousal//Will allow sex if the player's arousal is above the set number.\nSet to -1 to ignore player arousal.//{0}//(-1,100,1)												?:? flag_enable_random_sex
		int sexRequiredPlayerTattoos = -1;		//Tattoos//Will allow sex if the player has at least this many tattoos.\nSet to -1 to ignore player tattoos.//{0}//(-1,20,1)													?:? flag_sex_slave_tats
		//Header NPC Requirements
		bool sexRequireAllNPC = false;			//Require All//Random sex will only happen if every NPC requirement is met.\nOtherwise random sex will happen if at least one npc requirement is met.							?:? flag_enable_random_sex
		int sexRequiredNPCBindings = -1;		//Bondage//Will allow sex if the player has at least this many visible restraints.\nSet to -1 to ignore this option.//{0}//(-1,100,1)											?:? flag_enable_random_sex
		bool sexRequireNPCCollar = false;		//Collar//Will allow sex if the NPC is wearing a collar.																														?:? flag_enable_random_sex
		bool sexRequireNPCNude = true;			//Nudity//Will allow sex if the NPC is naked.																																	?:? flag_enable_random_sex
		int sexRequiredNPCArousal = 85;			//Arousal//Will allow sex if the NPC's arousal is above the set number.\nSet to -1 to ignore NPC arousal.//{0}//(-1,100,1)														?:? flag_enable_random_sex
		int sexRequiredNPCTattoos = -1;			//Tattoos//Will allow sex if the NPC has at least this many tattoos.\nSet to -1 to ignore NPC tattoos.//{0}//(-1,20,1)															?:? flag_sex_slave_tats
		//Header Creature Requirements
		int sexRequiredCreatureArousal = 75;	//Arousal//Will allow sex if the creature's arousal is above the set number.\nSet to -1 to ignore creature arousal.//{0}//(-1,100,1)											?:? flag_enable_random_sex
		//Header Requirement Exceptions			
		bool sexRequireBoth = true;				//Use Player And NPC Requirements//Random sex will only happen if both the player and npc meet their requirements.\nOtherwise only one of the two will need to.					?:? flag_enable_random_sex
		bool sexFollowerBypassPlayer = false;	//Followers Ignore Player Requirements//Followers ignore player requirements.																									?:? flag_enable_random_sex
		bool sexFollowerBypassNPC = true;		//Followers Ignore NPC Requirements//Followers ignore NPC requirements.																											?:? flag_enable_random_sex
		bool sexSpouseBypassPlayer = true;		//Spouse Ignores Player Requirements//Your spouse will ignore player requirements.																								?:? flag_enable_random_sex
		bool sexSpouseBypassNPC = true;			//Spouse Ignores NPC Requirements//Your spouse will ignore NPC requirements.																									?:? flag_enable_random_sex
		bool sexSummonBypassPlayer = false;		//Summons Ignore Player Requirements//Your summons will ignore player requirements.																								?:? flag_enable_random_sex
		bool sexSummonBypassNPC = false;		//Summons Ignore NPC Requirements//Your summons will ignore NPC requirements.																									?:? flag_enable_random_sex


		//Page Help
		bool ModSuspended = false;				//Suspend Mod//Will pause all interactions of the mod. This can be enabled by other mods on mod event.																	
		bool setAllDefaultSettings = false;		//Return to Default [WARNING]//If you exit the menu with this enabled all settings in the MCM will be reset to default.
		//Column
		
		//Header Optional Requirements Check
		//Header Tattoo Requirements
		bool helpSlaveTatsNGInstalled = false;	//Slave Tats NG//Slave Tats NG is required for this mod to work.\nMUST BE THE NG VERSION OF SLAVE TATS\nLink to the correct version is on this mods download page.		**NO_OPT
		bool helpLewdMarksInstalled = false;	//Lewd Marks//Lewd marks must be installed for Slave Tats Without the Glow Patch.\nINSTALLING WITH GLOW PATCH WILL BREAK SLAVE TATS NG									**NO_OPT
		bool helpRapeTattoosInstalled = false;	//Rape Tattoos//Rape tattoos is required for the random tattoo event, and is not required for lewd marks.																**NO_OPT
		//Header Other Integrations
		bool helpOSLArousedInstalled = false;	//OSL Aroused//The arousal system that this mod is designed around.\nVersion 6.0+ is required for full functionality.\nOther systems can be used, as long as they work with the SLA API\nSLO Aroused NG 3.3.0+ is used through its own API.	**NO_OPT
		bool helpUDInstalled = false;			//Unforgiving Devices//Remember to install the UD patch in the MCM.																										**NO_OPT
		bool helpQuickLootInstalled = false;	//Quick Loot IE//Allows for events to be triggered when looting an item from a container.																				**NO_OPT
		bool helpDWDInstalled = false;			//Devious Wicked Devices//Adds event to equip a wicked device. Follows all distrobution settings from the Devious Wicked Devices MCM page.								**NO_OPT
		bool helpANDInstalled = false;			//Advanced Nudity Detection//Allows the nudity trigger for consequences and the naked lewd mark to be controlled by AND keywords.										**NO_OPT
		bool helpCockCagesWorking = false;		//Chastity Cages//Allows for cock cages to be equipped onto characters.\nShould work with most mods that add cages, but the Vivi's patch linked on LL is recommended.\nRequires Use Gendered Chastity to be enabled in the Devices Page.	**NO_OPT


		//Page LOCKED
		//Header MCM is Locked!
		//Header Remove all of your restraints to unlock.
		//MCM_END
	};

	void PlayerMessage(Translator trans, bool useSurpriseMode = false);

	void ResetMCMSettings();

	void SaveMCMSettings();

	bool LoadSettingsFile();

	void MCMSetHelpPageOptions();

	void PushSettingsToMCM();

	void GetSettingsFromMCM();

	bool PapyrusFunctionsSettigns(RE::BSScript::IVirtualMachine* ivm);

}