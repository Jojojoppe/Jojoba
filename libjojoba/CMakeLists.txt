cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

# LIBVERMILION_CORE
# -----------------

add_compile_definitions(__JOJOBA_INTERNAL__)

add_library(jojoba_core SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/core/src/jojoba.cpp
)
target_include_directories(jojoba_core PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include	
	${JOJOBA_CORE_INCLUDE_DIRS}
)
target_link_libraries(jojoba_core ${JOJOBA_CORE_LIBS})
