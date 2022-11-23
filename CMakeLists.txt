CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(ClockConfigurator)

set(LIB_NAME "ClockConfig")

add_library(
${LIB_NAME}
#sources
${CMAKE_SOURCE_DIR}/src/serialport.cpp
#includes
${CMAKE_SOURCE_DIR}/include/serialport.h
)

include_directories(
${LIB_NAME}
PUBLIC
${CMAKE_SOURCE_DIR}/include/
)

target_include_directories(
${LIB_NAME}
PUBLIC
${CMAKE_SOURCE_DIR}/include/
)

target_link_libraries (
${LIB_NAME}
PRIVATE
PUBLIC
)

add_executable(${PROJECT_NAME} clock_configurator.cpp)
target_link_libraries(${PROJECT_NAME} ClockConfig)