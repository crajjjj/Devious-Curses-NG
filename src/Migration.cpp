#include "Migration.h"

#include "Settings.h"
#include "form_ids.h"

using namespace SKSE;

namespace DCURSES {
	bool needsMigrationFor(const std::string& current, const std::string& target) {
		// Zeroed: a version with fewer than four parts leaves the stream at EOF, and the missing parts are never written.
		int first[4] = {};
		std::istringstream parser(current);
		parser >> first[0];
		for (int idx = 1; idx < 4; idx++)
		{
			parser.get(); //Skip period
			parser >> first[idx];
		}

		int last[4] = {};
		std::istringstream parserl(target);
		parserl >> last[0];
		for (int idx = 1; idx < 4; idx++)
		{
			parserl.get(); //Skip period
			parserl >> last[idx];
		}

		return std::lexicographical_compare(first, first + 4, last, last + 4);
	}

	void VersionMigrate() {
		auto settings = Settings::GetSingleton();
		auto version = settings->lastLoadedVersion;

		if (needsMigrationFor(version, "0.9.0")) {
			log::info("Performing migration for version 0.9.0");
			if (settings->sexRequiredPlayerArousal == 0) {
				settings->sexRequiredPlayerArousal = -1;
			}
			if (settings->sexRequiredPlayerTattoos == 0) {
				settings->sexRequiredPlayerTattoos = -1;
			}

			settings->debugMode = false;
		}

		settings->lastLoadedVersion = DCURSES_VERSION;
	}
}