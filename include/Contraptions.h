#pragma once

namespace DCURSES {
	constexpr auto ZADC_CONTRAPTIONS = { 0x12c7, 0x1d96, 0x5904, 0x63f7, 0x63f9, 0x6963, 0x7db9, 0x7ef5, 0x8324, 0x8325, 0x845d, 0x89c4, 0xd997, 0xe9e6, 0xe9e7, 0xe9e8, 0xe9e9, 0xe9ea, 0x1001f, 0x1002a, 0x10033, 0x1003d, 0x10047 };

	RE::TESObjectACTI* GetRandomContraption();

	RE::TESObjectREFR* GetContraptionForActor(RE::Actor* actor);

	RE::TESObjectACTI* CreateAndLockContraption(RE::Actor* actor);

	constexpr auto ZADC_IN_CONTRAPTION_FACTION = 0x097E0A;

	// Main thread only. Removes the "in contraption" faction from the player when no contraption holds them.
	void ClearStaleContraptionFaction();
}