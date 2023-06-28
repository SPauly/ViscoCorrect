# this file contains all the file lists for the core and each target

set(_visco_srcs
    ${PROJECT_SOURCE_DIR}/src/application.cpp
    ${PROJECT_SOURCE_DIR}/src/calculator.cpp    
    ${PROJECT_SOURCE_DIR}/src/graph.cpp
    ${PROJECT_SOURCE_DIR}/src/manage_project.cpp
    ${PROJECT_SOURCE_DIR}/src/project.cpp
    ${PROJECT_SOURCE_DIR}/src/util/properties.cpp
)

set(_visco_hdrs
    ${PROJECT_SOURCE_DIR}/src/application.h
    ${PROJECT_SOURCE_DIR}/src/application_base.h
    ${PROJECT_SOURCE_DIR}/src/calculator.h    
    ${PROJECT_SOURCE_DIR}/src/graph.h
    ${PROJECT_SOURCE_DIR}/src/manage_project.h
    ${PROJECT_SOURCE_DIR}/src/project.h
    ${PROJECT_SOURCE_DIR}/src/util/event.h
    ${PROJECT_SOURCE_DIR}/src/util/input_units.h
    ${PROJECT_SOURCE_DIR}/src/util/mathematical_functions.h
    ${PROJECT_SOURCE_DIR}/src/util/properties.h
)

set(_visco_imgui_glfw_srcs
    ${PROJECT_SOURCE_DIR}/src/frontend/imgui_glfw/application_impl_imgui_glfw.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/imgui_glfw/graph_impl_imgui_glfw.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/util_frontend/layerstack.cpp
)

set(_visco_imgui_glfw_hdrs
    ${PROJECT_SOURCE_DIR}/src/frontend/imgui_glfw/application_impl_imgui_glfw.h
    ${PROJECT_SOURCE_DIR}/src/frontend/imgui_glfw/graph_impl_imgui_glfw.h
    ${PROJECT_SOURCE_DIR}/src/frontend/util_frontend/layer.h
    ${PROJECT_SOURCE_DIR}/src/frontend/util_frontend/layerstack.h
    ${PROJECT_SOURCE_DIR}/external/imgui
    ${PROJECT_SOURCE_DIR}/external/imgui/backends
    ${PROJECT_SOURCE_DIR}/external/implot
)

set(_visco_imgui_glfw_main
    ${PROJECT_SOURCE_DIR}/src/frontend/imgui_glfw/main_impl_imgui_glfw.cpp
)
