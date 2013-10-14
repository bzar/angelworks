CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(tmxparser)

LIST(APPEND CMAKE_MODULE_PATH ${tmxparser_SOURCE_DIR}/cmake)
message(${CMAKE_MODULE_PATH})

find_package(TinyXML REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(
  include
  base64
  ${TinyXML_INCLUDE_DIR}
  ${ZLIB_INCLUDE_DIR}
)

file(GLOB SOURCES src/*.cpp src/base64/*.cpp)
add_library(tmxparser ${SOURCES})
target_link_libraries(tmxparser ${TinyXML_LIBRARIES} ${ZLIB_LIBRARIES})
