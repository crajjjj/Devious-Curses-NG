// Patch-build stub: the Quest sources are not in the public repository. Keeps the QDAT
// cosave record round-tripping unchanged so a save returns to the official DLL intact.
#include "Quest/Serialized.h"

using namespace SKSE;

namespace DCURSES::Quest {
	QuestData* QuestData::GetSingleton() {
		static QuestData instance;
		return &instance;
	}

	void QuestData::revert() {
		std::memset(static_cast<void*>(this), 0, sizeof(QuestData));
	}

	void SerdeOnGameSaved(SKSE::SerializationInterface* serde) {
		if (!serde->OpenRecord(QuestDataRecord, 0)) {
			log::error("Unable to open record to write quest cosave data.");
			return;
		}
		serde->WriteRecordData(QuestData::GetSingleton(), sizeof(QuestData));
	}

	void SerdeOnRecordLoaded(SKSE::SerializationInterface* serde, std::uint32_t type, std::uint32_t size, std::uint32_t) {
		if (type != QuestDataRecord) {
			return;
		}
		if (size == sizeof(QuestData)) {
			serde->ReadRecordData(QuestData::GetSingleton(), sizeof(QuestData));
		}
		else {
			log::warn("Error reading quest data, quest state reset.");
		}
	}

	void SerdeOnRevert(SKSE::SerializationInterface*) {
		QuestData::GetSingleton()->revert();
	}
}
