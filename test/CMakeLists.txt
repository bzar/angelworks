CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(angelworks-tests)

include_directories(
  ${angelworks_SOURCE_DIR}/include
  ${kazmath_SOURCE_DIR}/src
  ${glhck_SOURCE_DIR}/include
  ${GLFW_SOURCE_DIR}/include
  ${engineworks_SOURCE_DIR}/include
)

add_executable(angelworks-test-simple
    simple.cpp
)
target_link_libraries(angelworks-test-simple angelworks engineworks glfw 
glhck ${GLFW_LIBRARIES})

