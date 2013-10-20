CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(angelworks-service-tmx)

add_subdirectory(lib)

include_directories(
  include
  ${tmxparser_SOURCE_DIR}/include
)


file(GLOB SOURCES src/*.cpp)

add_library(angelworks-service-tmx
    ${SOURCES}
    ${AS_ADDONS}/scriptarray/scriptarray.cpp
)

target_link_libraries(angelworks-service-tmx tmxparser)
