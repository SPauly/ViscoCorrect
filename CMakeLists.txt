cmake_minimum_required(VERSION 3.5.1)

project(ViscoCorrect)

# compiler configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wformat -fno-var-tracking-assignments -fverbose-asm -fopenmp")
set(CMAKE_RC_COMPILER=windres)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g -fno-omit-frame-pointer -rdynamic")

# option for building with debug features
option(BUILD_DEBUG_FEATURES "Build with debug features" ON)

#includes
include(ExternalProject)

#packages
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

#dependencies
set( GLFW_BUILD_DOCS OFF CACHE BOOL "")
set( GLFW_INSTALL OFF CACHE BOOL "")
set( GLFW_BUILD_TESTS OFF CACHE BOOL "")
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")

add_subdirectory(external/glfw)

add_library(imgui
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_demo.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_draw.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_tables.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp"
)

add_dependencies(imgui glfw)

target_include_directories(imgui PUBLIC
    "${PROJECT_SOURCE_DIR}/external/imgui"
    "${PROJECT_SOURCE_DIR}/external/imgui/backends"
    "${PROJECT_SOURCE_DIR}/external/glfw/include"
)

add_library( implot 
    "${PROJECT_SOURCE_DIR}/external/implot/implot.cpp"
    "${PROJECT_SOURCE_DIR}/external/implot/implot_items.cpp"
    "${PROJECT_SOURCE_DIR}/external/implot/implot_demo.cpp"
)

target_include_directories( implot PUBLIC
    "${PROJECT_SOURCE_DIR}/external/implot/"
)

add_dependencies(implot imgui)
target_link_libraries(implot imgui)

#Application
add_executable(ViscoCorrect
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/Application.cpp"    
    "${PROJECT_SOURCE_DIR}/src/utils/LayerStack.cpp"
    "${PROJECT_SOURCE_DIR}/src/Graph.cpp"
    "${PROJECT_SOURCE_DIR}/src/Project.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/Types.cpp"
)

target_include_directories(ViscoCorrect PUBLIC
    "${PROJECT_SOURCE_DIR}/src/"    
    "${PROJECT_SOURCE_DIR}/src/utils/"    
    "${PROJECT_SOURCE_DIR}/external/imgui"
    "${PROJECT_SOURCE_DIR}/external/imgui/backends"
    "${PROJECT_SOURCE_DIR}/external/implot"
)

target_link_libraries(ViscoCorrect glfw ${GLFW_LIBRARIES})
target_link_libraries(ViscoCorrect OpenGL::GL)
target_link_libraries(ViscoCorrect imgui implot)

# Debugtools
if(BUILD_DEBUG_FEATURES)
    add_library(debug_tools 
        "${PROJECT_SOURCE_DIR}/src/utils/Debugging/DebugTools.cpp"
        "${PROJECT_SOURCE_DIR}/src/utils/Debugging/CurveFitting.cpp"
    )

    target_include_directories(debug_tools PUBLIC   
        "${PROJECT_SOURCE_DIR}/src/utils/"
        "${PROJECT_SOURCE_DIR}/src/utils/Debug"
        "${PROJECT_SOURCE_DIR}/external/imgui"
        "${PROJECT_SOURCE_DIR}/external/implot"
    )

    target_compile_definitions(debug_tools PUBLIC DEBUG_BUILD)
    target_link_libraries(ViscoCorrect debug_tools)
endif()