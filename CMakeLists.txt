cmake_minimum_required(VERSION 3.2.3)
include(Scripts/CMakeMacros.cmake)

# Create solution if building just kt.
set(KT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(KT_WITH_TESTS ON CACHE BOOL "With Tests")
set(KT_WITH_BENCH ON CACHE BOOL "With Bench")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE INTERNAL "Build Configurations")
    project(kt)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    kt_default_build_settings()
endif()

include_directories("src/")
add_subdirectory("src/kt")

if(${KT_WITH_TESTS})
    add_subdirectory("Tests")
endif()

if(${KT_WITH_BENCH})
    add_subdirectory("Bench")
endif()