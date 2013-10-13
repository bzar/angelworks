CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(angelworks-service-rectcollide)

include_directories(
  include
  ${AS_ADDONS}/scripthandle/
)

file(GLOB SOURCES src/*.cpp)

add_library(angelworks-service-rectcollide
    ${AS_ADDONS}/scripthandle/scripthandle.cpp
    ${SOURCES}
)
