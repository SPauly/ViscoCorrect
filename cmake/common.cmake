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

set(glfw3_DIR ${SUP_GRPC_BINARIES_DIR}/glfw/lib/cmake/glfw3)
set(Protobuf_DIR ${SUP_GRPC_BINARIES_DIR}/protobuf/lib/cmake/protobuf)
set(c-ares_DIR ${SUP_GRPC_BINARIES_DIR}/c-ares/lib/cmake/c-ares)
set(re2_DIR ${SUP_GRPC_BINARIES_DIR}/re2/lib/cmake/re2)
set(ZLIB_ROOT ${SUP_GRPC_BINARIES_DIR}/zlib)
set(absl_DIR ${SUP_GRPC_BINARIES_DIR}/absl/lib/cmake/absl)
set(gRPC_DIR ${SUP_GRPC_BINARIES_DIR}/grpc/lib/cmake/grpc)
set(sup_grpc_proto ${SUP_ROOT_FOLDER}/build/utils/)

# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()
# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()