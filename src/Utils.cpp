#include "Utils.h"

#include "Settings.h"

namespace DCURSES::Util {

	double random() {
		return random<double>();
	}

	int ColorScale(int color, double mult) {
		int r = (color & 0xff0000) >> 4 * 4;
		int g = (color & 0x00ff00) >> 2 * 4;
		int b = color & 0x0000ff;

		int r1 = std::min(static_cast<int>(r * mult), 0xff);
		int g1 = std::min(static_cast<int>(g * mult), 0xff);
		int b1 = std::min(static_cast<int>(b * mult), 0xff);

		int result = r1 << 4 * 4 | g1 << 2 * 4 | b1;

		return result;
	}

	using _GetFormEditorID = const char* (*)(std::uint32_t);
	inline std::string GetFormEditorId(const RE::TESForm* a_form) {
		switch (a_form->GetFormType()) {
		case RE::FormType::Keyword:
		case RE::FormType::LocationRefType:
		case RE::FormType::Action:
		case RE::FormType::MenuIcon:
		case RE::FormType::Global:
		case RE::FormType::HeadPart:
		case RE::FormType::Race:
		case RE::FormType::Sound:
		case RE::FormType::Script:
		case RE::FormType::Navigation:
		case RE::FormType::Cell:
		case RE::FormType::WorldSpace:
		case RE::FormType::Land:
		case RE::FormType::NavMesh:
		case RE::FormType::Dialogue:
		case RE::FormType::Quest:
		case RE::FormType::Idle:
		case RE::FormType::AnimatedObject:
		case RE::FormType::ImageAdapter:
		case RE::FormType::VoiceType:
		case RE::FormType::Ragdoll:
		case RE::FormType::DefaultObject:
		case RE::FormType::MusicType:
		case RE::FormType::StoryManagerBranchNode:
		case RE::FormType::StoryManagerQuestNode:
		case RE::FormType::StoryManagerEventNode:
		case RE::FormType::SoundRecord:
			return std::string(a_form->GetFormEditorID());
		default:
		{
			static auto tweaks = GetModuleHandle(L"po3_Tweaks");
			static auto func = reinterpret_cast<_GetFormEditorID>(GetProcAddress(tweaks, "GetFormEditorID"));
			if (func) {
				return func(a_form->formID);
			}
			return {};
		}
		}
	}

	std::string tolower(std::string string) {
		std::transform(string.begin(), string.end(), string.begin(),
			[](unsigned char c) { return (char)std::tolower(c); });
		return string;
	}

	std::string toupper(std::string string) {
		std::transform(string.begin(), string.end(), string.begin(),
			[](unsigned char c) { return (char)std::toupper(c); });
		return string;
	}

	std::string trim(const std::string in) {
		size_t lindex = 0;
		size_t rindex = in.size() - 1;
		while (std::isspace(in[lindex])) {
			lindex += 1;
		}
		while (std::isspace(in[rindex])) {
			rindex -= 1;
		}
		return in.substr(lindex, rindex - lindex + 1);
	}

	std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
		std::vector<std::string> tokens;
		size_t index = 0;
		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter, index)) != std::string::npos) {
			token = s.substr(index, pos - index);
			tokens.push_back(token);
			index = pos + delimiter.length();
		}
		token = s.substr(index);
		tokens.push_back(token);

		return tokens;
	}

	std::string join(std::vector<std::string> list, std::string token, std::string if_empty) {
		std::string output;
		if (list.size() >= 2) {
			for (size_t i = 0; i < (list.size() - 1); i++) {
				output += list.at(i);
				output += token;
			}
		}
		if (list.size() >= 1) {
			output += list.back();
		}
		if (list.size() == 0) {
			output += if_empty;
		}

		return output;
	}

	size_t findMatchingParen(const std::string& str, size_t index) {
		int counter = 1;
		for (size_t i = index + 1; i < str.size(); i++) {
			if (str[i] == '(') counter++;
			if (str[i] == ')') counter--;
			if (counter == 0) {
				return i;
			}
		}
		return str.size() - 1;
	}

	bool testFormComp(const std::string& theme, RE::TESForm* form, bool showTrace) {
		if (showTrace) log::trace("{}", theme);
		const std::string comp = trim(theme);
		if (comp == "") return true;
		if (!form) return false;

		if (comp.contains('(')) {
			size_t i1 = comp.find('(');
			size_t i2 = findMatchingParen(comp, i1);
			std::string p1 = comp.substr(0, i1);
			std::string p2 = testFormComp(comp.substr(i1 + 1, i2 - i1 - 1), form) ? "" : "!";
			std::string p3 = comp.substr(i2 + 1);
			return testFormComp(p1 + p2 + p3, form);
		}
		else if (comp.contains('|')) {
			auto tokens = split(comp, "|");
			for (auto token : tokens) {
				if (testFormComp(token, form)) {
					return true;
				}
			}
			return false;
		}
		else if (comp.contains('&')) {
			auto tokens = split(comp, "&");
			for (auto token : tokens) {
				if (!testFormComp(token, form)) {
					return false;
				}
			}
			return true;
		}
		else if (comp.starts_with('!')) {
			return !testFormComp(comp.substr(1), form);
		}
		else {
			return
				Settings::GetSingleton()->useDeviceNamesInThemes && (tolower(form->GetName()).find(tolower(comp)) != std::string::npos) ||
				tolower(GetFormEditorId(form)).find(tolower(comp)) != std::string::npos;
		}
	}

	bool FormEditorIdContains(RE::TESForm* a_form, std::string substring) {
		return tolower(GetFormEditorId(a_form)).find(tolower(substring)) != std::string::npos;
	}

	RE::TESObjectREFR* GetClosestRefr(RE::Actor* actor) {
		if (!actor) return nullptr;
		RE::TESObjectREFR* ref = nullptr;
		actor->GetParentCell()->ForEachReference([&](RE::TESObjectREFR* refr) {
			if (actor->GetPosition().GetDistance(refr->GetPosition()) < ref->GetPosition().GetDistance(refr->GetPosition()) && actor != ref && actor->GetPosition().GetDistance(refr->GetPosition()) > 100.0) {
				ref = refr;
			}

			return RE::BSContainer::ForEachResult::kContinue;
			});

		return ref;
	}

	bool ActorIsCreature(RE::Actor* actor) {
		auto actorRace = actor->GetRace();
		//return !actorRace->HasKeywordString("ActorTypeNPC") && (actorRace->HasKeywordString("ActorTypeCreature") || actorRace->HasKeywordString("ActorTypeDwarven") || actorRace->HasKeywordString("ActorTypeAnimal"));
		auto ret = false;
		for (size_t i = 0; i < actorRace->numKeywords; i++) {
			auto keyword = actorRace->keywords[i]->formEditorID;
			if (keyword == "ActorTypeNPC") return false;
			if (keyword == "ActorTypeCreature" || keyword == "ActorTypeDwarven" || keyword == "ActorTypeAnimal") {
				ret = true;
			}
		}
		return ret;
	}

	// MUST NOT BORROW ANYTHING!
	void ExecuteWithDelay(std::chrono::milliseconds time, std::function<void()> func) {
		std::thread{ [func, time] {
			std::this_thread::sleep_for(time);
			SKSE::GetTaskInterface()->AddTask(func);
		} }.detach();
	}

	std::vector<RE::Actor*> GetFollowers() {
		std::vector<RE::Actor*> result;
		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			RE::BSSimpleList<RE::ActorHandle>* arr = &(processLists->aliveActorList);
			if (arr) {
				for (auto& actorHandle : *arr) {
					auto actorPtr = actorHandle.get();
					if (auto actor = actorPtr.get(); actor && actor->Is3DLoaded() && !actor->IsDead()) {
						if (actor->IsPlayerTeammate() && !actor->IsCommandedActor() && !ActorIsCreature(actor) && !actor->IsChild()) {
							result.push_back(actor);
						}
					}
				}
			}
		}
		return result;
	}

	std::vector<RE::Actor*> GetWatchingActors(RE::Actor* target, bool excludeFollowers, int radius, bool allowCreatures) {
		std::vector<RE::Actor*> results;
		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			RE::BSSimpleList<RE::ActorHandle>* arr = &(processLists->aliveActorList);
			if (arr) {
				for (auto& actorHandle : *arr) {
					auto actorPtr = actorHandle.get();
					auto actor = actorPtr.get();
					if (actor) {
						if (actor->Is3DLoaded() && !actor->IsDead() && !actor->IsChild()) {
							if ((actor->IsPlayerTeammate() && excludeFollowers) || actor->IsPlayerRef()) {
								continue;
							}
							if (!allowCreatures && ActorIsCreature(actor)) {
								continue;
							}
							auto actorPos = actor->GetPosition();
							auto targetPos = target->GetPosition();

							auto heading = actor->GetHeadingAngle(targetPos, true);

							if (heading < 65.0f) {
								float distance = actorPos.GetDistance(targetPos);

								// A negative radius (the default) means no distance limit; line of sight and detection still apply.
								if ((radius < 0 || distance < radius) && heading < 65.0f) {
									int detection = actor->RequestDetectionLevel(target, RE::DETECTION_PRIORITY::kCritical);
									bool los_ = false;
									bool los = actor->HasLineOfSight(target, los_);
									if (detection > 0 && los) {
										log::trace("Actor {} sees {}", actor->GetName(), target->GetName());
										results.push_back(actor);
									}
								}
							}
						}
					}
				}
			}
		}
		return results;
	}

	void StopPlayerCombat() {
		for (int i = 0; i <= 5; i++) {
			Util::ExecuteWithDelay(250ms * i, [] {
				bool didSomething = false;
				if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
					RE::BSSimpleList<RE::ActorHandle>* arr = &(processLists->aliveActorList);
					if (arr) {
						for (auto& actorHandle : *arr) {
							auto actorPtr = actorHandle.get();
							if (auto actor = actorPtr.get(); actor && actor->Is3DLoaded() && !actor->IsDead() && !actor->IsChild()) {
								if (RE::PlayerCharacter::GetSingleton()->IsCombatTarget(actor)) {
									actor->StopCombat();
									didSomething = true;
								}
							}
						}
					}
				}
				if (!didSomething) {
					RE::PlayerCharacter::GetSingleton()->StopCombat();
				}
				});
		}
	}

	void ProfileExecutionTime(std::string name, std::function<void()> func) {
		auto c1 = std::chrono::high_resolution_clock::now();
		func();
		auto c2 = std::chrono::high_resolution_clock::now();

		auto dt = (c2 - c1).count() / 1000000.0;

		log::trace("{}: {:.4f} ms", name, dt);
	}
}