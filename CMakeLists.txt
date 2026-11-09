# It's recommended to set a minimum CMake version.
# If you use CMake features from higher versions, update this to match.
cmake_minimum_required(VERSION 3.21)

# Set your project name. This will be the name of your SKSE .dll file.
project(DeviousCurses VERSION 0.9.0.1 LANGUAGES CXX)

# ON builds the DLL alone into dist/. OFF is the upstream build: codegen, the MCM scripts and the
# translations, which needs the private Quests sources (src/Quest holds stubs here) and SourceScripts.
option(DCURSES_DLL_ONLY "Build only the DLL, into dist/" ON)


set(sources
        plugin.cpp
        src/Consequences.cpp
        src/Contraptions.cpp
        src/DebugMode.cpp
        src/Devices.cpp
        src/Events.cpp
        src/FastTravel.cpp
        src/Locations.cpp
        src/MGEF_Controller.cpp
        src/Migration.cpp
        src/ModEvents.cpp
        src/OppDevices.cpp
        src/QuestInteractions.cpp
        src/Scripting.cpp
        src/Serializer.cpp
        src/Settings.cpp
        src/Tats.cpp
        src/TESEvents.cpp
        src/Themes.cpp
        src/Translation.cpp
        src/Utils.cpp
        src/Sex.cpp

        src/Quest/Radiant.cpp
        src/Quest/QuestSettings.cpp
        src/Quest/QuestEvents.cpp
        src/Quest/Serialized.cpp
)



if(DCURSES_DLL_ONLY)
    set(OUTPUT_FOLDER "${CMAKE_SOURCE_DIR}/dist")
else()
    set(OUTPUT_FOLDER "S:/Skyrim/MO2/mods/DeviousCurses")
endif()
set(QUEST_OUTPUT_FOLDER "S:/Skyrim/MO2/mods/DeviousCursesQuests")

# set(CommonLibSSE_DIR "S:/Skyrim/modding/CommonLibVR/build/debug-msvc-vcpkg-all")

# Setup your SKSE plugin as an SKSE plugin!
find_package(directxtk CONFIG REQUIRED)
find_package(CommonLibSSE CONFIG REQUIRED)
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${sources}) # <--- specifies plugin.cpp
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23) # <--- use C++23 standard
target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h) # <--- PCH.h is required!
target_link_libraries(${PROJECT_NAME} PRIVATE d3d11.dll)
target_link_libraries(${PROJECT_NAME} PRIVATE Microsoft::DirectXTK)
target_link_libraries(${PROJECT_NAME} PUBLIC CommonLibSSE::CommonLibSSE)
target_include_directories(${PROJECT_NAME} PRIVATE include)


set(SKYRIM_FOLDER "S:/SteamLibrary/steamapps/common/Skyrim Special Edition") # Change this to your skyrim instalation location
set(SCRIPTS_SOURCE "${CMAKE_SOURCE_DIR}/SourceScripts/")


add_custom_command(
    TARGET "${PROJECT_NAME}"
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${OUTPUT_FOLDER}/SKSE/Plugins"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${OUTPUT_FOLDER}/SKSE/Plugins/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
    
    VERBATIM
)

if(DCURSES_DLL_ONLY)
    return()
endif()

add_custom_target(
    Codegen ALL
    COMMENT "Running Python Codegen"
    COMMAND python CODEGEN.py
    WORKING_DIRECTORY ./../../
    VERBATIM
)
add_dependencies(${PROJECT_NAME} Codegen)

add_custom_target(
    MCM ALL
    COMMENT "Compiling MCM"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_SOURCE_DIR}/DCurses_MCM.psc" "${OUTPUT_FOLDER}/scripts/source/DCurses_MCM.psc"
    COMMAND "${SKYRIM_FOLDER}/Papyrus Compiler/PapyrusCompiler.exe" DCurses_MCM.psc -f="${SKYRIM_FOLDER}/Data/Scripts/Source/TESV_Papyrus_Flags.flg" -i="${SCRIPTS_SOURCE}\;${SKYRIM_FOLDER}/Data/Scripts/Source/" -o="${OUTPUT_FOLDER}/scripts" -q
    WORKING_DIRECTORY "${OUTPUT_FOLDER}/scripts/source"
)
add_dependencies(MCM Codegen)

add_custom_target(
    QuestMCM ALL
    COMMENT "Compiling Quest MCM"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_SOURCE_DIR}/DCurses_QuestMCM.psc" "${QUEST_OUTPUT_FOLDER}/scripts/source/DCurses_QuestMCM.psc"
    COMMAND "${SKYRIM_FOLDER}/Papyrus Compiler/PapyrusCompiler.exe" DCurses_QuestMCM.psc -f="${SKYRIM_FOLDER}/Data/Scripts/Source/TESV_Papyrus_Flags.flg" -i="${SCRIPTS_SOURCE}\;${SKYRIM_FOLDER}/Data/Scripts/Source\;${OUTPUT_FOLDER}/scripts/source/" -o="${QUEST_OUTPUT_FOLDER}/scripts" -q
    WORKING_DIRECTORY "${QUEST_OUTPUT_FOLDER}/scripts/source"
)
add_dependencies(QuestMCM Codegen)

add_custom_target(
    Translations ALL
    COMMENT "Preparing Translation Files"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_SOURCE_DIR}/translationData/Devious Curses_English.json" "${OUTPUT_FOLDER}/translations/Devious Curses_ENGLISH.json"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_SOURCE_DIR}/translationData/Devious Curses_English.txt" "${OUTPUT_FOLDER}/interface/translations/Devious Curses_ENGLISH.txt"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_SOURCE_DIR}/translationData/Devious Curses - Quests_English.txt" "${QUEST_OUTPUT_FOLDER}/interface/translations/Devious Curses - Quests_ENGLISH.txt"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_SOURCE_DIR}/Version.txt" "${OUTPUT_FOLDER}/Version.txt"
)
add_dependencies(Translations Codegen)