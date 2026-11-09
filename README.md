# Devious Curses NG

Source code for the mod [Devious Curses NG](https://www.loverslab.com/files/file/41083-devious-curses-ng/) hosted on LoversLab.

If you are looking for how to make a SKSE mod or an example of how to build this project, I highly recommend looking at [this tutorial.](https://github.com/SkyrimScripting/SKSE_Template_HelloWorld)

## This fork

Adds support for [SLO Aroused NG](https://crajjjj.github.io/SexlabArousedNG/) 3.3.0 or newer through its native API. The releases here are a drop-in `DeviousCurses.dll` and MCM for Devious Curses 0.9.0.1; OSL Aroused and other SLA-compatible arousal mods keep working as before.

- Arousal is read live for every actor. The `sla_Arousal` faction rank used before only moves for actors in an update cycle.
- Arousal this mod adds goes into its own effect, `DeviousCurses_Arousal`, which halves every 12 game hours (MCM, Main page: Arousal Half-Life).
- A Frigid Wash clears that effect and lowers arousal by 100 with `DeviousCurses_FrigidWash`, which climbs back to 0 evenly over 24 game hours (MCM, Misc page: Frigid Wash Duration). Libido is an OSL Aroused feature, so the libido setting still needs OSL Aroused.
- The MCM Help page shows whether SLO Aroused NG is in use; both sliders are greyed out without it.

Bug fixes:

- A Devious Devices contraption faction left on the player (`zadcNGInContraptionFaction`) made the mod treat every second as a running scene, so oppressive device and Lewd Mark counters stopped for the rest of the save. It is now removed when no contraption holds the player, about 20 seconds after it goes stale or after loading such a save. A `zadAnimatingFaction` held for 5 minutes is logged with the console command to clear it.
- The Nocturnal piercing never came off, because no spectator ever counted. Spectators now count at any distance, as long as they can see the player.

### Building

Run `build.cmd` (Visual Studio 2022 with the C++ and CMake workloads; vcpkg is the copy bundled with Visual Studio). It builds the DLL alone into `dist\SKSE\Plugins\`.

The public sources lack the Quests module, so `DCURSES_DLL_ONLY` skips codegen and the Papyrus steps, and `src/Quest/` holds stubs that keep the Quests save record intact. `cmake/ports/commonlibsse-ng` pins the same CommonLibSSE-NG commit as the upstream registry, with the archive hash GitHub serves today.

After changing the MCM block in `include/Settings.h`, run `python tools/codegen_public.py`: it runs `CODEGEN.py` without the Quests MCM and writes `DCurses_MCM.psc`, `translationData/Devious Curses_ENGLISH.txt` and the generated parts of `src/Settings.cpp`. Compile `DCurses_MCM.psc` with the Papyrus compiler against the 0.9.0.1 `DCursesLib.psc` and the SkyUI SDK, SexLab P+, PapyrusUtil, Devious Devices NG, RaceMenu, UIExtensions, MfgFix NG, AnimSpeedSE and Lovense sources.
