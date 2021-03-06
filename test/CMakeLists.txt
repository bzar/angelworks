CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(angelworks-tests)

find_package(Boost REQUIRED system filesystem)
include_directories(
  ${angelworks_SOURCE_DIR}/include
  ${angelworks-service-glhck_SOURCE_DIR}/include
  ${angelworks-service-timer_SOURCE_DIR}/include
  ${angelworks-service-rectcollide_SOURCE_DIR}/include
  ${angelworks-service-tmx_SOURCE_DIR}/include
  ${tmxparser_SOURCE_DIR}/include
  ${kazmath_SOURCE_DIR}/src
  ${glhck_SOURCE_DIR}/include
  ${GLFW_SOURCE_DIR}/include
  ${engineworks_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
)

add_executable(angelworks-test-simple
    simple.cpp
)
target_link_libraries(angelworks-test-simple angelworks engineworks)


add_executable(angelworks-test-glhck
    glhck.cpp
)
target_link_libraries(angelworks-test-glhck angelworks engineworks glfw
glhck angelworks-service-glhck angelworks-service-timer ${GLFW_LIBRARIES} ${Boost_LIBRARIES})

add_executable(angelworks-test-rectcollide
    rectcollide.cpp
)
target_link_libraries(angelworks-test-rectcollide angelworks engineworks glfw
glhck angelworks-service-glhck angelworks-service-timer angelworks-service-rectcollide ${GLFW_LIBRARIES} ${Boost_LIBRARIES})

add_executable(angelworks-test-tmx
    tmx.cpp
)
target_link_libraries(angelworks-test-tmx angelworks engineworks glfw
glhck angelworks-service-glhck angelworks-service-timer angelworks-service-tmx ${GLFW_LIBRARIES} ${Boost_LIBRARIES})

file(COPY img DESTINATION .)
file(COPY tmx DESTINATION .)
