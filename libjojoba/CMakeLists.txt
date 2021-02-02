cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

# LIBVERMILION_CORE
# -----------------

add_compile_definitions(__JOJOBA_INTERNAL__)

# Add vermilion
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../third_party/vermilion ${PROJECT_BINARY_DIR}/third_party/vermilion)
set(JOJOBA_CORE_LIBS ${JOJOBA_CORE_LIBS} vermilion_core)
set(JOJOBA_CORE_INCLUDE_DIRS ${JOJOBA_CORE_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/third_party/vermilion/libvermilion/include)

add_library(jojoba_core SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/core/src/jojoba.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core/src/instance.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core/src/stringutils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core/src/asset_manager.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/core/src/imgui/imgui.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core/src/imgui/imgui_demo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core/src/imgui/imgui_draw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core/src/imgui/imgui_tables.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core/src/imgui/imgui_widgets.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/core/src/imgui/imgui_vermilion.cpp
)
target_include_directories(jojoba_core PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include	
	${JOJOBA_CORE_INCLUDE_DIRS}
)
target_link_libraries(jojoba_core ${JOJOBA_CORE_LIBS})
