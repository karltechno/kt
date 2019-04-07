include(${KT_BASE_DIR}/Scripts/CMakeMacros.cmake)

SET(COMPILED_SOURCES
    "catch.hpp"
    "Concurrency.cpp"
    "Containers.cpp"
    "Core.cpp"
    "File.cpp"
    "HashMap.cpp"
    "Math.cpp"
    "String.cpp"
    "TestMain.cpp"
    "Timer.cpp"
    "SIMD.cpp"
    "Sort.cpp"
    "Memory.cpp"
    "Serialization.cpp"
    "Slice.cpp"
    "StaticFunctionTest.cpp"
)


include_directories("/")
kt_default_build_settings()
if(MSVC)
# add exceptions for catch
    remove_definitions(-D_HAS_EXCEPTIONS)
    add_compile_options(/EHsc)
endif()

add_executable(kt_tests ${COMPILED_SOURCES})
target_link_libraries(kt_tests kt)