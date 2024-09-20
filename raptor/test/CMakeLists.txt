project(raptor.test)

include(FetchContent)

set(CATCH_CONFIG_CONSOLE_WIDTH "100")

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
)

FetchContent_MakeAvailable(Catch2)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_executable("${PROJECT_NAME}"
    ${SOURCES})

target_link_libraries("${PROJECT_NAME}" PRIVATE
    raptor
    Catch2::Catch2WithMain)

target_include_directories("${PROJECT_NAME}" PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    get_property(TARGET raptor PROPERTY INCLUDE_DIRECTORIES))

include(CTest)
add_test(NAME "${PROJECT_NAME}" COMMAND "${PROJECT_NAME}")