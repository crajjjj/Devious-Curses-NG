/*
0.9 TODO
make fine consequence use getWatchingActors
add stuff
*/

#include "FastTravel.h"
#include "Settings.h"
#include "Scripting.h"
#include "Utils.h"
#include "Serializer.h"
#include "Sex.h"
#include "OppDevices.h"
#include "Tats.h"
#include "Devices.h"
#include "Migration.h"
#include "TESEvents.h"
#include "QuestInteractions.h"
#include "QLIEIntegration.hpp"
#include "DebugMode.h"
#include "MGEF_Controller.h"
#include "Events.h"
#include "Themes.h"
#include "Translation.h"
#include "Consequences.h"
#include "Contraptions.h"

#include "Quest/QuestSettings.h"
#include "Quest/QuestEvents.h"

#include "apis/jcontainers.hpp"
#include "apis/DDNG_API.h"
#include "apis/SlaveTatsNG_Interface.h"

#include <d3d11.h>
#include <windows.h>
#include <debugapi.h>
#include <chrono>
#include <xstring>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <algorithm>

using namespace SKSE;

namespace DCURSES {
    void InitializeLogging() {
        auto path = log::log_directory();
        if (!path) {
            stl::report_and_fail("Unable to lookup SKSE logs directory.");
        }
        *path /= PluginDeclaration::GetSingleton()->GetName();
        *path += L".log";

        auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
        auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));

        spdlog::set_default_logger(std::move(loggerPtr));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::trace);
    }

    static bool UPDATE_LOOP_RUNNING = false;
    bool can_install_fast_travel_hook = false;

    void StartUpdateLoop() {
        if (UPDATE_LOOP_RUNNING) {
            return;
        }

        UPDATE_LOOP_RUNNING = true;

        std::thread{ [] {
            bool shouldSkipNextLoop = false;
            int contraptionSeconds = 0;
            int zadAnimatingSeconds = 0;
            while (true) {
                std::this_thread::sleep_for(1000ms);
                
                SetFastTravelStatePapyrus(can_install_fast_travel_hook);

                if (shouldSkipNextLoop) {
                    //log::trace("Skip Loop Marked");
                    shouldSkipNextLoop = false;
                }

                //log::trace("last settings: {}", std::string(std::format("{:%H%M%S}", lastSettingsEditTime)));

                if (IsModDisabled()) {
                    //log::trace("Event timer skipped, mod is disabled.");
                    continue;
                }

                if (RE::UI::GetSingleton()->GameIsPaused()) {
                    //log::trace("Event timer skipped, game is paused.");
                    continue;
                }


                bool isInNonPausedMenu = false;
                std::vector<std::string> menu_names = Util::split("InventoryMenu,Console,Dialogue Menu,MessageBoxMenu,MagicMenu,Loading Menu,TweenMenu,BarterMenu,GiftMenu,MapMenu,Lockpicking Menu,StatsMenu,ContainerMenu,Sleep/Wait Menu,LevelUp Menu,Journal Menu,Book Menu,FavoritesMenu,RaceSex Menu,Crafting Menu,Training Menu,Tutorial Menu", ",");
                for (auto menu : menu_names) {
                    if (RE::UI::GetSingleton()->IsMenuOpen(menu)) {
                        if (menu != "Crafting Menu" && menu != "Dialogue Menu") {
                            log::info("Event timer skipped, menu open: {}", menu);
                        }
                        isInNonPausedMenu = true;
                    }
                }
                

                auto player = RE::PlayerCharacter::GetSingleton();

                if (player->IsDead() || player->IsDeleted() || player->IsDisabled()) {
                    continue;
                }

                RE::TESFaction* SexlabAnimatingFaction = StaticDataHolder::GetSingleton()->LookupForm<RE::TESFaction>(0x00E50F, "SexLab.esm");
                RE::TESFaction* ZadAnimatingFaction = StaticDataHolder::GetSingleton()->LookupForm<RE::TESFaction>(0x029567, "Devious Devices - Integration.esm");
                RE::TESFaction* zadcNGInContraptionFaction = StaticDataHolder::GetSingleton()->LookupForm<RE::TESFaction>(ZADC_IN_CONTRAPTION_FACTION, "Devious Devices - Contraptions.esm");

                bool inSexLab = player->IsInFaction(SexlabAnimatingFaction);
                bool inZadAnimating = player->IsInFaction(ZadAnimatingFaction);
                bool inContraption = player->IsInFaction(zadcNGInContraptionFaction);

                if (inSexLab) {
                    Serialized::GetCounters()->clock_lastSex = 0;
                    Serialized::GetCounters()->SinceLastSex = 0;
                }

                // These factions pause every refresh below, so one left on by mistake stops the mod for the rest of the save.
                if (inContraption) {
                    if (++contraptionSeconds % 10 == 0) {
                        SKSE::GetTaskInterface()->AddTask([] { ClearStaleContraptionFaction(); });
                    }
                }
                else {
                    contraptionSeconds = 0;
                }
                if (inZadAnimating && !inSexLab) {
                    if (++zadAnimatingSeconds == 300) {
                        log::warn("Player has been in zadAnimatingFaction for 5 minutes, which pauses this mod. If no Devious Devices animation is playing, it is stuck: player.removefromfaction {:08X}", ZadAnimatingFaction->GetFormID());
                    }
                }
                else {
                    zadAnimatingSeconds = 0;
                }

                bool isAnimating = inSexLab || inZadAnimating || inContraption;

                if (!isAnimating && !isInNonPausedMenu && !shouldSkipNextLoop) {
                    auto c1 = std::chrono::high_resolution_clock::now();
                    SexUpdate();
                    auto c2 = std::chrono::high_resolution_clock::now();
                    TatsUpdate();
                    auto c3 = std::chrono::high_resolution_clock::now();
                    OppDeviceUpdate();
                    auto c4 = std::chrono::high_resolution_clock::now();
                    ConsUpdate();
                    auto c5 = std::chrono::high_resolution_clock::now();

                    auto d1 = (c2 - c1).count() / 1000000.0;
                    auto d2 = (c3 - c2).count() / 1000000.0;
                    auto d3 = (c4 - c3).count() / 1000000.0;
                    auto d4 = (c5 - c4).count() / 1000000.0;
                    auto dt = (c5 - c1).count() / 1000000.0;

                    if (dt >= 10) {
                        log::info("Notable Update Time: Sex: {:.4f}, Marks: {:.4f}, ODevices: {:.4f}, Consequence: {:.4f} total: {:.4f}ms", d1, d2, d3, d4, dt);
                    }
                    else if (Settings::GetSingleton()->debugMode && dt >= 0.1) {
                        log::debug("Update Time: Sex: {:.4f}, Marks: {:.4f}, ODevices: {:.4f}, Consequence: {:.4f} total: {:.4f}ms", d1, d2, d3, d4, dt);
                    }
                }
                
                if (isAnimating || isInNonPausedMenu){
                    shouldSkipNextLoop = true;
                }

                //Always do last!
                Serialized::GetCounters()->tick();
            }
            log::critical("Event timer has stopped.");
            UPDATE_LOOP_RUNNING = false;
        } }.detach();
    }
    
    RE::TESObjectARMO* P_GetRandomEquipableDevice(RE::StaticFunctionTag*) {
        auto dev = GetRandomEquipableDevice(RE::PlayerCharacter::GetSingleton(), {});
        if (dev) {
            return dev.value().inv;
        }
        return nullptr;
    }

    int P_numDevicesVisible(RE::StaticFunctionTag*, RE::Actor* akActor) {
        return GetVisibleDeviceCount(akActor);
    }

    int P_numDevicesEquipped(RE::StaticFunctionTag*, RE::Actor* akActor) {
        return GetWornDeviceCount(akActor);
    }

    bool P_WearingOppressiveDevice(RE::StaticFunctionTag*) {
        return GetOppDeviceMask();
    }

    bool PapyrusFunctions(RE::BSScript::IVirtualMachine* ivm) {
        ivm->RegisterFunction("GetRandomEquipableDevice", "DCursesLib", P_GetRandomEquipableDevice);
        ivm->RegisterFunction("NumDevicesVisible", "DCursesLib", P_numDevicesVisible);
        ivm->RegisterFunction("NumDevicesEquipped", "DCursesLib", P_numDevicesEquipped);
        ivm->RegisterFunction("WearingOppressiveDevice", "DCurses_MCM", P_WearingOppressiveDevice);
        PapyrusFunctionsSettigns(ivm);
        Quest::PapyrusFunctionsSettigns(ivm);
        PapyrusFunctionsSex(ivm);
        PapyrusFunctionsTats(ivm);
        return true;
    }

}

std::string getJContainersPluginName() {
    auto patchVersion = REL::Module::get().version().patch();

    std::string pluginName{ "JContainers64" };
    if (REL::Module::IsVR()) {
        pluginName = "JContainersVR";
    }
    else if (patchVersion == 659 || patchVersion == 1179) {
        pluginName = "JContainersGOG";
    }

    return pluginName;
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);

    using namespace DCURSES;

    InitializeLogging();

    auto version_string = std::string(DCURSES_VERSION);
    log::info("Initializing DeviousCurses version {}", version_string);

    Quest::LoadSettingsFile();

    LoadSettingsFile();
    VersionMigrate();
    SaveMCMSettings();

    if (Settings::GetSingleton()->debugMode) {
        log::debug("Debug mode");
    }

    REL::Version version = skse->RuntimeVersion();

    can_install_fast_travel_hook = skse->RuntimeVersion() >= SKSE::RUNTIME_SSE_1_6_640;

    if (can_install_fast_travel_hook) {
        SKSE::AllocTrampoline(14);
    }
    else {
        log::info("Fast travel hooks skipped for version < 1.6.640");
    }

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message *message) {
        switch (message->type)
        {
        case SKSE::MessagingInterface::kDataLoaded: {
            if (!CheckESPLoaded()) {
                stl::report_and_fail("Devious Curses ESP is not loaded. If you are on skyrim version older than 1.6.1130 (Not AE) make sure to install Backported Extended ESL Support.");
                return;
            }
            else {
                log::trace("ESP Loaded.");
            }

            static auto tweaks = GetModuleHandle(L"po3_Tweaks");
            if (!tweaks) {
                stl::report_and_fail("Devious Curses requires powerofthree's Tweaks to function correctly. Please refer to the mod page requirements and installation instructions.");
            }
            else {
                log::trace("po3 tweaks installed");
            }

            log::trace("Calling JCWrapper init()");
            jcontainers::JCWrapper::GetSingleton()->Init();
            RegisterEventSinks();

            QLIEAttemptInit();

            if (can_install_fast_travel_hook) {
                InstallFastTravelHooks();
            }

            if (Settings::GetSingleton()->debugMode) {
                Debug::AddLocationData();
            }

            break;
        }

        case SKSE::MessagingInterface::kNewGame:
        case SKSE::MessagingInterface::kPostLoadGame: {
            MGEFOnGameLoad();

            StaticDataHolder::GetSingleton()->InvalidateCache();

            bool DDNG_loaded = DeviousDevicesAPI::LoadAPI();

            if (!DDNG_loaded) {
                log::critical("Devious Devices NG not loaded. Mod will be disabled.");
                stl::report_and_fail("Devious devices NG not loaded for Devious Curses. Please check to make sure that you have version 4.0+");
            }
            else {
                log::trace("Devious Devices NG loaded: {}.", DeviousDevicesAPI::g_API->GetDatabase().size());
            }

            if (!QLIEAttemptRegisterEvent()) {
                log::info("Quick Loot IE Not Loaded.");
            }

            if (Quest::LoadSettingsFile()) {
                Quest::PushSettingsToMCM();
            }

            if (LoadSettingsFile()) {
                PushSettingsToMCM();
            }
            else {
                log::trace("Attempting settings load from papyrus");
                DCURSES::GetSettingsFromMCM();
            }
            EventsCheckModIntergations();
            RecalculateDeviceLists();
            MCMSetHelpPageOptions();
            ScriptingManager().MCMRegisterModEvents();

            Serialized::GetCounters()->clock_SexTimeout -= 2;
           

            Util::ExecuteWithDelay(2s, [] {
                StartUpdateLoop();
            });

            Quest::OnGameLoaded();

            break;
        }

        case SKSE::MessagingInterface::kPostLoad: {
            CreateExclusionsFileIfNeeded();
            CreateModExclusionsFileIfNeeded();
            CreateThemesFileIfNeeded();

            std::string pluginName = getJContainersPluginName();

            SKSE::GetMessagingInterface()->RegisterListener("SlaveTatsNG", slavetats_ng::messaging_hook);
            SKSE::GetMessagingInterface()->RegisterListener(pluginName.c_str(), [](SKSE::MessagingInterface::Message* a_msg) {
                if (a_msg && a_msg->type == jc::message_root_interface) {
                    const jc::root_interface* root = jc::root_interface::from_void(a_msg->data);
                    if (root)
                        jcontainers::JCWrapper::GetSingleton()->PreInit(root);
                }
            });

            break;
        }
        case SKSE::MessagingInterface::kSaveGame: {
            Serialized::GetCounters()->clock_SexTimeout -= 2;
            SaveMCMSettings();
            Quest::SaveMCMSettings();
            break;
        }
        case SKSE::MessagingInterface::kInputLoaded: {
            if (Settings::GetSingleton()->debugMode) {
                Debug::CheckTranslations();
                Debug::CheckMCMTranslations();
            }
            Translator::UpdateTranslations();

            log::trace("Moving Russian Language File");

            break;
        }
        }
    });

    log::trace("Initializing Papyrus binding...");
    if (GetPapyrusInterface()->Register(PapyrusFunctions)) {
        log::trace("Papyrus functions bound.");
    } else {
        stl::report_and_fail("Failure to register Papyrus bindings.");
    }

    InitializeSerialization();

    return true;
}