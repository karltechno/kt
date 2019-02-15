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
    "HandleAllocator.cpp"
    "StackTrace.cpp"
)

set(EXTERNAL_SOURCES
    "External/xxhash.h"
    # "External/xxhash.c" add toggle for inlining?
)

set(HEADER_SOURCES
    "AABB.h"
    "Array.h"
    "Concurrency.h"
    "DebugAllocator.h"
    "File.h"
    "FilePath.h"
    "HandleAllocator.h"
    "Hash.h"
    "HashMap.h"
    "HashMapDeclare.h"
    "kt.h"
    "LinearAllocator.h"
    "Logging.h"
    "Macros.h"
    "Mat2.h"
    "Mat3.h"
    "Mat4.h"
    "MathUtil.h"
    "Memory.h"
    "Platform.h"
    "Quat.h"
    "Random.h"
    "Serialization.h"
    "SIMD.h"
    "Sort.h"
    "StackTrace.h"
    "Strings.h"
    "Timer.h"
    "Vec2.h"
    "Vec3.h"
    "Vec4.h"
)

if(MSVC)
    set(NATVIS_SOURCES
        "kt.natvis"
    )
endif()

set(INL_SOURCES
    "inl/AABB.inl"
    "inl/Array.inl"
    "inl/Concurrency.inl"
    "inl/DebugAllocator.inl"
    "inl/Hash.inl"
    "inl/HashMap.inl"
    "inl/kt.inl"
    "inl/LinearAllocator.inl"
    "inl/Mat2.inl"
    "inl/Mat3.inl"
    "inl/Mat4.inl"
    "inl/MathUtil.inl"
    "inl/Memory.inl"
    "inl/Quat.inl"
    "inl/Random.inl"
    "inl/SIMD.inl"
    "inl/SIMD_SSE.inl"
    "inl/Sort.inl"
    "inl/Strings.inl"
    "inl/Vec2.inl"
    "inl/Vec3.inl"
    "inl/Vec4.inl"
)

source_group("inl" FILES ${INL_SOURCES})
source_group("Source" FILES ${COMPILED_SOURCES} ${NATVIS_SOURCES} ${HEADER_SOURCES})
source_group("External" FILES ${EXTERNAL_SOURCES})

kt_default_build_settings()
add_library(kt ${COMPILED_SOURCES} ${HEADER_SOURCES} ${INL_SOURCES} ${NATVIS_SOURCES} ${EXTERNAL_SOURCES})
target_include_directories(kt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_include_directories(kt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})