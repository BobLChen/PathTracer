cmake_minimum_required(VERSION 3.8)

project(IMGUI)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(imgui 
    imconfig.h
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_glfw.h
    imgui_impl_opengl3.cpp
    imgui_impl_opengl3.h
    imgui_internal.h
    imgui_stdlib.cpp
    imgui_stdlib.h
    imgui_widgets.cpp
    imgui.cpp
    imgui.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
)

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)