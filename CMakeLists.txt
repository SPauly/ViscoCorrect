cmake_minimum_required(VERSION 3.5.1)

project(ViscoCorrect_Superbuild)

# compiler configuration

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wformat -fno-var-tracking-assignments -fverbose-asm -fopenmp")
set(CMAKE_RC_COMPILER=windres)

set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g -fno-omit-frame-pointer -rdynamic")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


#includes
include(costum-config)
include(ExternalProject)

# Build third_party_dependencies
if(SUP_THIRDPARTY_BUILD)
message(STATUS "Adding Third Party Dependencies")

  ExternalProject_Add(external
      PREFIX external
      SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external"
      BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/external"
      INSTALL_COMMAND ""
      CMAKE_CACHE_ARGS
          -DSUP_ROOT_FOLDER:PATH=${SUP_ROOT_FOLDER}
          -DSUP_GLFW_DIR:PATH=${SUP_GLFW_DIR}
          -DCMAKE_INSTALL_PREFIX:PATH=${SUP_ROOT_FOLDER}/build/external
  )
endif() #SUP_THIRDPARTY_BUILD

# Build utilities
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
          -DCMAKE_INSTALL_PREFIX:PATH=${SUP_ROOT_FOLDER}/build/utils
  )
endif() #SUP_UTILS_BUILD

# Main Application
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
          -Dglfw3_DIR:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/external/glfw/lib/cmake/glfw3
          -Dimgui_DIR:PATH=${CMAKE_CURRENT_SOURCE_DIR}/build/external/$<CONFIG>
          -DCMAKE_INSTALL_PREFIX:PATH=${SUP_ROOT_FOLDER}/build/ViscoCorrect
      DEPENDS external 
  )
endif() #SUP_APPLICATION_BUILD