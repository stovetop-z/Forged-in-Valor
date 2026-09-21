cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

message(VERBOSE "Executing patch step for vorbis")

block(SCOPE_FOR VARIABLES)

execute_process(
  WORKING_DIRECTORY "/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/vorbis-src"
  COMMAND_ERROR_IS_FATAL LAST
  COMMAND  [====[/opt/homebrew/bin/cmake]====] [====[-DVORBIS_DIR=/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/vorbis-src]====] [====[-P]====] [====[/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/sfml-src/tools/vorbis/PatchVorbis.cmake]====]
)

endblock()
