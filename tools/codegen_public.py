# Runs CODEGEN.py without the Quests MCM, whose sources are not public.
# Writes DCurses_MCM.psc, translationData/ (MCM strings and the event-text JSON), Version.txt, the version
# in include/form_ids.h and the CODEGEN regions of src/Settings.cpp, src/Devices.cpp and src/Translation.cpp.
# Usage: python tools/codegen_public.py
import os

os.chdir(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
os.makedirs("translationData", exist_ok=True)

src = open("CODEGEN.py").read()
quest = 'FullMCMFileGeneration(r"include\\Quest\\QuestSettings.h"'
start = src.index(quest)
src = src[:start] + src[src.index("\n", start):]
exec(compile(src, "CODEGEN.py", "exec"))
