cmake_minimum_required(VERSION 3.5.1)

set (BUILDLIBS "multifit_nlinear" CACHE STRING "")
set (NO_AMPL_BINDINGS ON CACHE BOOL "")
set (GSL_DISABLE_TESTS ON CACHE BOOL "")

add_subdirectory(external/gsl)

add_library(debug_tools SHARED
${_visco_debug_srcs}
)

target_include_directories(debug_tools PUBLIC
    ${_visco_debug_includes}
)

add_dependencies(debug_tools gsl ${_visco_TARGET_LIBRARIES})
target_compile_definitions(debug_tools PUBLIC DEBUG_BUILD)

target_link_libraries(debug_tools PUBLIC imgui implot gsl gslcblas)
