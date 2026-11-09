// Patch-build stub: the Quest sources are not in the public repository. The Quest MCM
// lives in Devious Curses - Quests.esp, which the public release does not ship.
#include "Quest/QuestSettings.h"

namespace DCURSES::Quest {
	void PlayerMessage(Translator, bool) {}

	void ResetMCMSettings() {}

	void SaveMCMSettings() {}

	bool LoadSettingsFile() {
		return false;
	}

	void PushSettingsToMCM() {}

	bool PapyrusFunctionsSettigns(RE::BSScript::IVirtualMachine*) {
		return true;
	}
}
