cmake_minimum_required(VERSION 3.10)
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------
# Include paths
# -------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/dev
    ${CMAKE_SOURCE_DIR}/Engine/include
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/imgui/backends
)

# -------------------------
# Source files
# -------------------------
set(SOURCES
    Engine/glad.c
    Engine/main.cpp
    Engine/Texture.cpp
    Engine/VertexArray.cpp
    Engine/VertexBuffer.cpp
    Engine/VertexBufferLayout.cpp
    Engine/TextRenderer.cpp
    Engine/Window.cpp
    Engine/Renderer.cpp
    Engine/Mesh.cpp
    Engine/OBJLoader.cpp
    Engine/Camera.cpp
    Engine/Deimgui.cpp
    Engine/FileBrowser.cpp




    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp

    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(cpp ${SOURCES})

# -------------------------
# Libraries
# -------------------------
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(cpp
    
    OpenGL::GL
    glfw
    dl
)


# -------------------------
# Copy texture directory
# -------------------------
add_custom_command(TARGET cpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Engine/texture
    $<TARGET_FILE_DIR:cpp>/texture
)

add_custom_command(TARGET cpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Engine/Resources
    $<TARGET_FILE_DIR:cpp>/Resources
)