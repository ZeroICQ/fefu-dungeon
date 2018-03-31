#include(CodeCoverage)

SET(GCC_COMPILE_FLAGS "-Wall -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

set(SRC main.cpp)

add_executable(${PROJECT_NAME} ${SRC})
