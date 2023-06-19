cmake_minimum_required(VERSION 3.9.1)

project(scope)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

add_executable(scope
    src/main.cpp
    src/gui.cpp
    src/dataSupply.cpp

    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_demo.cpp

    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp

    lib/implot/implot_items.cpp
    lib/implot/implot.cpp
    lib/implot/implot_demo.cpp

    lib/serial/src/serial.cc
    lib/serial/src/impl/unix.cc
    lib/serial/src/impl/list_ports/list_ports_linux.cc
)


target_include_directories(scope PRIVATE lib/imgui lib/imgui/backends lib/implot)
target_include_directories(scope PRIVATE lib/glfw/include)
target_include_directories(scope PRIVATE lib/glew/include)
target_include_directories(scope PRIVATE lib/serial/include lib/serial/include/serial/impl)
target_link_libraries(scope PRIVATE
    glfw
    GL
    ${CMAKE_SOURCE_DIR}/lib/glew/lib/libGLEW.a
)


add_executable(buffer_test tests/buffer_test.cpp)

enable_testing()

add_test(
    NAME buffer_test
    COMMAND $<TARGET_FILE:buffer_test>
)