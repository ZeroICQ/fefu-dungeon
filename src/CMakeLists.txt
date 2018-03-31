#include(CodeCoverage)

SET(GCC_COMPILE_FLAGS "-Wall -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

find_package(Curses REQUIRED)

if (NOT CURSES_FOUND)
    message("FATAL ERROR: curses not found")
endif()


set(SRC main.cpp cui.cpp menu.cpp)

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
