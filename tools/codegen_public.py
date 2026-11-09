# Runs CODEGEN.py without the Quests MCM, whose sources are not public.
# Writes DCurses_MCM.psc, translationData/ and the generated parts of src/Settings.cpp.
# Usage: python tools/codegen_public.py
import os

os.chdir(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
os.makedirs("translationData", exist_ok=True)

src = open("CODEGEN.py").read()
quest = 'FullMCMFileGeneration(r"include\\Quest\\QuestSettings.h"'
start = src.index(quest)
src = src[:start] + src[src.index("\n", start):]
exec(compile(src, "CODEGEN.py", "exec"))
