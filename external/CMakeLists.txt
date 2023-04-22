cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wformat")
set(-DCMAKE_RC_COMPILER=windres)

# Project
project(external)

include(ExternalProject)

#build smaller dependencies (GLFW and ImGUI)
# GLFW
message(STATUS "SUP: External:Adding GLFW")
message(STATUS "SUP: External: DEB: dir is: ${SUP_GLFW_DIR}")

ExternalProject_Add(glfw
  PREFIX glfw
  SOURCE_DIR "${SUP_GLFW_DIR}"
  CMAKE_CACHE_ARGS
        -DCMAKE_CXX_FLAGS:STRING=pkg-config --libs glfw3
        -DCMAKE_INSTALL_PREFIX:PATH=${SUP_ROOT_FOLDER}/build/third_party/glfw
        -DGLFW_BUILD_DOCS:BOOL=OFF
        -DGLFW_BUILD_TESTS:BOOL=OFF 
        -DGLFW_BUILD_EXAMPLES:BOOL=OFF
        -DGLFW_INSTALL:BOOL=ON
)

# ImGUI
message(STATUS "SUP: External: Building imgui")

add_library(imgui
      "${PROJECT_SOURCE_DIR}/imgui/imgui.cpp"
      "${PROJECT_SOURCE_DIR}/imgui/imgui_demo.cpp"
      "${PROJECT_SOURCE_DIR}/imgui/imgui_draw.cpp"
      "${PROJECT_SOURCE_DIR}/imgui/imgui_tables.cpp"
      "${PROJECT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
      "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
      "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
)

add_dependencies(imgui glfw)

target_include_directories(imgui PUBLIC
    "${PROJECT_SOURCE_DIR}/imgui"
    "${PROJECT_SOURCE_DIR}/imgui/backends"
    "${SUP_ROOT_FOLDER}/build/third_party/glfw/include"
)

