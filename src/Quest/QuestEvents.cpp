// Patch-build stub: the Quest sources are not in the public repository, and the
// Devious Curses - Quests.esp they drive is not part of the public release.
#include "Quest/QuestEvents.h"

namespace DCURSES::Quest {
	bool CheckObjectActivation(RE::TESObjectREFR*) {
		return false;
	}

	void CheckQuestStageChanged(RE::FormID, uint16_t) {}

	void OnLocationChanged() {}

	void OnGameLoaded() {}

	void OnSettingsChanged() {}
}
