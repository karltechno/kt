macro(kt_default_build_settings)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKT_DEBUG=1")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DKT_DEBUG=0")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DKT_DEBUG=0")

	if(MSVC)
		add_definitions(-D_HAS_EXCEPTIONS=0 -D_CRT_SECURE_NO_WARNINGS=1)
		add_definitions(/W4)
		add_definitions(/WX)
	endif()
	
endmacro()