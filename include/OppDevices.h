#pragma once

namespace DCURSES {
	constexpr int ODEVICE_TICK = 5;
	constexpr int ODEVICE_MADNESS_DIVISOR = 1000;

	class ODBitmask {
	private:
		uint32_t mask_internal;


	public:
		enum OppressiveDevice : uint32_t {
			SummonerCollar = 1 << 0,
			LivingLatex = 1 << 1,
			DwarvenCuirass = 1 << 2,
			MadnessPlug = 1 << 3,
			NocturnalPiercing = 1 << 4,
		};

		ODBitmask() {
			mask_internal = 0;
		}

		ODBitmask(uint32_t mask) {
			mask_internal = mask;
		}

		operator uint32_t() const {
			return this->mask_internal;
		}

		ODBitmask operator&(uint32_t other) {
			return ODBitmask(this->mask_internal & other);
		}

		ODBitmask operator&=(uint32_t other) {
			this->mask_internal &= other;
			return *this;
		}

		ODBitmask operator|(uint32_t other) {
			return ODBitmask(this->mask_internal | other);
		}

		ODBitmask operator|=(uint32_t other) {
			this->mask_internal |= other;
			return *this;
		}

		bool hasDevice(OppressiveDevice od) {
			return *this | od;
		}
	};

	ODBitmask GetOppDeviceMask();

	bool IsWearingOppLatex(ODBitmask device_mask = UINT32_MAX);
	bool IsWearingOppSummonerCollar(ODBitmask device_mask = UINT32_MAX);
	bool IsWearingOppMadness(ODBitmask device_mask = UINT32_MAX);
	bool IsWearingOppNocturnal(ODBitmask device_mask = UINT32_MAX);

	int RemoveDwarvenStuff();

	bool OppSummonerCollarEvent(std::string containerName);
	bool OppLivingLatexEvent(std::string containerName);
	bool OppDwarvenCuirassEvent(std::string containerName, bool skipLocation = false);
	bool OppMadnessPlugEvent(std::string containerName);
	bool OppNocturnalPiercingEvent(std::string containerName);
	bool DoOppDeviceEvent(std::string containerName);

	void OppOnSexEnd(RE::Actor* actor);

	void OppLatexMagicEvent();

	void OppDeviceOnMagicHitEvent(const RE::TESMagicEffectApplyEvent* magicEvent);

	void OppNocturnalHands(bool fromRecast = false);
	void OppNocturnalTryRecast();
	void OppNocturnalPlayerSex(const RE::BSTArray<RE::Actor*>& partners);

	void OppDoMadnessEffect(bool ending_event = false);
	void OppMadnessPlayerOrgasm();

	void OppDeviceUpdate();
}