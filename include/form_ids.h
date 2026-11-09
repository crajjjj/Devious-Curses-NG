#pragma once

using namespace SKSE;

namespace DCURSES {
constexpr auto DCURSES_VERSION = "0.9.0.1";

// SUMMONS
constexpr auto SUMMON_FAMILIAR = 0x640B5;
constexpr auto SUMMON_ATRONACH_FLAME = 0x204C0;
constexpr auto SUMMON_ATRONACH_FROST = 0x204C1;
constexpr auto SUMMON_ATRONACH_STORM = 0x204C2;
constexpr auto SUMMON_DREMORA_LORD = 0x10DDEE;
constexpr auto DCURSES_COLLAR_SUMMONED = 0x80D;

// DCURSES
constexpr auto TATTOO_CHARM = 0x809;
constexpr auto VOLATILE_GEM = 0x839;
constexpr auto AROUSAL_POTION = 0x83B;
constexpr auto WARDING_CHARM = 0x845;
constexpr auto DCURSES_MCM = 0x800;
constexpr auto DCURSES_LIB = 0x804;

// DEVICES
constexpr auto SUMMONER_COLLAR = 0x810;
constexpr auto SUMMONER_COLLAR_R = 0x80A;
constexpr auto LIVING_LATEX = 0x812;
constexpr auto LIVING_LATEX_R = 0x813;
constexpr auto LIVING_LATEX_OPEN = 0x819;
constexpr auto LIVING_LATEX_OPEN_R = 0x81A;
constexpr auto DWARVEN_CURIAS = 0x828;
constexpr auto DWARVEN_CURIAS_R = 0x81B;
constexpr auto DWARVEN_CURIAS_HEAVY = 0x82E;
constexpr auto DWARVEN_CURIAS_HEAVY_R = 0x82D;
constexpr auto MADNESS_PLUG = 0x82F;
constexpr auto MADNESS_PLUG_R = 0x82A;
constexpr auto MADNESS_PIERCINGS = 0x834;
constexpr auto LUCKY_PIERCINGS = 0x83E;
constexpr auto LUCKY_PIERCINGS_R = 0x83D;
constexpr auto NOCTURNAL_PIERCING = 0x841;
constexpr auto NOCTURNAL_PIERCING_R = 0x840;

// KEYS
constexpr auto MAGIC_KEY = 0x806;
constexpr auto SUMMONER_COLLAR_KEY = 0x807;

// SPELLS
constexpr auto MGEF_SPELL = 0x817;
constexpr auto SUMMONER_SUMMON_SPELL = 0x801;
constexpr auto HEALSLUT_RESTORATION_SPELL = 0x83A;
constexpr auto NOCTURNAL_INVIS_SPELL = 0x843;

// EFFECTS
constexpr auto ALLURE_EFFECT = 0x80B;
constexpr auto HEAT_EFFECT = 0x805;
constexpr auto NUDITY_EFFECT = 0x80F;
constexpr auto BONDAGE_EFFECT = 0x811;
constexpr auto BRANDING_EFFECT = 0x808;
constexpr auto HEALSLUT_EFFECT = 0x835;
constexpr auto PAINSLUT_EFFECT = 0x80D;
constexpr auto SUMMONER_COLLAR_EFFECT = 0x803;
constexpr auto SOMMONER_COLLAR_SUMMON_EFFECT = 0x802;
constexpr auto LIVING_LATEX_EFFECT = 0x816;
constexpr auto DWARVEN_CUIRASS_EFFECT = 0x82B;
constexpr auto MADNESS_PLUG_EFFECT = 0x833;
constexpr auto AROUSAL_POTION_EFFECT = 0x83C;
constexpr auto NOCTURNAL_PIERCING_EFFECT = 0x846;
constexpr auto NOCTURNAL_INVIS_EFFECT = 0x844;

// PERKS
constexpr auto MGEF_CONTROLLER = 0x818;
constexpr auto HEALSLUT_PERK = 0x837;

// DDX
constexpr auto DDX_RUSTY_PEAR_ANAL_SIGN = 0x1FC45;
constexpr auto DDX_RUSTY_PEAR_VAGINAL = 0x1FC4F;
constexpr auto DDX_STEEL_YOKE = 0x3D2E1;
constexpr auto DDX_RED_ARMBINDER = 0x110F2;
constexpr auto DDX_RED_CATSUIT = 0x3D8fC;
constexpr auto DDX_PUPPY_COLLAR = 0x4F916;

constexpr auto DDX_GOLDEN_CUFFS_ARMS = 0x4000B;
constexpr auto DDX_GOLDEN_CUFFS_LEGS = 0x4000D;
constexpr auto DDX_GOLDEN_COLLAR = 0x4000F;

// UD
constexpr auto UD_ABADONPLUG_INVENTORY = 0x135DDE;
constexpr auto UD_ABADONPLUGANAL_INVENTORY = 0x13B43C;
const std::vector<uint32_t> UD_ARMORSET01 = {
	0x14FAB,
	0x14FA0,
	0x14FBA,
	0x14FA7,
	0x14FA5,
	0x14FAD,
	0x15050,
	0x1504E,
};
const std::vector<uint32_t> UD_ARMORSET02 = {
	0x14FDC,
	0x14FDF,
	0x14FD0,
	0x14FDE,
	0x14FD2,
	0x14FDA,
	0x15052,
};
const std::vector<uint32_t> UD_ARMORSET03 = {
	0x14FE5,
	0x14FED,
	0x14FE7,
	0x14FE9,
	0x1504A,
	0x1504C,
};

// OTHER
constexpr auto CHASTITY_PIERCING_GOLD8 = 0x803;

	class StaticDataHolder {
	private: 
		std::mutex mutex = std::mutex();
		std::map<std::pair<uint32_t, std::string>, RE::TESForm*> _internalData;
		std::map<std::string, const RE::TESFile*> _modFiles;
		StaticDataHolder() {}
	public:
		StaticDataHolder(StaticDataHolder const&) = delete;
		void operator=(StaticDataHolder const&) = delete;

		static StaticDataHolder* GetSingleton() {
			static StaticDataHolder instance;
			return &instance;
		}

		RE::TESForm* LookupFormNoCache(uint32_t a_localFormID, std::string modname) {
			return RE::TESDataHandler::GetSingleton()->LookupForm(a_localFormID, modname);
		}

		template<class T>
		T* LookupFormNoCache(uint32_t a_localFormID, std::string modname) {
			RE::TESForm* form = LookupFormNoCache(a_localFormID, modname);
			return (form && form->Is(T::FORMTYPE)) ? static_cast<T*>(form) : nullptr;
		}

		RE::TESForm* LookupForm(uint32_t formid, std::string modname) {
			auto pair = std::make_pair(formid, modname);
			if (_internalData.count(pair)) {
				//log::trace("StaticDataHolder Returning stored data {:X} from {}", formid, modname);
				return _internalData.at(pair);
			}
			else {
				//log::trace("StaticDataHolder grabbing {:X} from {}", formid, modname);
				RE::TESForm* form = LookupFormNoCache(formid, modname);
				if (form) {
					mutex.lock();
					_internalData.insert({ pair, form });
					mutex.unlock();
				}
				return form;
			}
		}

		template<class T>
		T* LookupForm(uint32_t formid, std::string modname) {
			RE::TESForm* form = LookupForm(formid, modname);
			return (form && form->Is(T::FORMTYPE)) ? static_cast<T*>(form) : nullptr;
		}

		const RE::TESFile* LookupModByName(std::string name) {
			if (_modFiles.count(name)) {
				return _modFiles.at(name);
			}
			else {
				const RE::TESFile* file = RE::TESDataHandler::GetSingleton()->LookupModByName(name);
				if (file) {
					mutex.lock();
					_modFiles.insert({ name, file });
					mutex.unlock();
				}
				return file;
			}
		}

		void InvalidateCache() {
			mutex.lock();
			_internalData = std::map<std::pair<uint32_t, std::string>, RE::TESForm*>();
			mutex.unlock();
		}
	};
}