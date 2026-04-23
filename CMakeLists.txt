cmake_minimum_required(VERSION 3.10)
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/dev

    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/imgui/backends

    ${CMAKE_SOURCE_DIR}/ui
    ${CMAKE_SOURCE_DIR}/ui/include

    ${CMAKE_SOURCE_DIR}/renderer
    ${CMAKE_SOURCE_DIR}/renderer/include

    ${CMAKE_SOURCE_DIR}/Engine/include

    ${CMAKE_SOURCE_DIR}/src/include



)


set(SOURCES
    Engine/glad.c
    Engine/Window.cpp
    Engine/Camera.cpp

    renderer/Texture.cpp
    renderer/VertexArray.cpp
    renderer/VertexBuffer.cpp
    renderer/VertexBufferLayout.cpp
    renderer/TextRenderer.cpp
    renderer/Renderer.cpp
    renderer/Mesh.cpp
    renderer/OBJLoader.cpp


    ui/Deimgui.cpp
    ui/FileBrowser.cpp


    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp

    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp



    src/main.cpp

)

add_executable(cpp ${SOURCES})


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(cpp
    
    OpenGL::GL
    glfw
    dl
)


add_custom_command(TARGET cpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:cpp>/Resources
)