#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.

if env["platform"] == "windows":
    env.Append(LIBPATH=['lib/SDL2/'])
    env.Append(LIBS=['SDL2main','SDL2'])
    env.Append(CPPPATH=['include/'])

elif env["platform"] == "linux":
    env.Append(CPPPATH=['include/'])
    env.Append(LINKFLAGS='-lSDL2')
    #env.Append(LIBS="libSDL2")

env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

library = env.SharedLibrary(
    "bin/libffbplugin{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)
