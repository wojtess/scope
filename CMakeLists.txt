cmake_minimum_required(VERSION 3.9.1)

project(scope)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

add_executable(scope
    src/main.cpp
    src/gui.cpp
    src/shader.cpp

    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_demo.cpp

    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)


target_include_directories(scope PRIVATE lib/imgui lib/imgui/backends)
target_include_directories(scope PRIVATE lib/glfw/include)
target_include_directories(scope PRIVATE lib/glew/include)
target_link_libraries(scope PRIVATE
    glfw
    GL
    ${CMAKE_SOURCE_DIR}/lib/glew/lib/libGLEW.a
    )