cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

message(VERBOSE "Executing patch step for flac")

block(SCOPE_FOR VARIABLES)

execute_process(
  WORKING_DIRECTORY "/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/flac-src"
  COMMAND_ERROR_IS_FATAL LAST
  COMMAND  [====[/opt/homebrew/bin/cmake]====] [====[-DFLAC_DIR=/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/flac-src]====] [====[-P]====] [====[/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/sfml-src/tools/flac/PatchFLAC.cmake]====]
)

endblock()
