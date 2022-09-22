cmake_minimum_required(VERSION 3.6)

set(project_name app)
project(${project_name})
if (APPLE)
    find_library(GL OpenGL)
    find_library(COCO Cocoa)
    find_library(IOK IOKit)
    find_library(CV CoreVideo)
else ()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
endif ()
# GLFW


#GLU

set(cimgui lib/cimgui)
set(imgui_impl ${cimgui}/imgui/examples)
set(gl3w lib/gl3w)
set(linmath lib/linmath/)
set(glfw lib/glfw-3.3.8)

# library search path
set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/lib/pkgconfig")
set(ENV{LD_LIBRARY_PATH} "ENV{LD_LIBRARY_PATH}:/usr/local/lib")

# build dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -g")

set(CMAKE_VERBOSE_MAKEFILE ON)

# Include dirs
include_directories(
        src
        ${cimgui}
        ${cimgui}/imgui ${imgui_impl}
        ${gl3w}
        ${klib}
        ${linmath}
        ${glfw}
        ${glfw}/include/
        lib/old_polish/src
)

add_subdirectory(${glfw})

file(GLOB
        cimgui_files
        "${cimgui}/*.cpp"
        "${cimgui}/imgui/*.cpp"
        "${imgui_impl}/imgui_impl_glfw.cpp"
        "${imgui_impl}/imgui_impl_opengl3.cpp")

add_library(cimgui ${cimgui_files})
target_link_libraries(cimgui glfw ${GLFW_LIBRARIES})

target_compile_definitions(cimgui PRIVATE
        IMGUI_IMPL_API=extern\ \"C\"
        IMGUI_IMPL_OPENGL_LOADER_GL3W)

file(GLOB source_files "src/*.c")
file(GLOB gl3w_files "${gl3w}/GL/*.c")

file(GLOB glfw_files
        "${glfw}/src/*.c")

add_library(yard
        ${CMAKE_SOURCE_DIR}/lib/old_polish/src/operations.c
        ${CMAKE_SOURCE_DIR}/lib/old_polish/src/graph.c
        ${CMAKE_SOURCE_DIR}/lib/old_polish/src/input.c
        ${CMAKE_SOURCE_DIR}/lib/old_polish/src/yard.c
        )

add_executable(yard_test
        ${CMAKE_SOURCE_DIR}/lib/old_polish/src/main.c)
target_link_libraries(
        yard_test
        yard
)

add_executable(
        ${project_name}
        ${source_files}
        ${gl3w_files}
        "${imgui_impl}/imgui_impl_glfw.cpp"
        "${imgui_impl}/imgui_impl_opengl3.cpp"
        #        ${glfw_files}
)
target_compile_definitions(${project_name} PUBLIC
        IMGUI_IMPL_API=\ )

target_link_libraries(
        ${project_name}
        ${GL}
        ${COCO}
        ${IOK}
        ${CV}
        m
        dl
        cimgui
        glfw
        ${GLFW_LIBRARIES}
        yard

)

