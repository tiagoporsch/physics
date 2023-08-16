cmake_minimum_required(VERSION 3.27)
project(physics)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

file(GLOB SRCS CONFIGURE_DEPENDS "include/*.h" "src/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "include/")
target_link_libraries(${CMAKE_PROJECT_NAME} sfml-graphics)