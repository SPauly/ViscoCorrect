cmake_minimum_required(VERSION 3.5.1)

project(ViscoCorrect)

# compiler configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wformat -fno-var-tracking-assignments -fverbose-asm -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g -fno-omit-frame-pointer -rdynamic")

# option for building with debug features
option(BUILD_DEBUG_FEATURES "Build with debug features" ON)

# Load file lists
include(${PROJECT_SOURCE_DIR}/cmake/file_lists.cmake)

# Set frontend variable
set(_visco_FRONTEND "imgui_glfw") # Change this to the desired frontend

# Set frontend-specific directory
set(_visco_FRONTEND_DIRECTORY "${PROJECT_SOURCE_DIR}/cmake/")

# Construct the target-specific variable names
set(_visco_TARGET_SRC "_visco_${_visco_FRONTEND}_srcs")
set(_visco_TARGET_HDR "_visco_${_visco_FRONTEND}_hdrs")
set(_visco_TARGET_MAIN "_visco_${_visco_FRONTEND}_main")

# Set the target-specific CMake file based on the chosen frontend
set(_visco_TARGET_CMAKE_FILE "target_${_visco_FRONTEND}.cmake")

# Include target-specific CMake file 
include(${PROJECT_SOURCE_DIR}/cmake/${_visco_TARGET_CMAKE_FILE})

# Build Application
add_executable( ViscoCorrect
    ${${_visco_TARGET_MAIN}}
    ${_visco_srcs}
    ${${_visco_TARGET_SRC}}
)

target_include_directories( ViscoCorrect PUBLIC
    ${_visco_hdrs}
    ${${_visco_TARGET_HDR}}
)

target_link_libraries( ViscoCorrect
    ${_visco_TARGET_LIBRARIES} # Must be specified in target specifi .cmake
)

# Add dependencies to ViscoCorrect
add_dependencies(ViscoCorrect ${_visco_TARGET_LIBRARIES})