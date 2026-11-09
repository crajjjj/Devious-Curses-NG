#include "Contraptions.h"

#include "Settings.h"
#include "form_ids.h"
#include "Utils.h"
#include "Scripting.h"

namespace DCURSES {
	RE::TESObjectACTI* GetRandomContraption() {
		std::vector<RE::TESObjectACTI*> contraptions;
		for (auto acti_id : ZADC_CONTRAPTIONS) {
			auto activator = StaticDataHolder::GetSingleton()->LookupForm<RE::TESObjectACTI>(acti_id, "Devious Devices - Contraptions.esm");
			if (!activator) {
				log::warn("Unable to get activator for form {:x}", acti_id);
			}
			else {
				contraptions.push_back(activator);
			}
		}

		return contraptions[Util::random(contraptions.size())];
	}

	RE::TESObjectREFR* GetContraptionForActor(RE::Actor* actor) {
		if (!actor) return nullptr;

		// Collect first: the walk holds each cell's lock, and the script lookups should not run under it.
		// Contraptions are activators; the type check keeps the script lookups off every other ref.
		std::vector<RE::TESObjectREFR*> nearby;
		RE::TES::GetSingleton()->ForEachReferenceInRange(actor, 500, [&](RE::TESObjectREFR* refr) {
			auto base = refr->GetBaseObject();
			if (base && (base->Is(RE::FormType::Activator) || base->Is(RE::FormType::Furniture))) {
				nearby.push_back(refr);
			}
			return RE::BSContainer::ForEachResult::kContinue;
		});

		for (auto refr : nearby) {
			auto scriptObject = ContraptionsGetRefScript(refr);
			if (!scriptObject) {
				continue;
			}

			RE::BSScript::Variable* user = scriptObject->GetProperty("User");
			if (user && user->Unpack<RE::Actor*>() == actor) {
				return refr;
			}
		}

		return nullptr;
	}

	RE::TESObjectACTI* CreateAndLockContraption(RE::Actor* actor) {
		auto contraption = GetRandomContraption();
		if (!Settings::GetSingleton()->disableForce3rdPerson) { ScriptingManager().ForceThirdPerson(); }
		SKSE::GetTaskInterface()->AddTask([actor, contraption]() {
			auto ptr = actor->PlaceObjectAtMe(contraption, false);
			auto object = ptr.get();
			auto settings = Settings::GetSingleton();

			auto scriptObject = ContraptionsGetRefScript(object);
			if (scriptObject) {
				scriptObject.get()->GetProperty("DestroyOnRemove")->SetBool(true);
				if (settings->eventContraptionTime > 0.0) {
					scriptObject.get()->GetProperty("isSelfBondage")->SetBool(true);
					scriptObject.get()->GetProperty("ForceTimer")->SetBool(true);
					scriptObject.get()->GetProperty("SelfBondageReleaseTimer")->SetFloat(settings->eventContraptionTime);
				}
			}

			object->data.angle = RE::NiPoint3(0, actor->GetAngleY(), actor->GetAngleZ());
			Util::ExecuteWithDelay(1500ms, [actor, object] {
				ScriptingManager().ContraptionsLockActor(actor, object);
				});
			});
		return contraption;
	}

	void ClearStaleContraptionFaction() {
		// DDC removes the faction only when it unlocks the actor, so a contraption lost any other way leaves it on
		// for good, and the update loop then skips every refresh as if a scene were running.
		// DDC disables the player's movement for as long as it holds them, so with movement disabled the faction is
		// left alone without looking further. Otherwise it takes two misses about 10 seconds apart, so a lock or
		// unlock in progress is never mistaken for a stale faction; a longer gap starts the count over.
		static std::chrono::steady_clock::time_point lastMiss{};
		auto player = RE::PlayerCharacter::GetSingleton();
		auto faction = StaticDataHolder::GetSingleton()->LookupForm<RE::TESFaction>(ZADC_IN_CONTRAPTION_FACTION, "Devious Devices - Contraptions.esm");
		auto controls = RE::ControlMap::GetSingleton();
		if (!faction || !player->IsInFaction(faction) || !controls || !controls->IsMovementControlsEnabled() || GetContraptionForActor(player)) {
			lastMiss = {};
			return;
		}
		auto now = std::chrono::steady_clock::now();
		if (lastMiss == std::chrono::steady_clock::time_point{} || now - lastMiss > 15s) {
			lastMiss = now;
			return;
		}
		lastMiss = {};
		log::warn("Player has the contraption faction, but no contraption holds the player. Removing the faction.");
		player->RemoveFromFaction(faction);
	}
}