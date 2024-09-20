cmake_minimum_required(VERSION 3.28)

project(raptor_workspace)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_POSITION_INDEPENDENT_CODE true)

string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_LC)
string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE_LC)
set(CONFIG_NAME "${SYSTEM_NAME_LC}.${BUILD_TYPE_LC}.${CMAKE_SYSTEM_PROCESSOR}")

include(FetchContent)

# add_subdirectory(gdextension "${CMAKE_BINARY_DIR}/gdextension")
add_subdirectory(raptor "${CMAKE_BINARY_DIR}/raptor")

# for clangd support
set(COMPILE_COMMANDS_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
set(COMPILE_COMMANDS_TARGET "${CMAKE_CURRENT_SOURCE_DIR}/build/compile_commands.json")
file(CREATE_LINK ${COMPILE_COMMANDS_SOURCE} ${COMPILE_COMMANDS_TARGET} SYMBOLIC)