# Minimum-gRPC-ImGUI-ClientServerTEMPLATE

This repository is still in the making. Any contributions (issues, feature request, etc.) is welcome!!!
## Usage
Press "Use this template" or clone the repository if you want to contribute.
The recommended way is not to use preinstalled gRPC binaries but instead build it together as submodule with all its dependencies. This way it is ensured that all the files are where they expected and the versions can easily be updated. 
  - open /cmake/costum-config.cmake
  - set `SUP_GRPC_PRECOMPILED_BINARIES` to `OFF`
  - run `git submodule add https://github.com/grpc/grpc.git external` this should take a while
  - now you can build the example by letting cmake build all Projects (this should build: third_party_dependencies(gRPC, glfw, ImGUI), utils(protofiles), server and client)
  
If you have the gRPC binaries installed locally you can follow the steps below (Note: this way is not tested and not recommended; hence the build system might not find the files it needs):
  - open /cmake/costum-config.cmake
  - set `SUP_GRPC_PRECOMPILED_BINARIES` to `ON`
  - set `SUP_GRPC_BINARIES_DIR` to the path of the main folder of the binary build
  - for VSCode go into /.vscode/c_cpp_properties.json and add the path to grpc to the include path
 
### Manual setup
Since SETUP.sh is not yet functional the setup has to be done manually.
  - open /cmake/costum-config.cmake
  - change `SUP_PROJECT_NAME` to your projects name
  - change `SUP_ROOT_FOLDER` to your repositories root folder 
  - change /utils/Your_Project_protocol.proto by exchanging `Your_Project` Note: the `_protocol.proto` ending is essential for the build system
  - If you want to use the build-in Ping Demo and application structure: Rename every instance of `Your_Project` with your projects name (you can use tools built into vscode for example)
  - build all Projects
 
## Notes:
Please do not hesitate to open an issue in case you encounter any problems! I'm glad to help.

Use this build system at your own risk! (has only been tested on Windows so far)
