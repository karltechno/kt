include(../Scripts/CMakeMacros.cmake)

SET(COMPILED_SOURCES 
    "Bench.h"
    "Entry.cpp"
    "HashMapBench.cpp"
    "SortBench.cpp"
)

file(GLOB MAIN_SOURCES "*.*")

include_directories("/")

kt_default_build_settings()

add_executable(kt_bench ${COMPILED_SOURCES} ${MAIN_SOURCES})
target_link_libraries(kt_bench kt)