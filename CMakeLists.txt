cmake_minimum_required(VERSION 3.5.1)
# set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wformat")
set(-DCMAKE_RC_COMPILER=windres)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(costum-config)

include(ExternalProject)

project(${SUP_PROJECT_NAME})

# Build third_party_dependencies
if(SUP_THIRDPARTY_BUILD)
message(STATUS "Adding Third Party Dependencies")
message(STATUS "Using gRPC Location: ${SUP_GRPC_BINARIES_DIR}")

  ExternalProject_Add(Third_Party_Depends
      PREFIX external
      SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external"
      BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/third_party"
      INSTALL_COMMAND ""
      CMAKE_CACHE_ARGS
          -DSUP_ROOT_FOLDER:PATH=${SUP_ROOT_FOLDER}
          -DSUP_GRPC_PRECOMPILED_BINARIES:BOOL=${SUP_GRPC_PRECOMPILED_BINARIES}
          -DSUP_GRPC_BINARIES_DIR:PATH=${SUP_GRPC_BINARIES_DIR}
          -DSUP_GLFW_DIR:PATH=${SUP_GLFW_DIR}
          -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/Third_Party_Depends
  )
endif() #SUP_THIRDPARTY_BUILD

if(SUP_UTILS_BUILD)
message(STATUS "Adding Utilities")

  ExternalProject_Add(utils
      PREFIX utils
      SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/utils"
      BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/utils"
      INSTALL_COMMAND ""
      BUILD_ALWAYS TRUE
      CMAKE_CACHE_ARGS
          -DSUP_ROOT_FOLDER:PATH=${SUP_ROOT_FOLDER}
          -DSUP_PROJECT_NAME:STRING=${SUP_PROJECT_NAME}
          -DSUP_GRPC_BINARIES_DIR:PATH=${SUP_GRPC_BINARIES_DIR}
  )
endif() #SUP_UTILS_BUILD

if(SUP_CLIENT_BUILD)
message(STATUS "Adding Client")

  ExternalProject_Add(client
      PREFIX client
      SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/client"
      BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/client"
      INSTALL_COMMAND ""
      BUILD_ALWAYS TRUE
      CMAKE_CACHE_ARGS
          -DSUP_ROOT_FOLDER:PATH=${SUP_ROOT_FOLDER}
          -Dglfw3_DIR:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/third_party/glfw/lib/cmake/glfw3
          -Dimgui_LIB:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/third_party
          -Dimgui_DIR:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/third_party/libimgui.a
          -DSUP_GRPC_BINARIES_DIR:PATH=${SUP_GRPC_BINARIES_DIR}
      DEPENDS Third_Party_Depends utils
  )
endif() #SUP_CLIENT_BUILD

if(SUP_SERVER_BUILD)
message(STATUS "Adding Server")

  ExternalProject_Add(server
      PREFIX server
      SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/server"
      BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/server"
      INSTALL_COMMAND ""
      BUILD_ALWAYS TRUE
      CMAKE_CACHE_ARGS
          -DSUP_ROOT_FOLDER:PATH=${SUP_ROOT_FOLDER}
          -DSUP_PROJECT_NAME:STRING=${SUP_PROJECT_NAME}
          -DSUP_GRPC_BINARIES_DIR:PATH=${SUP_GRPC_BINARIES_DIR}
      DEPENDS Third_Party_Depends utils
  ) 
endif() #SUP_SERVER_BUILD