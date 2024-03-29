include(${KT_BASE_DIR}/Scripts/CMakeMacros.cmake)

set(COMPILED_SOURCES 
    "DebugAllocator.cpp"
    "Concurrency.cpp"
    "File.cpp"
    "FilePath.cpp"
    "Hash.cpp"
    "LinearAllocator.cpp"
    "Logging.cpp"
    "Memory.cpp"
    "Strings.cpp"
    "Timer.cpp"
    "Serialization.cpp"
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
    "Handles.h"
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
    "simd.h"
    "simd_v4b32.h"
    "simd_v4f32.h"
    "Sort.h"
    "Slice.h"
    "StackTrace.h"
    "StaticFunction.h"
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
    "inl/Handles.inl"
    "inl/Hash.inl"
    "inl/HashMap.inl"
    "inl/kt.inl"
    "inl/Mat2.inl"
    "inl/Mat3.inl"
    "inl/Mat4.inl"
    "inl/MathUtil.inl"
    "inl/Memory.inl"
    "inl/Quat.inl"
    "inl/Random.inl"
    "inl/Serialization.inl"
    "inl/simd_v4b32_sse.inl"
    "inl/simd_v4f32_sse.inl"
    "inl/Sort.inl"
    "inl/StaticFunction.inl"
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

if(MSVC)
    target_compile_options(kt PUBLIC /wd4127)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(kt PUBLIC -mcx16)
endif()

target_include_directories(kt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_include_directories(kt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})