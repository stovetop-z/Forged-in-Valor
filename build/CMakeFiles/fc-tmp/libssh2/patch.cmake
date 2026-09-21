cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

message(VERBOSE "Executing patch step for libssh2")

block(SCOPE_FOR VARIABLES)

execute_process(
  WORKING_DIRECTORY "/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/libssh2-src"
  COMMAND_ERROR_IS_FATAL LAST
  COMMAND  [====[/opt/homebrew/bin/cmake]====] [====[-DLIBSSH2_DIR=/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/libssh2-src]====] [====[-DMODULES_DIR=/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/sfml-src/src/SFML/Network/../../../cmake/Modules]====] [====[-P]====] [====[/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/sfml-src/tools/libssh2/PatchLibssh2.cmake]====]
)

endblock()
