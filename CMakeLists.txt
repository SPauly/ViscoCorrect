# ViscoCorrect - Correction factors for centrifugal pumps
# Copyright (C) 2023  Simon Pauly
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
# Contact via <https://github.com/SPauly/ViscoCorrect> 

cmake_minimum_required(VERSION 3.5.1)

project(ViscoCorrect)

# compiler configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RC_COMPILER=windres)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od")
set(CMAKE_CXX_FLAGS_RELEASE "/O2")  

# option for building with debug features
option(BUILD_DEBUG_FEATURES "Build with debug features" OFF)

# Load file lists
include(${PROJECT_SOURCE_DIR}/src/file_lists.cmake)

# Set frontend variable
set(_visco_FRONTEND "imgui_glfw") # Change this to the desired frontend

# Construct the target-specific variable names
set(_visco_TARGET_SRC "_visco_${_visco_FRONTEND}_srcs")
set(_visco_TARGET_INCLUDES "_visco_${_visco_FRONTEND}_includes")
set(_visco_TARGET_MAIN "_visco_${_visco_FRONTEND}_main")

# Set the target-specific CMake file based on the chosen frontend
set(_visco_TARGET_CMAKE_FILE "target_${_visco_FRONTEND}.cmake")

# send Status
message(STATUS "Building with frontend: ${_visco_FRONTEND}")
message(STATUS "Using Sources and headers: ${_visco_includes} ${${_visco_TARGET_INCLUDES}}")

# Include target-specific CMake file 
include(${PROJECT_SOURCE_DIR}/cmake/${_visco_TARGET_CMAKE_FILE})

# Build Application
add_executable( ViscoCorrect WIN32
    ${${_visco_TARGET_MAIN}}
    ${_visco_srcs}
    ${${_visco_TARGET_SRC}}
)

target_include_directories( ViscoCorrect PUBLIC
    ${_visco_includes}
    ${${_visco_TARGET_INCLUDES}}
)

target_link_libraries( ViscoCorrect
    ${_visco_TARGET_LIBRARIES} # Must be specified in target specifi .cmake
)

# Add dependencies to ViscoCorrect
add_dependencies(ViscoCorrect ${_visco_TARGET_LIBRARIES})

# Set the BUILD_DEBUG_FEATURES to OFF when building in Release mode
target_compile_definitions(ViscoCorrect PRIVATE
    $<$<CONFIG:Release>:BUILD_DEBUG_FEATURES=OFF>
    $<$<CONFIG:Debug>:BUILD_DEBUG_FEATURES=ON>
)


# Debugtools
if(BUILD_DEBUG_FEATURES)
    include(cmake/target_debug_tools.cmake)
    target_link_libraries(ViscoCorrect debug_tools)

    set_target_properties(debug_tools PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/debug"
    )
endif()