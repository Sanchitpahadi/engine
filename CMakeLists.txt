cmake_minimum_required(VERSION 3.16)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    core
    assets
    ECS
    ECS/Component
    physics
    physics/Components
    renderer
    renderer/resource
    scene
    ui
    ui/panel

    dev
    dev/GLFW/include
    dev/glm

    imgui
)

file(GLOB_RECURSE SRC
    core/*.cpp
    assets/*.cpp
    ECS/*.cpp
    physics/*.cpp
    renderer/*.cpp
    renderer/resource/*.cpp
    scene/*.cpp
    ui/*.cpp
    ui/panel/*.cpp
    src/*.cpp
)


set(IMGUI_SRC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(engine
    ${SRC}
    ${IMGUI_SRC}
    core/glad.c
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(engine
    OpenGL::GL
    glfw
    dl
)

add_custom_command(TARGET engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:engine>/Resources
)