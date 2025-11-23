cmake_minimum_required(VERSION 3.21)

project(MIDIMapper)
set(CMAKE_CXX_STANDARD 20)

if (PROJECT_IS_TOP_LEVEL)

    #First, we'll add the CMake folder, incase we'll need to find_package later:
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

    #Compile commands, useful for some IDEs like VS-Code
    set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "Minimum OS X deployment version" FORCE)

    option(UniversalBinary "Build universal binary for mac" OFF)

    if (UniversalBinary)
        set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
    endif ()

    #static linking on Windows
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

endif ()
#We 'require' that we need juce. If JUCE isn't found, it will revert to what's in
#CMake/Findjuce.cmake, where you can see how JUCE is brought in/configured
find_package(juce REQUIRED)
find_package(EADataStructures REQUIRED)

#adding any custom modules you might have:
add_subdirectory(Modules)

if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory(Apps)
    add_subdirectory(Plugins)
endif ()


