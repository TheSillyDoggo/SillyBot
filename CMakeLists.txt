cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(SillyBot VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/SillyBot.cpp
    src/SillyPopup.cpp
    src/Hooks/MenuLayer.cpp
    src/Hooks/PauseLayer.cpp
    src/Hooks/EndLevelLayer.cpp
    src/Recorder/AudioRecorder.cpp
    src/Recorder/VideoRecorder.cpp
    src/UI/ReplayManagerDropdown.cpp
    src/UI/LoadMacroPopup.cpp
    src/UI/SaveMacroPopup.cpp
    src/UI/MacroCell.cpp
    # Add your cpp files here
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

CPMAddPackage("gh:maxnut/GDReplayFormat#7a36cf4")

target_link_libraries(${PROJECT_NAME} libGDR ${FFMPEG_LIBS})

setup_geode_mod(${PROJECT_NAME})