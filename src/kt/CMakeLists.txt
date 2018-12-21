include(${KT_BASE_DIR}/Scripts/CMakeMacros.cmake)

set(COMPILED_SOURCES 
    "Concurrency.cpp"
    "File.cpp"
    "FilePath.cpp"
    "Hash.cpp"
    "Logging.cpp"
    "Memory.cpp"
    "Strings.cpp"
    "Timer.cpp"
    "LinearAllocator.cpp"
    "HandleAllocator.cpp"
)

set(EXTERNAL_SOURCES
    "External/xxhash.h"
    # "External/xxhash.c" add toggle for inlining?
)

file(GLOB HEADER_SOURCES "*.h")

if(MSVC)
    file(GLOB NATVIS_SOURCES "*.natvis")
endif()

file(GLOB INL_SOURCES "inl/*.inl")

source_group("inl" FILES ${INL_SOURCES})
source_group("Source" FILES ${COMPILED_SOURCES} ${NATVIS_SOURCES} ${HEADER_SOURCES})
source_group("External" FILES ${EXTERNAL_SOURCES})

include_directories("/")

kt_default_build_settings()
add_library(kt ${COMPILED_SOURCES} ${HEADER_SOURCES} ${INL_SOURCES} ${NATVIS_SOURCES} ${EXTERNAL_SOURCES})