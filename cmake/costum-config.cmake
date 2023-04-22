# Main Variables
set(SUP_PROJECT_NAME "ViscoCorrect" CACHE STRING "Name of the main Project" FORCE)
get_filename_component(SUP_ROOT_FOLDER_par ${CMAKE_SOURCE_DIR} DIRECTORY)
set(SUP_ROOT_FOLDER "${SUP_ROOT_FOLDER_par}/ViscoCorrect")

# Build Variables
set(SUP_THIRDPARTY_BUILD ON CACHE BOOL "Build Third Party Dependencies" FORCE)
set(SUP_APPLICATION_BUILD ON CACHE BOOL "Build Application" FORCE)
set(SUP_UTILS_BUILD OFF CACHE BOOL "Generate and compile the utilities" FORCE)

# Third_Party_Depends Variables
set(SUP_GLFW_DIR "${SUP_ROOT_FOLDER}/external/glfw" CACHE PATH "GLFW Location" FORCE)
