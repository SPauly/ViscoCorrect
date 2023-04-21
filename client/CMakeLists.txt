cmake_minimum_required(VERSION 3.5.1)
# set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wformat")
set(-DCMAKE_RC_COMPILER=windres)

project(client)

set(OpenGL_GL_PREFERENCE GLVND)

include(${SUP_ROOT_FOLDER}/cmake/common.cmake)

link_directories(${imgui_LIB})
link_directories(${SUP_ROOT_FOLDER}/build/utils)
link_directories(${SUP_GRPC_BINARIES_DIR}/grpc/lib)
link_directories(${SUP_GRPC_BINARIES_DIR}/protobuf/lib)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/Application.cpp"    
    "${PROJECT_SOURCE_DIR}/src/LayerStack.cpp"    
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}"
    "${SUP_ROOT_FOLDER}/utils"
    "${PROJECT_SOURCE_DIR}/src/"
    "${SUP_ROOT_FOLDER}/external/imgui"
    "${SUP_ROOT_FOLDER}/external/imgui/backends"
    "${SUP_ROOT_FOLDER}/build/thir_party/glfw/include/"
    "${SUP_GRPC_BINARIES_DIR}/grpc/include"
    "${SUP_GRPC_BINARIES_DIR}/protobuf/include"
)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} ${imgui_DIR})
target_link_libraries(${PROJECT_NAME}
sup_grpc_proto
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF}
)


