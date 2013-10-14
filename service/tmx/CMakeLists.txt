CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(angelworks-service-tmx)

add_subdirectory(lib)

include_directories(
  include
)

file(GLOB SOURCES src/*.cpp)

add_library(angelworks-service-tmx
    ${SOURCES}
)
