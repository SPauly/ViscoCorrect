  cmake_minimum_required(VERSION 3.5.1)

  project(ViscoCorrect)

  # compiler configuration
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED True)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wformat -fno-var-tracking-assignments -fverbose-asm -fopenmp")
  set(CMAKE_RC_COMPILER=windres)

  set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g -fno-omit-frame-pointer -rdynamic")


  #includes
  include(ExternalProject)

  #packages
  set(OpenGL_GL_PREFERENCE GLVND)

  find_package(OpenGL REQUIRED)


  #dependencies
  set( GLFW_BUILD_DOCS OFF CACHE BOOL "")
  set( GLFW_INSTALL OFF CACHE BOOL "")
  set( GLFW_BUILD_TESTS OFF CACHE BOOL "")
  set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")

  add_subdirectory(external/glfw)

  add_library(imgui
        "${PROJECT_SOURCE_DIR}/external/imgui/imgui.cpp"
        "${PROJECT_SOURCE_DIR}/external/imgui/imgui_demo.cpp"
        "${PROJECT_SOURCE_DIR}/external/imgui/imgui_draw.cpp"
        "${PROJECT_SOURCE_DIR}/external/imgui/imgui_tables.cpp"
        "${PROJECT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp"
        "${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp"
        "${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp"
  )

  add_dependencies(imgui glfw)

  target_include_directories(imgui PUBLIC
      "${PROJECT_BINARY_DIR}"
      "${PROJECT_SOURCE_DIR}/external/imgui"
      "${PROJECT_SOURCE_DIR}/external/imgui/backends"
      "${PROJECT_SOURCE_DIR}/external/glfw/include"
  )

  #Application
  add_executable(ViscoCorrect
      "${PROJECT_SOURCE_DIR}/src/main.cpp"
      "${PROJECT_SOURCE_DIR}/src/Application.cpp"    
      "${PROJECT_SOURCE_DIR}/src/LayerStack.cpp"
      "${PROJECT_SOURCE_DIR}/src/Viewport.cpp"
  )

  target_include_directories(ViscoCorrect PUBLIC
      "${PROJECT_BINARY_DIR}"
      "${PROJECT_SOURCE_DIR}/src/"
      "${PROJECT_SOURCE_DIR}/external/imgui"
      "${PROJECT_SOURCE_DIR}/external/imgui/backends"
  )

  target_link_libraries(ViscoCorrect glfw ${GLFW_LIBRARIES})
  target_link_libraries(ViscoCorrect OpenGL::GL)
  target_link_libraries(ViscoCorrect imgui)
