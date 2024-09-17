project(gdextension)

include(FetchContent)

FetchContent_Declare(
    GDExtension
    GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
    GIT_TAG godot-4.3-stable
)

FetchContent_MakeAvailable(GDExtension)

set(GODOT_ENABLE_EXCEPTIONS, true)

find_package(OpenMP REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    get_property(TARGET raptor PROPERTY INCLUDE_DIRECTORIES))

target_link_libraries(${PROJECT_NAME} PUBLIC
    raptor
    godot::cpp
    OpenMP::OpenMP_CXX)


set(BIN_OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CONFIG_NAME}")
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy_if_different
    "$<TARGET_FILE:${PROJECT_NAME}>" "${BIN_OUTPUT}$<TARGET_FILE_SUFFIX:${PROJECT_NAME}>"
    MAIN_DEPENDENCY ${PROJECT_NAME}
    )
