CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(angelworks-service-glhck)

include_directories(
  include
  ${kazmath_SOURCE_DIR}/src
  ${glhck_SOURCE_DIR}/include
  ${GLFW_SOURCE_DIR}/include
  ${engineworks_SOURCE_DIR}/include
  ${Angelscript_SOURCE_DIR}/../../include
  ${Angelscript_SOURCE_DIR}/../../../add_on
)

file(GLOB SOURCES src/*.cpp)
set(AS_ADDONS ${Angelscript_SOURCE_DIR}/../../../add_on/)

add_library(angelworks-service-glhck
    ${SOURCES}
    ${AS_ADDONS}/scriptarray/scriptarray.cpp
)

target_link_libraries(angelworks-service-glhck glhck)
