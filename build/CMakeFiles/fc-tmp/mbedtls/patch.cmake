cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

message(VERBOSE "Executing patch step for mbedtls")

block(SCOPE_FOR VARIABLES)

execute_process(
  WORKING_DIRECTORY "/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/mbedtls-src"
  COMMAND_ERROR_IS_FATAL LAST
  COMMAND  [====[/opt/homebrew/bin/cmake]====] [====[-DMBEDTLS_DIR=/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/mbedtls-src]====] [====[-P]====] [====[/Users/stovetop/Projects/personal/Forged in Valor/build/_deps/sfml-src/tools/mbedtls/PatchMbedTLS.cmake]====]
)

endblock()
