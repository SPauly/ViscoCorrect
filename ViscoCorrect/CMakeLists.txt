cmake_minimum_required(VERSION 3.5.1)

project(ViscoCorrect)

set(OpenGL_GL_PREFERENCE GLVND)

#link_directories(${imgui_LIB})

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
)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} ${imgui_DIR}/imgui.lib)

