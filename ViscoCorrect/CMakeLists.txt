cmake_minimum_required(VERSION 3.5.1)
# set the C++ standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wformat")
set(-DCMAKE_RC_COMPILER=windres)

project(ViscoCorrect)

set(OpenGL_GL_PREFERENCE GLVND)

link_directories(${imgui_LIB})

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(ViscoCorrect
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/Application.cpp"    
    "${PROJECT_SOURCE_DIR}/src/LayerStack.cpp"    
)

target_include_directories(ViscoCorrect PUBLIC
    "${PROJECT_SOURCE_DIR}"
    "${SUP_ROOT_FOLDER}/utils"
    "${PROJECT_SOURCE_DIR}/src/"
    "${SUP_ROOT_FOLDER}/external/imgui"
    "${SUP_ROOT_FOLDER}/external/imgui/backends"
    "${SUP_ROOT_FOLDER}/build/thir_party/glfw/include/"
)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} ${imgui_DIR})


