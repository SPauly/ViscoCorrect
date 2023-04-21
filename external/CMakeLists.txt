cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wformat")
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

# Build gRPC  
if(SUP_GRPC_PRECOMPILED_BINARIES)
  message(STATUS "SUP: External: Using precompiled binaries for gRPC at: ${SUP_GRPC_BINARIES_DIR}")
  #set all necessary variables or run some tests

else()
#Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# changed by the author of this repository 
  message(STATUS "External: Building gRPC from source at: ${SUP_ROOT_FOLDER}/external/grpc")
  # Builds absl project from the git submodule.
  ExternalProject_Add(absl
    PREFIX absl
    SOURCE_DIR "${SUP_ROOT_FOLDER}/external/grpc/third_party/abseil-cpp"
    CMAKE_CACHE_ARGS
          -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=TRUE
          -DCMAKE_INSTALL_PREFIX:PATH=${SUP_GRPC_BINARIES_DIR}/absl
  )

  # Builds c-ares project from the git submodule.
  ExternalProject_Add(c-ares
    PREFIX c-ares
    SOURCE_DIR "${SUP_ROOT_FOLDER}/external/grpc/third_party/cares/cares"
    CMAKE_CACHE_ARGS
          -DCARES_SHARED:BOOL=OFF
          -DCARES_STATIC:BOOL=ON
          -DCARES_STATIC_PIC:BOOL=ON
          -DCMAKE_INSTALL_PREFIX:PATH=${SUP_GRPC_BINARIES_DIR}/c-ares
  )

  # Builds protobuf project from the git submodule.
  ExternalProject_Add(protobuf
    PREFIX protobuf
    SOURCE_DIR "${SUP_ROOT_FOLDER}/external/grpc/third_party/protobuf/cmake"
    CMAKE_CACHE_ARGS
          -Dprotobuf_BUILD_TESTS:BOOL=OFF
          -Dprotobuf_WITH_ZLIB:BOOL=OFF
          -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF
          -DCMAKE_INSTALL_PREFIX:PATH=${SUP_GRPC_BINARIES_DIR}/protobuf
  )

  # Builds re2 project from the git submodule.
  ExternalProject_Add(re2
    PREFIX re2
    SOURCE_DIR "${SUP_ROOT_FOLDER}/external/grpc/third_party/re2"
    CMAKE_CACHE_ARGS
          -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=TRUE
          -DCMAKE_INSTALL_PREFIX:PATH=${SUP_GRPC_BINARIES_DIR}/re2
  )

  # Builds zlib project from the git submodule.
  ExternalProject_Add(zlib
    PREFIX zlib
    SOURCE_DIR "${SUP_ROOT_FOLDER}/external/grpc/third_party/zlib"
    CMAKE_CACHE_ARGS
          -DCMAKE_INSTALL_PREFIX:PATH=${SUP_GRPC_BINARIES_DIR}/zlib
  )

  # the location where protobuf-config.cmake will be installed varies by platform
  if (WIN32)
    set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${SUP_GRPC_BINARIES_DIR}/protobuf/cmake")
  else()
    set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${SUP_GRPC_BINARIES_DIR}/protobuf/lib/cmake/protobuf")
  endif()

  # if OPENSSL_ROOT_DIR is set, propagate that hint path to the external projects with OpenSSL dependency.
  set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "")
  if (OPENSSL_ROOT_DIR)
    set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "-DOPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR}")
  endif()

  # Builds gRPC based on locally checked-out sources and set arguments so that all the dependencies
  # are correctly located.
  ExternalProject_Add(grpc
    PREFIX grpc
    SOURCE_DIR "${SUP_ROOT_FOLDER}/external/grpc"
    CMAKE_CACHE_ARGS
          -DgRPC_INSTALL:BOOL=ON
          -DgRPC_BUILD_TESTS:BOOL=OFF
          -DgRPC_BUILD_MSVC_MP_COUNT:STRING=-1
          -DgRPC_PROTOBUF_PROVIDER:STRING=package
          -DgRPC_PROTOBUF_PACKAGE_TYPE:STRING=CONFIG
          -DProtobuf_DIR:PATH=${SUP_GRPC_BINARIES_DIR}/protobuf/lib/cmake/protobuf
          -DgRPC_RE2_PROVIDER:STRING=package
          -Dre2_DIR:STRING=${SUP_GRPC_BINARIES_DIR}/re2/lib/cmake/re2
          -DgRPC_ZLIB_PROVIDER:STRING=package
          -DZLIB_ROOT:STRING=${SUP_GRPC_BINARIES_DIR}/zlib
          -DgRPC_ABSL_PROVIDER:STRING=package
          -Dabsl_DIR:STRING=${SUP_GRPC_BINARIES_DIR}/absl/lib/cmake/absl
          -DgRPC_CARES_PROVIDER:STRING=package
          -Dc-ares_DIR:PATH=${SUP_GRPC_BINARIES_DIR}/c-ares/lib/cmake/c-ares
          -DgRPC_SSL_PROVIDER:STRING=package
          ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
          -DCMAKE_INSTALL_PREFIX:PATH=${SUP_GRPC_BINARIES_DIR}/grpc
    DEPENDS c-ares protobuf re2 zlib absl
  )

endif() #SUP_GRPC_PRECOMPILED_BINARIES