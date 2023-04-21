# Main Variables
set(SUP_PROJECT_NAME "Your_Project" CACHE STRING "Name of the main Project" FORCE)
set(SUP_PROJECT_NAME_old "Your_Project" CACHE STRING "Only used for SETUP.sh" FORCE)
get_filename_component(SUP_ROOT_FOLDER_par ${CMAKE_SOURCE_DIR} DIRECTORY)
set(SUP_ROOT_FOLDER "${SUP_ROOT_FOLDER_par}/Minimum-gRPC-ImGUI-ClientServerTEMPLATE")
# Build Variables
set(SUP_THIRDPARTY_BUILD ON CACHE BOOL "Build Third Party Dependencies" FORCE)
set(SUP_CLIENT_BUILD ON CACHE BOOL "Build Client" FORCE)
set(SUP_SERVER_BUILD ON CACHE BOOL "Build Server" FORCE)
set(SUP_UTILS_BUILD ON CACHE BOOL "Generate and compile the utilities" FORCE)

# Third_Party_Depends Variables
set(SUP_GRPC_PRECOMPILED_BINARIES ON CACHE BOOL "Use precompiled binaries for gRPC -> expects Path to build folder with all dependencies. See README" FORCE)
set(SUP_GRPC_BINARIES_DIR "${SUP_ROOT_FOLDER}/build/third_party" CACHE PATH "Location of precompiled binaries. Has to be set if option is set to ON" FORCE)
set(SUP_GLFW_DIR "${SUP_ROOT_FOLDER}/external/glfw" CACHE PATH "GLFW Location" FORCE)
