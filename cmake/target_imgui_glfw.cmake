cmake_minimum_required(VERSION 3.5.1)

project(ViscoCorrectFrontend)

link_directories("${PROJECT_BINARY_DIR}/lib/frontend/imgui_glfw")    

#packages
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# Configure GLFW
set( GLFW_BUILD_DOCS OFF CACHE BOOL "")
set( GLFW_INSTALL OFF CACHE BOOL "")
set( GLFW_BUILD_TESTS OFF CACHE BOOL "")
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")

add_subdirectory(${PROJECT_SOURCE_DIR}/external/glfw)

add_library(imgui
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_demo.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_draw.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_tables.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp"
)

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

# Add the dependencies
add_dependencies(imgui glfw)
add_dependencies(implot imgui)

target_link_libraries(implot imgui)

# set the libraries for linking
set(_visco_TARGET_LIBRARIES 
    implot    
    imgui
    glfw 
    ${GLFW_LIBRARIES}
    OpenGL::GL
)