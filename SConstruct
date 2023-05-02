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

env.Append(CPPPATH=['include/'])
env.Append(LIBS=['SDL2'])

if env["platform"] == "windows":
    #env.Append(LIBS=['libSDL2main','libSDL2'])
    #env.Append(CPPPATH=['include/'])
    print("Compiling for windows")
    env.Append(LIBPATH=['lib/SDL2/'])
    #env.Append(LIBS=['SDL2main','SDL2'])
    #env.Append(LIBS=['SDL2'])
    env.Append(CPPFLAGS='-mwindows')
    #env.Append(LIBS=' pkg-config --libs sdl2')
    
    #env.Append(CPPFLAGS='--shared')
    #env.Append(CPPFLAGS='$(pkg-config --cflags --libs sdl2)')
    env.Append(LINKFLAGS='-DSDL_MAIN_HANDLED')

#elif env["platform"] == "linux":
    #env.Append(CPPPATH=['include/'])
    #env.Append(LINKFLAGS='-lSDL2')

env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

library = env.SharedLibrary(
    "bin/libffbplugin{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)
