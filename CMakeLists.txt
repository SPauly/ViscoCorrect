cmake_minimum_required(VERSION 3.5.1)
# set the C++ standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wformat")
set(-DCMAKE_RC_COMPILER=windres)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(costum-config)

include(ExternalProject)

project(${SUP_PROJECT_NAME}_Superbuild)

# Build third_party_dependencies
if(SUP_THIRDPARTY_BUILD)
message(STATUS "Adding Third Party Dependencies")

  ExternalProject_Add(Third_Party_Depends
      PREFIX external
      SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external"
      BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/third_party"
      INSTALL_COMMAND ""
      CMAKE_CACHE_ARGS
          -DSUP_ROOT_FOLDER:PATH=${SUP_ROOT_FOLDER}
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
  )
endif() #SUP_UTILS_BUILD

if(SUP_APPLICATION_BUILD)
message(STATUS "Adding Application")

  ExternalProject_Add(ViscoCorrect
      PREFIX ViscoCorrect
      SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${SUP_PROJECT_NAME}"
      BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/${SUP_PROJECT_NAME}"
      INSTALL_COMMAND ""
      BUILD_ALWAYS TRUE
      CMAKE_CACHE_ARGS
          -DSUP_ROOT_FOLDER:PATH=${SUP_ROOT_FOLDER}
          -Dglfw3_DIR:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/third_party/glfw/lib/cmake/glfw3
          -Dimgui_LIB:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/third_party
          -Dimgui_DIR:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/third_party/libimgui.a
      DEPENDS Third_Party_Depends 
  )
endif() #SUP_APPLICATION_BUILD