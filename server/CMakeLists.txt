cmake_minimum_required(VERSION 3.5.1)
# set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wformat")
set(-DCMAKE_RC_COMPILER=windres)

if (WIN32)
  set(ZLIB_DLL_DIR "${SUP_GRPC_BINARIES_DIR}/zlib/bin")
  set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS "${ZLIB_DLL_DIR}/libzlib.dll")
  include(InstallRequiredSystemLibraries)
  install(PROGRAMS "${ZLIB_DLL_DIR}/libzlib.dll" DESTINATION bin)
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN;${SUP_GRPC_BINARIES_DIR}/zlib/bin")
endif()

project(server)

include(${SUP_ROOT_FOLDER}/cmake/common.cmake)
link_directories(${SUP_ROOT_FOLDER}/build/utils)
link_directories(${SUP_GRPC_BINARIES_DIR}/grpc/lib)
link_directories(${SUP_GRPC_BINARIES_DIR}/protobuf/lib)

add_executable(${PROJECT_NAME}
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/Server.cpp"    
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
    "${SUP_ROOT_FOLDER}/utils"
    "${SUP_GRPC_BINARIES_DIR}/grpc/include"
    "${SUP_GRPC_BINARIES_DIR}/protobuf/include"
)

target_link_libraries(${PROJECT_NAME}
sup_grpc_proto
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF}
)