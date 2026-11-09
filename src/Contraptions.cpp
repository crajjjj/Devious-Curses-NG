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
		std::vector<RE::TESObjectREFR*> nearby;
		RE::TES::GetSingleton()->ForEachReferenceInRange(actor, 500, [&](RE::TESObjectREFR* refr) {
			nearby.push_back(refr);
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
		// for good, and the update loop then skips every refresh as if a scene were running. Two misses in a row
		// before acting, so a lock or unlock in progress is never mistaken for a stale faction.
		static int misses = 0;
		auto player = RE::PlayerCharacter::GetSingleton();
		auto faction = StaticDataHolder::GetSingleton()->LookupForm<RE::TESFaction>(ZADC_IN_CONTRAPTION_FACTION, "Devious Devices - Contraptions.esm");
		if (!faction || !player->IsInFaction(faction) || GetContraptionForActor(player)) {
			misses = 0;
			return;
		}
		if (++misses < 2) {
			return;
		}
		misses = 0;
		log::warn("Player has the contraption faction, but no contraption holds the player. Removing the faction.");
		player->RemoveFromFaction(faction);
	}
}