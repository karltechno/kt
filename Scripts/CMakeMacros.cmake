
macro(kt_default_build_settings)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKT_DEBUG=1")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DKT_DEBUG=0")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DKT_DEBUG=0")

	# todo: should be target properties!

	if(MSVC)
		add_definitions(-D_HAS_EXCEPTIONS=0 -D_CRT_SECURE_NO_WARNINGS=1)
		add_compile_options(/W4 /WX /wd4127)
	elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
		add_compile_options(-Wall -Werror -Wextra)
	endif()
	
endmacro()