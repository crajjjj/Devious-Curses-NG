#pragma once

#include "Translation.h"

namespace DCURSES {
    RE::BSScript::IVirtualMachine* GetVM();

    RE::BSScript::IObjectHandlePolicy* GetHP();

    bool CheckESPLoaded();

    bool CheckQuestLoaded();

    bool CheckRapeTattoos();

    bool CheckUD();

    bool CheckSimpleSlavery();

    bool CheckWickedDevices();

    bool CheckAND();

    bool CheckOSL();

    bool CheckSLOAroused();

    class ScriptIntent {
    public:
        std::string script_name;
        std::string function;
        RE::TESForm* form = nullptr;
        RE::FormType type = RE::FormType::None;

        RE::BSScript::IFunctionArguments* args;

        ScriptIntent(std::string script_name, std::string function, RE::BSScript::IFunctionArguments* args);
        ScriptIntent(std::string script_name, std::string function);
        ScriptIntent(RE::TESForm* form, RE::FormType type, std::string script_name, std::string function, RE::BSScript::IFunctionArguments* args);
        ScriptIntent(RE::TESForm* form, RE::FormType type, std::string script_name, std::string function);
    };

    class VirtualScriptCallbackFunctor : public RE::BSScript::IStackCallbackFunctor {
    public:
        RE::BSTSmartPointer<VirtualScriptCallbackFunctor> nextCallback = nullptr;
        ScriptIntent intent = ScriptIntent("", "", nullptr);

        void dispatch();

        virtual void SetObject(const RE::BSTSmartPointer<RE::BSScript::Object>&) override {}

        VirtualScriptCallbackFunctor(ScriptIntent intent);

        void SetNextCallback(RE::BSTSmartPointer<VirtualScriptCallbackFunctor> next);

        RE::BSTSmartPointer<VirtualScriptCallbackFunctor> PushNewCallback(RE::BSTSmartPointer<VirtualScriptCallbackFunctor> next);
    };

    class ScriptCallbackFunctor : public VirtualScriptCallbackFunctor {
    public:
        ScriptCallbackFunctor(ScriptIntent intent) : VirtualScriptCallbackFunctor(intent) {}

        virtual void operator()(RE::BSScript::Variable) override;
    };

    typedef RE::BSTSmartPointer<VirtualScriptCallbackFunctor> ScriptCallback;

    template<class T>
    class ScriptCallbackFunctor_R : public VirtualScriptCallbackFunctor {
        std::function<void(std::optional<T>, ScriptCallback)> onCallback;
    public:
        ScriptCallbackFunctor_R(ScriptIntent intent, std::function<void(std::optional<T>, ScriptCallback)> onCallbackFunction);

        virtual void operator()(RE::BSScript::Variable a_result) override;
    };

    template<class V>
    class ScriptCallbackFunctor_R<RE::BSTArray<V>> : public VirtualScriptCallbackFunctor {
        std::function<void(RE::BSTArray<V>, ScriptCallback)> onCallback;
    public:
        ScriptCallbackFunctor_R(ScriptIntent intent, std::function<void(RE::BSTArray<V>, ScriptCallback)> onCallbackFunction);

        virtual void operator()(RE::BSScript::Variable a_result) override;
    };

    class ScriptingManager {
    private:
        typedef std::vector<std::pair<ScriptIntent, ScriptCallback>> IntentList;
        static IntentList* intents() {
            static IntentList intents;
            return &intents;
        }

        static uint32_t refCount;

        static void DoSequential(IntentList intents);

        static void RunOrderedIntent();

    public:
        ScriptingManager(ScriptingManager const&) = delete;
        void operator=(ScriptingManager const&) = delete;

        ScriptingManager();

        ~ScriptingManager();

        void RunIntent(ScriptIntent intent);

        template<class T>
        void RunIntentWith(ScriptIntent intent, std::function<void(std::optional<T>, ScriptCallback)> onCallback) {
            intents()->push_back({ intent, ScriptCallback(new ScriptCallbackFunctor_R<T>(intent, onCallback)) });
        }

        template<class T>
        void RunIntentWithResult(ScriptIntent intent, std::function<ScriptCallback(std::optional<T>)> onCallback) {
            std::function<void(std::optional<T>, ScriptCallback)> newCallbackFunction = [onCallback](std::optional<T> data, ScriptCallback firstCallback) {
                auto newCallback = onCallback(data);
                if (newCallback != nullptr) {
                    auto tempCallback = firstCallback->nextCallback;
                    firstCallback->nextCallback = newCallback;
                    newCallback->nextCallback = tempCallback;
                }
                };
            intents()->push_back({ intent, ScriptCallback(new ScriptCallbackFunctor_R<T>(intent, newCallbackFunction)) });
        }

        template<class T>
        void RunIntentWithResultSimple(ScriptIntent intent, std::function<void(std::optional<T>)> onCallback) {
            std::function<void(std::optional<T>, ScriptCallback)> newCallbackFunction = [onCallback](std::optional<T> data, ScriptCallback) {
                onCallback(data);
                };
            intents()->push_back({ intent, ScriptCallback(new ScriptCallbackFunctor_R<T>(intent, newCallbackFunction)) });
        }

        void RunOrdered(std::function<void()> onCallback);

        void RunOnMenuClose(std::function<void()> onCallback);

        void Wait(float seconds);

        void MCMRegisterModEvents();

        void StartSex(RE::Actor* aggressor, bool preferAggressive);

        void SlowStrip(RE::Actor* actor);

        void OpenDebugMenu();

        int GetArousal(RE::Actor* actor);

        void WithArousal(RE::Actor* actor, std::function<void(float)> func);

        void ModifyArousal(RE::Actor* actor, float arousal);

        void ResetArousal(RE::Actor* actor);

        void Generic_ModifyArousal(RE::Actor* actor, int arousal);

        void OSL_ModifyArousal(RE::Actor* actor, float arousal);

        void OSL_ModifyLibido(RE::Actor* actor, float ammount);

        void LockDevice(RE::Actor* akActor, RE::TESObjectARMO* deviceInventory, bool force = false);

        void UnlockDevice(RE::Actor* akActor, RE::TESObjectARMO* deviceInventory, RE::TESObjectARMO* deviceRendered = nullptr, RE::BGSKeyword* zad_DeviousDevice = nullptr, bool destroyDevice = false, bool genericonly = true);

        void SwapDevices(RE::Actor* akActor, RE::TESObjectARMO* deviceInventory, RE::BGSKeyword* zad_DeviousDevice = nullptr, bool destroyDevice = false, bool genericonly = true);

        void VibrateEffect(RE::Actor* akActor, int vibStrength, int duration, bool teaseOnly = false, bool silent = false);

        void ActorDeviceOrgasm(RE::Actor* akActor, int setArousalTo = -1, int vsID = -1);

        void ShockActor(RE::Actor* akActor);

        void UnequipItem(RE::Actor* akActor, RE::TESForm* item);

        void UnequipItemSlot(RE::Actor* akActor, RE::BGSBipedObjectForm::BipedObjectSlot itemSlot);

        void UnequipSpell(RE::Actor* akActor, RE::SpellItem* spell, int akSource);

        void RTDoTattooEvent(RE::Actor* akActor, int count);

        void ContraptionsLockActor(RE::Actor* akActor, RE::TESObjectREFR* furniture);
        
        void ContraptionsUnlockActor(RE::Actor* akActor);

        void QuestSetStage(RE::TESQuest* quest, int stage);

        void ForceThirdPerson();

        void DisableMenus();

        void EnableMenus();

        void EnableFastTravel(bool enabled);

        void CloseContainerMenus();

        void RequestSaveGame();

        void PushActorAway(RE::TESObjectREFR* object, RE::Actor* akActor, float force);

        void VisualEffectPlay(RE::BGSReferenceEffect* effect, RE::TESObjectREFR* ref_location, float time, RE::TESObjectREFR* facing_object = nullptr);

        void PlaySummonVFX(RE::TESObjectREFR* ref_location);

        void DBGMessageBox(Translator trans);

        void DBGNotification(Translator trans);
    };

    RE::BSScript::Variable* GetMCMSetting(std::string name, std::string modname, std::string scriptname);

    bool IsModDisabled();

    void SetMCMSetting(std::string name, RE::BSScript::Variable& value, std::string modname, std::string scriptname);

    RE::BSTSmartPointer<RE::BSScript::Object> ContraptionsGetRefScript(RE::TESObjectREFR* furniture);
}