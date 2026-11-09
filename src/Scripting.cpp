#include "Scripting.h"

#include "form_ids.h"
#include "apis/DDNG_API.h"
#include "SexLabPPFunctions.h"
#include "Settings.h"
#include "Translation.h"

#include "Quest/QuestSettings.h"

//#undef GetObject

using namespace SKSE;

namespace DCURSES {
    uint32_t ScriptingManager::refCount = 0;

    RE::BSScript::IVirtualMachine* GetVM() {
        return RE::BSScript::Internal::VirtualMachine::GetSingleton();
    }

    RE::BSScript::IObjectHandlePolicy* GetHP() {
        auto vm = GetVM();
        return vm->GetObjectHandlePolicy();
    }

    bool CheckESPLoaded() {
        RE::TESForm* form = RE::TESDataHandler::GetSingleton()->LookupForm(DCURSES_MCM, "Devious Curses.esp");
        if (form == nullptr) {
            return false;
        }
        return true;
    }

    bool CheckQuestLoaded()
    {
        RE::TESForm* form = RE::TESDataHandler::GetSingleton()->LookupForm(Quest::MCMQUEST, "Devious Curses - Quests.esp");
        if (form == nullptr) {
            return false;
        }
        return true;
    }

    bool CheckRapeTattoos() {
        RE::TESForm* form = StaticDataHolder::GetSingleton()->LookupForm(0xd62, "RapeTattoos.esp");
        if (form == nullptr) {
            return false;
        }
        return true;
    }

    bool CheckUD() {
        RE::TESForm* form = StaticDataHolder::GetSingleton()->LookupForm(0x5901, "UnforgivingDevices.esp");
        if (form == nullptr) {
            return false;
        }
        return true;
    }

    bool CheckSimpleSlavery() {
        RE::TESForm* form = StaticDataHolder::GetSingleton()->LookupForm(0x492e, "SimpleSlavery.esp");
        if (form == nullptr) {
            return false;
        }
        return true;
    }

    bool CheckWickedDevices() {
        RE::TESForm* form = StaticDataHolder::GetSingleton()->LookupForm(0x816, "Devious Wicked Devices.esp");
        if (form == nullptr) {
            return false;
        }
        return true;
    }

    bool CheckAND() {
        RE::TESForm* form = StaticDataHolder::GetSingleton()->LookupForm(0x837, "Advanced Nudity Detection.esp");
        if (form == nullptr) {
            return false;
        }
        return true;
    }

    bool CheckOSL() {
        RE::TESForm* form = StaticDataHolder::GetSingleton()->LookupForm(0x806, "OSLAroused.esp");
        RE::TESForm* form2 = StaticDataHolder::GetSingleton()->LookupForm(0x836, "OSLAroused.esp");
        if (form == nullptr || form2 == nullptr) {
            return false;
        }
        return true;
    }

    namespace {
        // SLO Aroused NG 3.3.0+ exports a C API from its DLL. Arousal there is the sum of named
        // effects, so this mod adds its own effects instead of writing the shared exposure value.
        struct SLOArousedAPI {
            float (*GetArousal)(RE::Actor*) = nullptr;
            float (*GetDynamicEffectValue)(RE::Actor*, const char*) = nullptr;
            void (*AddDecayingEffect)(RE::Actor*, const char*, float, float) = nullptr;
            void (*AddLinearEffect)(RE::Actor*, const char*, float, float, float) = nullptr;
            void (*ClearDynamicEffect)(RE::Actor*, const char*) = nullptr;
        };

        constexpr auto SLO_AROUSAL_EFFECT = "DeviousCurses_Arousal";
        constexpr auto SLO_RESET_EFFECT = "DeviousCurses_FrigidWash";

        const SLOArousedAPI* GetSLOAroused() {
            static const std::optional<SLOArousedAPI> api = []() -> std::optional<SLOArousedAPI> {
                HMODULE dllHandle = GetModuleHandle(L"SexlabArousedNG.dll");
                if (dllHandle == NULL) {
                    return std::nullopt;
                }
                SLOArousedAPI result;
                result.GetArousal = reinterpret_cast<decltype(result.GetArousal)>(GetProcAddress(dllHandle, "SLA_GetArousal"));
                result.GetDynamicEffectValue = reinterpret_cast<decltype(result.GetDynamicEffectValue)>(GetProcAddress(dllHandle, "SLA_GetDynamicEffectValue"));
                result.AddDecayingEffect = reinterpret_cast<decltype(result.AddDecayingEffect)>(GetProcAddress(dllHandle, "SLA_AddDecayingEffect"));
                result.AddLinearEffect = reinterpret_cast<decltype(result.AddLinearEffect)>(GetProcAddress(dllHandle, "SLA_AddLinearEffect"));
                result.ClearDynamicEffect = reinterpret_cast<decltype(result.ClearDynamicEffect)>(GetProcAddress(dllHandle, "SLA_ClearDynamicEffect"));
                if (!result.GetArousal || !result.GetDynamicEffectValue || !result.AddDecayingEffect || !result.AddLinearEffect || !result.ClearDynamicEffect) {
                    log::warn("SLO Aroused NG is older than 3.3.0, using the SLA API.");
                    return std::nullopt;
                }
                log::info("SLO Aroused NG found, using its native API.");
                return result;
            }();
            return api ? &*api : nullptr;
        }
    }

    bool CheckSLOAroused() {
        return GetSLOAroused() != nullptr;
    }

    RE::BSScript::Variable* GetMCMSetting(std::string name, std::string modname, std::string scriptname) {
        RE::TESForm* form = StaticDataHolder::GetSingleton()->LookupForm(DCURSES_MCM, modname);
        if (!form) {
            log::error("Unable to get quest object for {}", modname);
            return nullptr;
        }
        auto handle = GetHP()->GetHandleForObject(RE::FormType::Quest, form);
        RE::BSTSmartPointer<RE::BSScript::Object> mcmObject;
        GetVM()->FindBoundObject(handle, scriptname.c_str(), mcmObject);
        if (!mcmObject) {
            log::error("Unable to get script object for {}", scriptname);
            return nullptr;
        };

        RE::BSScript::Variable* variable = mcmObject->GetProperty(name);
        if (!variable) {
            log::error("MCM setting {} not found. Do you need to build papyrus?", name);
            return nullptr;
        }
        return variable;
    }

    bool IsModDisabled() {
        auto v = GetMCMSetting("ModSuspended", "Devious Curses.esp", "DCurses_MCM");
        if (v && v->IsBool()) {
            return v->GetBool();
        }
        return false;
    }

    void SetMCMSetting(std::string name, RE::BSScript::Variable& value, std::string modname, std::string scriptname) {

        RE::TESForm* form = StaticDataHolder::GetSingleton()->LookupForm(DCURSES_MCM, modname);
        if (!form) {
            log::error("Unable to get quest object for {}", modname);
            return;
        }
        auto handle = GetHP()->GetHandleForObject(RE::FormType::Quest, form);
        RE::BSTSmartPointer<RE::BSScript::Object> mcmObject;
        GetVM()->FindBoundObject(handle, scriptname.c_str(), mcmObject);
        if (!mcmObject) {
            log::error("Unable to get script object for {}", scriptname);
            return;
        };

        GetVM()->SetPropertyValue(mcmObject, name.c_str(), value);
    }

    RE::BSTSmartPointer<RE::BSScript::Object> ContraptionsGetRefScript(RE::TESObjectREFR* furniture) {
        auto handle = GetHP()->GetHandleForObject(RE::FormType::Reference, furniture);
        RE::BSTSmartPointer<RE::BSScript::Object> scriptObject;
        GetVM()->FindBoundObject(handle, "zadcFurnitureScript", scriptObject);

        if (!scriptObject) {
            return nullptr;
        }

        return scriptObject;
    }

    ScriptIntent::ScriptIntent(std::string script_name, std::string function, RE::BSScript::IFunctionArguments* args) {
        this->script_name = script_name;
        this->function = function;
        this->args = args;
    }

    ScriptIntent::ScriptIntent(std::string script_name, std::string function) {
        this->script_name = script_name;
        this->function = function;
        this->args = new RE::BSScript::ZeroFunctionArguments;
    }

    ScriptIntent::ScriptIntent(RE::TESForm* form, RE::FormType type, std::string script_name, std::string function, RE::BSScript::IFunctionArguments* args) {
        if (!form) { log::error("nullptr form passed into ScriptIntent {} {}", script_name, function); }
        this->script_name = script_name;
        this->function = function;
        this->form = form;
        this->type = type;
        this->args = args;
    }

    ScriptIntent::ScriptIntent(RE::TESForm* form, RE::FormType type, std::string script_name, std::string function) {
        if (!form) { log::error("nullptr form passed into ScriptIntent {} {}", script_name, function); }
        this->script_name = script_name;
        this->function = function;
        this->form = form;
        this->type = type;
        this->args = new RE::BSScript::ZeroFunctionArguments;
    }

    void VirtualScriptCallbackFunctor::dispatch() {
        RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>(this);
        if (intent.form == nullptr) {
            GetVM()->DispatchStaticCall(intent.script_name, intent.function, intent.args, callback);
            delete intent.args;
        }
        else {
            RE::VMHandle hand = GetHP()->GetHandleForObject(intent.type, intent.form);
            RE::BSTSmartPointer<RE::BSScript::Object> obj;
            GetVM()->FindBoundObject(hand, intent.script_name.c_str(), obj);
            if (!obj) {
                log::error("Unable to get script \"{}\" object for {:08x} \"{}\" ({})", intent.script_name, intent.form->formID, intent.form->GetName(), RE::FormTypeToString(intent.type));
            }
            GetVM()->DispatchMethodCall(obj, intent.function, intent.args, callback);
            delete intent.args;
        }

    }

    VirtualScriptCallbackFunctor::VirtualScriptCallbackFunctor(ScriptIntent intent) {
        this->intent = intent;
    }

    void VirtualScriptCallbackFunctor::SetNextCallback(RE::BSTSmartPointer<VirtualScriptCallbackFunctor> next) {
        nextCallback = next;
    }

    RE::BSTSmartPointer<VirtualScriptCallbackFunctor> VirtualScriptCallbackFunctor::PushNewCallback(RE::BSTSmartPointer<VirtualScriptCallbackFunctor> next) {
        auto tempCallback = nextCallback;
        nextCallback = next;
        next->nextCallback = tempCallback;
        return next;
    }


    void ScriptCallbackFunctor::operator()(RE::BSScript::Variable) {
        if (nextCallback != nullptr) {
            nextCallback->dispatch();
        }
    }


    template<class T>
    ScriptCallbackFunctor_R<T>::ScriptCallbackFunctor_R(ScriptIntent intent, std::function<void(std::optional<T>, ScriptCallback)> onCallbackFunction) : VirtualScriptCallbackFunctor(intent) {
        onCallback = onCallbackFunction;
    }

    template<class T>
    void ScriptCallbackFunctor_R<T>::operator()(RE::BSScript::Variable a_result) {
        std::optional<T> data;
        if (a_result.IsNoneObject()) {
            data = std::nullopt;
        }
        else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
            data = a_result.GetSInt();
        }
        else if constexpr (std::is_floating_point_v<T>) {
            data = a_result.GetFloat();
        }
        else if constexpr (std::is_same_v<RE::BGSRefAlias*, T>) {
            data = a_result.Unpack<T>();
        }
        else {
            static_assert(std::is_same_v<RE::TESForm*, T>);
            data = a_result.Unpack<T>();
        }

        onCallback(data, ScriptCallback(this));

        if (nextCallback != nullptr) {
            nextCallback->dispatch();
        }
    }

    template<class V>
    ScriptCallbackFunctor_R<RE::BSTArray<V>>::ScriptCallbackFunctor_R(ScriptIntent intent, std::function<void(RE::BSTArray<V>, ScriptCallback)> onCallbackFunction) : VirtualScriptCallbackFunctor(intent) {
        onCallback = onCallbackFunction;
    }

    template<class V>
    void ScriptCallbackFunctor_R<RE::BSTArray<V>>::operator()(RE::BSScript::Variable a_result) {
        RE::BSTArray<V> result;

        if (!a_result.IsNoneObject() && !a_result.IsNoneArray()) {

            result = a_result.Unpack<RE::BSTArray<V>>();
        }

        onCallback(result, ScriptCallback(this));

        if (nextCallback != nullptr) {
            nextCallback->dispatch();
        }
    }


    void ScriptingManager::DoSequential(IntentList intents) {
        RE::BSTSmartPointer <VirtualScriptCallbackFunctor> dispatch = intents[0].second;

        for (int i = static_cast<int>(intents.size()) - 2; i >= 0; i--) {
            intents[i].second->SetNextCallback(intents[i + 1].second);
        }

        dispatch->dispatch();
    }

    void ScriptingManager::RunOrderedIntent() {
        if (!intents()->empty()) {
            DoSequential(*intents());
            intents()->clear();
        }
    }

    ScriptingManager::ScriptingManager() {
        ScriptingManager::refCount += 1;
    }

    ScriptingManager::~ScriptingManager() {
        ScriptingManager::refCount -= 1;
        if (ScriptingManager::refCount == 0) {
            ScriptingManager::RunOrderedIntent();
        }
    }

    void ScriptingManager::RunIntent(ScriptIntent intent) {
        intents()->push_back({ intent, ScriptCallback(new ScriptCallbackFunctor(intent)) });
    }

    void ScriptingManager::RunOrdered(std::function<void()> onCallback) {
        std::function<void(std::optional<int>, ScriptCallback)> newCallbackFunction = [onCallback](std::optional<int>, ScriptCallback) {
            onCallback();
            };
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<float>(std::move(0.001f));
        auto intent = ScriptIntent("Utility", "Wait", args);
        intents()->push_back({ intent, ScriptCallback(new ScriptCallbackFunctor_R<int>(intent, newCallbackFunction)) });
    }

    void ScriptingManager::RunOnMenuClose(std::function<void()> onCallback) {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<float>(0.1f);
        auto intent = ScriptIntent("Utility", "WaitMenuMode", args);
        RunIntent(intent);
        RE::BSScript::IFunctionArguments* args2 = RE::MakeFunctionArguments<float>(0.01f);
        auto intent2 = ScriptIntent("Utility", "Wait", args2);
        RunIntentWith<int>(intent2, [onCallback](std::optional<int>, ScriptCallback) {
            onCallback();
            });
    }

    void ScriptingManager::Wait(float seconds) {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<float>(std::move(seconds));
        auto intent = ScriptIntent("Utility", "Wait", args);
        RunIntent(intent);
    }

    void ScriptingManager::MCMRegisterModEvents() {
        RE::TESForm* form = StaticDataHolder::GetSingleton()->LookupForm(DCURSES_MCM, "Devious Curses.esp");
        auto intent = ScriptIntent(form, RE::FormType::Quest, "DCurses_MCM", "RegisterModEvents");
        RunIntent(intent);
    }

    void ScriptingManager::StartSex(RE::Actor* aggressor, bool preferAggressive) {
        std::string additionalTags = Settings::GetSingleton()->sexFilterFuta ? (SexLab::IsFuta(aggressor) ? "FF,Lesbian" : "MF,FM") : "";
        RE::TESForm* DCursesLib = StaticDataHolder::GetSingleton()->LookupForm(DCURSES_LIB, "Devious Curses.esp");
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, bool>(std::move(aggressor), std::move(preferAggressive), std::move(additionalTags));
        auto intent = ScriptIntent(DCursesLib, RE::FormType::Quest, "DCursesLib", "StartSex", args);
        RunIntent(intent);
    }

    void ScriptingManager::SlowStrip(RE::Actor* actor) {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*>(std::move(actor));
        RE::TESForm* DCursesLib = StaticDataHolder::GetSingleton()->LookupForm(DCURSES_LIB, "Devious Curses.esp");
        auto intent = ScriptIntent(DCursesLib, RE::FormType::Quest, "DCursesLib", "SlowStrip", args);
        RunIntent(intent);
    }

    void ScriptingManager::OpenDebugMenu() {
        RE::TESForm* DCursesLib = StaticDataHolder::GetSingleton()->LookupForm(DCURSES_LIB, "Devious Curses.esp");
        auto intent = ScriptIntent(DCursesLib, RE::FormType::Quest, "DCursesLib", "OpenDebugMenu");
        RunIntent(intent);
    }

    int ScriptingManager::GetArousal(RE::Actor* actor) {
        if (auto slo = GetSLOAroused()) {
            // Live value for any actor; the faction rank only moves for actors in an update cycle.
            return static_cast<int>(std::clamp(slo->GetArousal(actor), 0.0f, 100.0f));
        }
        RE::TESFaction* sla_arousal = StaticDataHolder::GetSingleton()->LookupForm<RE::TESFaction>(0x3FC36, "SexLabAroused.esm");
        auto arousal = actor->GetFactionRank(sla_arousal, actor == RE::PlayerCharacter::GetSingleton());
        return arousal;
    }

    void ScriptingManager::WithArousal(RE::Actor* actor, std::function<void(float)> func) {
        if (auto slo = GetSLOAroused()) {
            // Writes through the native API apply at once, so this read already sees them.
            func(std::clamp(slo->GetArousal(actor), 0.0f, 100.0f));
            return;
        }

        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*>(std::move(actor));

        if (CheckOSL()) {
            auto intent = ScriptIntent("OSLArousedNative", "GetArousal", args);
            RunIntentWithResultSimple<float>(intent, [func](std::optional<float> arousal) {
                func(arousal.value_or(-1));
                });
        }
        else {
            RE::TESForm* aroused = StaticDataHolder::GetSingleton()->LookupForm(0x4290f, "SexLabAroused.esm");
            auto intent = ScriptIntent(aroused, RE::FormType::Quest, "slaFrameworkScr", "GetActorArousal", args);
            RunIntentWithResultSimple<int>(intent, [func](std::optional<int> arousal) {
                func(static_cast<float>(arousal.value_or(-1)));
                });
        }
    }

    void ScriptingManager::ModifyArousal(RE::Actor* actor, float arousal) {
        if (auto slo = GetSLOAroused()) {
            float value = std::clamp(slo->GetDynamicEffectValue(actor, SLO_AROUSAL_EFFECT) + arousal, 0.0f, 100.0f);
            if (value > 0.0f) {
                slo->AddDecayingEffect(actor, SLO_AROUSAL_EFFECT, value, std::max(Settings::GetSingleton()->arousalHalfLife, 1.0f));
            }
            else {
                slo->ClearDynamicEffect(actor, SLO_AROUSAL_EFFECT);
            }
        }
        else if (CheckOSL()) {
            OSL_ModifyArousal(actor, arousal);
        }
        else {
            Generic_ModifyArousal(actor, static_cast<int>(arousal));
        }
    }

    void ScriptingManager::ResetArousal(RE::Actor* actor) {
        if (auto slo = GetSLOAroused()) {
            // Other mods' effects stay, so hold the total down with -100 that climbs back to 0 evenly over the
            // set duration. Drinking another one restarts it at -100.
            slo->ClearDynamicEffect(actor, SLO_AROUSAL_EFFECT);
            float hours = std::max(Settings::GetSingleton()->arousalPotionDuration, 1.0f);
            slo->AddLinearEffect(actor, SLO_RESET_EFFECT, -100.0f, 100.0f / hours, 0.0f);
        }
        else {
            ModifyArousal(actor, -1000);
        }
    }

    void ScriptingManager::Generic_ModifyArousal(RE::Actor* actor, int arousal) {
        RE::TESForm* aroused = StaticDataHolder::GetSingleton()->LookupForm(0x4290f, "SexLabAroused.esm");
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, int, std::string>(std::move(actor), std::move(arousal), "");
        auto intent = ScriptIntent(aroused, RE::FormType::Quest, "slaFrameworkScr", "UpdateActorExposure", args);
        RunIntent(intent);
    }

    void ScriptingManager::OSL_ModifyArousal(RE::Actor* actor, float arousal) {
        if (!CheckOSL()) { return; }
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, float>(std::move(actor), std::move(arousal));
        auto intent = ScriptIntent("OSLArousedNative", "ModifyArousal", args);
        RunIntent(intent);
    }

    void ScriptingManager::OSL_ModifyLibido(RE::Actor* actor, float ammount) {
        if (!CheckOSL()) { return; }

        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*>(std::move(actor));
        auto intent = ScriptIntent("OSLArousedNative", "GetLibido", args);
        RunIntentWithResultSimple<float>(intent, [actor, ammount](std::optional<float> libido) {
            RE::Actor* actor_non_const = actor;
            RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, float>(std::move(actor_non_const), std::move(libido.value_or(0.0f) + ammount));
            auto intent = ScriptIntent("OSLArousedNative", "SetLibido", args);
            ScriptingManager().RunIntent(intent);
            });
    }

    void ScriptingManager::LockDevice(RE::Actor* akActor, RE::TESObjectARMO* deviceInventory, bool force) {
        if (akActor == nullptr || deviceInventory == nullptr) { return; }

        RE::TESForm* libs = StaticDataHolder::GetSingleton()->LookupForm(std::stoi("f624", 0, 16), "Devious Devices - Integration.esm");
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, RE::TESObjectARMO*, bool>(std::move(akActor), std::move(deviceInventory), std::move(force));
        auto intent = ScriptIntent(libs, RE::FormType::Quest, "zadLibs", "LockDevice", args);
        RunIntent(intent);
    }

    void ScriptingManager::UnlockDevice(RE::Actor* akActor, RE::TESObjectARMO* deviceInventory, RE::TESObjectARMO* deviceRendered, RE::BGSKeyword* zad_DeviousDevice, bool destroyDevice, bool genericonly) {
        if (deviceRendered == nullptr) {
            deviceRendered = DeviousDevicesAPI::g_API->GetDeviceRender(deviceInventory);
        }
        if (akActor == nullptr || deviceInventory == nullptr) { return; }

        RE::TESForm* libs = StaticDataHolder::GetSingleton()->LookupForm(std::stoi("f624", 0, 16), "Devious Devices - Integration.esm");
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, RE::TESObjectARMO*, RE::TESObjectARMO*, RE::BGSKeyword*, bool, bool>(std::move(akActor), std::move(deviceInventory), std::move(deviceRendered), std::move(zad_DeviousDevice), std::move(destroyDevice), std::move(genericonly));
        auto intent = ScriptIntent(libs, RE::FormType::Quest, "zadLibs", "UnlockDevice", args);
        RunIntent(intent);
    }

    void ScriptingManager::SwapDevices(RE::Actor* akActor, RE::TESObjectARMO* deviceInventory, RE::BGSKeyword* zad_DeviousDevice, bool destroyDevice, bool genericonly) {
        if (akActor == nullptr || deviceInventory == nullptr) { return; }

        RE::TESForm* libs = StaticDataHolder::GetSingleton()->LookupForm(std::stoi("f624", 0, 16), "Devious Devices - Integration.esm");
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, RE::TESObjectARMO*, RE::BGSKeyword*, bool, bool>(std::move(akActor), std::move(deviceInventory), std::move(zad_DeviousDevice), std::move(destroyDevice), std::move(genericonly));
        auto intent = ScriptIntent(libs, RE::FormType::Quest, "zadLibs", "SwapDevices", args);
        RunIntent(intent);
    }

    void ScriptingManager::VibrateEffect(RE::Actor* akActor, int vibStrength, int duration, bool teaseOnly, bool silent) {
        if (akActor == nullptr) { return; }

        RE::TESForm* libs = StaticDataHolder::GetSingleton()->LookupForm(std::stoi("f624", 0, 16), "Devious Devices - Integration.esm");
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, int, int, bool, bool>(std::move(akActor), std::move(vibStrength), std::move(duration), std::move(teaseOnly), std::move(silent));
        auto intent = ScriptIntent(libs, RE::FormType::Quest, "zadLibs", "VibrateEffect", args);
        RunIntent(intent);
    }

    void ScriptingManager::ActorDeviceOrgasm(RE::Actor* akActor, int setArousalTo, int vsID) {
        if (akActor == nullptr) { return; }

        RE::TESForm* libs = StaticDataHolder::GetSingleton()->LookupForm(std::stoi("f624", 0, 16), "Devious Devices - Integration.esm");
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, int, int>(std::move(akActor), std::move(setArousalTo), std::move(vsID));
        auto intent = ScriptIntent(libs, RE::FormType::Quest, "zadLibs", "ActorOrgasm", args);
        RunIntent(intent);
    }

    void ScriptingManager::ShockActor(RE::Actor* akActor) {
        if (akActor == nullptr) { return; }

        RE::TESForm* libs = StaticDataHolder::GetSingleton()->LookupForm(std::stoi("f624", 0, 16), "Devious Devices - Integration.esm");
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*>(std::move(akActor));
        auto intent = ScriptIntent(libs, RE::FormType::Quest, "zadLibs", "ShockActor", args);
        RunIntent(intent);
    }

    void ScriptingManager::UnequipItem(RE::Actor* akActor, RE::TESForm* item) {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::TESForm*, bool, bool>(std::move(item), std::move(false), std::move(true));
        auto intent = ScriptIntent(akActor, RE::FormType::ActorCharacter, "Actor", "UnequipItem", args);
        RunIntent(intent);
    }

    void ScriptingManager::UnequipItemSlot(RE::Actor* akActor, RE::BGSBipedObjectForm::BipedObjectSlot itemSlot) {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<int>(std::move(static_cast<int>(itemSlot)));
        auto intent = ScriptIntent(akActor, RE::FormType::ActorCharacter, "Actor", "UnequipItemSlot", args);
        RunIntent(intent);
    }

    void ScriptingManager::UnequipSpell(RE::Actor* akActor, RE::SpellItem* spell, int akSource) {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::SpellItem*, int>(std::move(spell), std::move(akSource));
        auto intent = ScriptIntent(akActor, RE::FormType::ActorCharacter, "Actor", "UnequipSpell", args);
        RunIntent(intent);
    }

    void ScriptingManager::RTDoTattooEvent(RE::Actor* akActor, int count) {
        if (akActor == nullptr) { return; }

        RE::TESForm* form = StaticDataHolder::GetSingleton()->LookupForm(0xd62, "RapeTattoos.esp");
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, int>(std::move(akActor), std::move(count));
        auto intent = ScriptIntent(form, RE::FormType::Quest, "rapeTattoos", "doTattooActionFor", args);
        RunIntent(intent);
    }

    void ScriptingManager::ContraptionsLockActor(RE::Actor* akActor, RE::TESObjectREFR* furniture) {
        if (akActor == nullptr || furniture == nullptr) { return; }

        RE::TESForm* clibs = StaticDataHolder::GetSingleton()->LookupForm(0x22fd, "Devious Devices - Contraptions.esm");
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, RE::TESObjectREFR*, RE::TESPackage*, bool>(std::move(akActor), std::move(furniture), nullptr, false);
        auto intent = ScriptIntent(clibs, RE::FormType::Quest, "zadcLibs", "LockActorV2", args);
        RunIntent(intent);
    }

    void ScriptingManager::ContraptionsUnlockActor(RE::Actor* akActor) {
        if (akActor == nullptr) { return; }

        RE::TESForm* clibs = StaticDataHolder::GetSingleton()->LookupForm(0x22fd, "Devious Devices - Contraptions.esm");
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*>(std::move(akActor));
        auto intent = ScriptIntent(clibs, RE::FormType::Quest, "zadcLibs", "UnlockActor", args);
        RunIntent(intent);
    }

    void ScriptingManager::ForceThirdPerson() {
        auto intent = ScriptIntent("Game", "ForceThirdPerson");
        RunIntent(intent);
    }

    void ScriptingManager::DisableMenus() {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<bool, bool, bool, bool, bool, bool, bool, bool, int>(std::move(false), std::move(false), std::move(false), std::move(false), std::move(false), std::move(true), std::move(true), std::move(false), std::move(0));
        auto intent = ScriptIntent("Game", "DisablePlayerControls", args);
        RunIntent(intent);
    }

    void ScriptingManager::EnableMenus() {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<bool, bool, bool, bool, bool, bool, bool, bool, int>(std::move(false), std::move(false), std::move(false), std::move(false), std::move(false), std::move(true), std::move(true), std::move(false), std::move(0));
        auto intent = ScriptIntent("Game", "EnablePlayerControls", args);
        RunIntent(intent);
    }

    void ScriptingManager::EnableFastTravel(bool enabled) {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<bool>(std::move(enabled));
        auto intent = ScriptIntent("Game", "EnableFastTravel", args);
        RunIntent(intent);
    }

    void ScriptingManager::CloseContainerMenus() {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<std::string, std::string, std::string>("HUD Menu", "_global.skse.CloseMenu", "ContainerMenu");
        //UI.InvokeString("HUD Menu", "_global.skse.CloseMenu", "InventoryMenu")
        auto intent = ScriptIntent("UI", "InvokeString", args);
        RunIntent(intent);

        //RE::BSScript::IFunctionArguments* args2 = RE::MakeFunctionArguments<std::string, std::string, std::string>("HUD Menu", "_global.skse.CloseMenu", "InventoryMenu");
        //UI.InvokeString("HUD Menu", "_global.skse.CloseMenu", "InventoryMenu")
        //auto intent2 = ScriptIntent("UI", "InvokeString", args2);
        //RunIntent(intent2);
    }

    void ScriptingManager::QuestSetStage(RE::TESQuest* quest, int stage) {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<int>(std::move(stage));
        auto intent = ScriptIntent(quest, RE::FormType::Quest, "Quest", "SetStage", args);
        RunIntent(intent);
    }

    void ScriptingManager::RequestSaveGame() {
        auto intent = ScriptIntent("Game", "RequestSave");
        RunIntent(intent);
    }

    void ScriptingManager::PushActorAway(RE::TESObjectREFR* object, RE::Actor* akActor, float force) {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::Actor*, float>(std::move(akActor), std::move(force));
        auto intent = ScriptIntent(object, RE::FormType::Reference, "ObjectReference", "PushActorAway", args);
        RunIntent(intent);
    }

    void ScriptingManager::VisualEffectPlay(RE::BGSReferenceEffect* effect, RE::TESObjectREFR* ref_location, float time, RE::TESObjectREFR* facing_object) {
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<RE::TESObjectREFR*, float, RE::TESObjectREFR*>(std::move(ref_location), std::move(time), std::move(facing_object));
        auto intent = ScriptIntent(effect, RE::FormType::ReferenceEffect, "VisualEffect", "Play", args);
        RunIntent(intent);
        //RE::BGSReferenceEffect* SummonEffect = RE::TESForm::LookupByID(0x00107D96)->As<RE::BGSReferenceEffect>();
    }

    void ScriptingManager::PlaySummonVFX(RE::TESObjectREFR* ref_location) {
        RE::BGSReferenceEffect* summon_effect = RE::TESForm::LookupByID(0x00107D96)->As<RE::BGSReferenceEffect>();
        VisualEffectPlay(summon_effect, ref_location, 3.0);
    }

    void ScriptingManager::DBGMessageBox(Translator trans) {
        auto message = trans.GetTranslation();
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<std::string>(std::move(message));
        auto intent3 = ScriptIntent("Debug", "MessageBox", args);
        RunIntent(intent3);
    }

    void ScriptingManager::DBGNotification(Translator trans) {
        auto message = trans.GetTranslation();
        RE::BSScript::IFunctionArguments* args = RE::MakeFunctionArguments<std::string>(std::move(message));
        auto intent3 = ScriptIntent("Debug", "Notification", args);
        RunIntent(intent3);
    }
}
