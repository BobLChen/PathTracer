cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

project(GLSL-Pathtracer)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DNOMINMAX=1")

add_definitions(-DEMBED_RES_PATH=\"${CMAKE_SOURCE_DIR}/\")

if (WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_WINDOWS=1")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_MAC=1")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework QuartzCore")
	set(CMAKE_XCODE_GENERATE_SCHEME ON)
	set(XCODE_SCHEME_ENVIRONMENT ON)
elseif (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_LINUX=1")
endif ()

set(ALL_LIBS
	${OPENGL_LIBRARY}
	Core
	glfw
	glad
	imgui
	imguizmo
)

include_directories(
	src/
	external/glm/
	external/glfw/include/
	external/glad/include/
	external/imgui/
	external/imguizmo/
)

find_package(OpenGL REQUIRED)

add_subdirectory(src)
add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/imgui)
add_subdirectory(external/imguizmo)

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.*")
foreach(file ${files})
	SET(ASSETS
		${ASSETS}
		${file}
	)
endforeach()
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Resources" FILES ${ASSETS})

add_executable(PathTracer
	src/Main.cpp
	${ASSETS}
)
target_link_libraries(PathTracer ${ALL_LIBS})