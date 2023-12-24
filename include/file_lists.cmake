# this file contains all the file lists for the core and each target

set(_visco_srcs
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/application.cpp
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/calculator.cpp    
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/graph_base.cpp
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/manage_project.cpp
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/util/properties.cpp
)

set(_visco_hdrs
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/application.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/application_base.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/calculator.h    
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/graph_base.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/manage_project.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/project.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/util/event.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/util/input_units.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/util/mathematical_functions.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/util/properties.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/util/timer.h
)

set(_visco_includes
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/spauly
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/util
)

set(_visco_debug_srcs
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/debug/curve_fitting.cpp
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/debug/debug_tools.cpp
)

set(_visco_debug_hdrs
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/debug/curve_fitting.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/debug/debug_tools.h
)

set(_visco_debug_includes
    ${_visco_includes}
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/debug
    ${PROJECT_SOURCE_DIR}/external/imgui
    ${PROJECT_SOURCE_DIR}/external/imgui/backends
    ${PROJECT_SOURCE_DIR}/external/implot
    ${PROJECT_BINARY_DIR}/external/gsl
)

set(_visco_imgui_glfw_srcs
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/frontend/imgui_glfw/application_impl_imgui_glfw.cpp
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/frontend/imgui_glfw/graph_impl_imgui_glfw.cpp
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/frontend/util_frontend/layerstack.cpp
)

set(_visco_imgui_glfw_hdrs
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/frontend/imgui_glfw/application_impl_imgui_glfw.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/frontend/imgui_glfw/graph_impl_imgui_glfw.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/frontend/util_frontend/layer.h
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/frontend/util_frontend/layerstack.h
    ${PROJECT_SOURCE_DIR}/external/imgui
    ${PROJECT_SOURCE_DIR}/external/imgui/backends
    ${PROJECT_SOURCE_DIR}/external/implot
)

set(_visco_imgui_glfw_includes
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/frontend/imgui_glfw
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/frontend/util_frontend
    ${PROJECT_SOURCE_DIR}/external/imgui
    ${PROJECT_SOURCE_DIR}/external/imgui/backends
    ${PROJECT_SOURCE_DIR}/external/implot
)

set(_visco_imgui_glfw_main
    ${PROJECT_SOURCE_DIR}/src/spauly/viscocorrect/frontend/imgui_glfw/main_impl_imgui_glfw.cpp
)
