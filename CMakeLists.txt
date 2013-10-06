CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(angelworks)

add_subdirectory(lib)

include_directories(
  include
  ${kazmath_SOURCE_DIR}/src
  ${glhck_SOURCE_DIR}/include
  ${GLFW_SOURCE_DIR}/include
  ${engineworks_SOURCE_DIR}/include
  ${Angelscript_SOURCE_DIR}/../../include
  ${Angelscript_SOURCE_DIR}/../../../add_on
)

file(GLOB SOURCES src/*.cpp src/**/*.cpp )
add_definitions(-DGLHCK_KAZMATH_FLOAT -DUSE_SINGLE_PRECISION)
list(APPEND CMAKE_CXX_FLAGS -std=c++11)
set(AS_ADDONS ${Angelscript_SOURCE_DIR}/../../../add_on/)

add_library(angelworks 
    ${SOURCES}
    ${AS_ADDONS}/scriptstdstring/scriptstdstring.cpp
)
target_link_libraries(angelworks Angelscript)

add_subdirectory(test)
