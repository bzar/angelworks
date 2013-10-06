CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(angelworks-service-timer)

include_directories(
  include
)

file(GLOB SOURCES src/*.cpp)

add_library(angelworks-service-timer
    ${SOURCES}
)
