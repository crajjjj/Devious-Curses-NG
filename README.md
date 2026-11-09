# Devious Curses NG

Source code for the mod [Devious Curses NG](https://www.loverslab.com/files/file/41083-devious-curses-ng/) hosted on LoversLab.

If you are looking for how to make a SKSE mod or an example of how to build this project, I highly recommend looking at [this tutorial.](https://github.com/SkyrimScripting/SKSE_Template_HelloWorld)

## This fork

Adds support for [SLO Aroused NG](https://crajjjj.github.io/SexlabArousedNG/) 3.3.0 or newer through its native API. The releases here are a drop-in `DeviousCurses.dll` for Devious Curses 0.9.0.1; OSL Aroused and other SLA-compatible arousal mods keep working as before.

- Arousal is read live for every actor. The `sla_Arousal` faction rank used before only moves for actors in an update cycle.
- Arousal this mod adds goes into its own effect, `DeviousCurses_Arousal`, which halves every 12 game hours.
- A Frigid Wash clears that effect and cancels the rest of the drinker's arousal with `DeviousCurses_FrigidWash`, which wears off at the same rate. Libido is an OSL Aroused feature, so the libido setting still needs OSL Aroused.

### Building

Run `build.cmd` (Visual Studio 2022 with the C++ and CMake workloads; vcpkg is the copy bundled with Visual Studio). It builds the DLL alone into `dist\SKSE\Plugins\`.

The public sources lack the codegen inputs (`translationData`, `SourceScripts`) and the Quests module, so `DCURSES_DLL_ONLY` skips codegen and the Papyrus steps, and `src/Quest/` holds stubs that keep the Quests save record intact. `cmake/ports/commonlibsse-ng` pins the same CommonLibSSE-NG commit as the upstream registry, with the archive hash GitHub serves today.
