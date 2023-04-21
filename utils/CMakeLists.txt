cmake_minimum_required(VERSION 3.5.1)
# set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wformat")
set(-DCMAKE_RC_COMPILER=windres)

project(utils)

include(${SUP_ROOT_FOLDER}/cmake/common.cmake)

  
# Proto file
get_filename_component(sup_proto "${SUP_PROJECT_NAME}_protocol.proto" ABSOLUTE)
get_filename_component(sup_proto_path "${sup_proto}" PATH)

# Generated sources
set(sup_proto_srcs "${PROJECT_SOURCE_DIR}/${SUP_PROJECT_NAME}_protocol.pb.cc")
set(sup_proto_hdrs "${PROJECT_SOURCE_DIR}/${SUP_PROJECT_NAME}_protocol.pb.h")
set(sup_grpc_srcs "${PROJECT_SOURCE_DIR}/${SUP_PROJECT_NAME}_protocol.grpc.pb.cc")
set(sup_grpc_hdrs "${PROJECT_SOURCE_DIR}/${SUP_PROJECT_NAME}_protocol.grpc.pb.h")

add_custom_command(
      OUTPUT "${sup_proto_srcs}" "${sup_proto_hdrs}" "${sup_grpc_srcs}" "${sup_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${PROJECT_SOURCE_DIR}"
        --cpp_out "${PROJECT_SOURCE_DIR}"
        -I "${sup_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${sup_proto}"
      DEPENDS "${sup_proto}")

include_directories("${PROJECT_SOURCE_DIR}")

# sup_grpc_proto
add_library(sup_grpc_proto
  ${sup_grpc_srcs}
  ${sup_grpc_hdrs}
  ${sup_proto_srcs}
  ${sup_proto_hdrs})

target_include_directories(sup_grpc_proto PUBLIC
    ${PROJECT_SOURCE_DIR}/
    ${SUP_GRPC_BINARIES_DIR}/protobuf/include/
    ${SUP_GRPC_BINARIES_DIR}/grpc/include/
)
  
target_link_libraries(sup_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})