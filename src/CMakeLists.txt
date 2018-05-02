#include(CodeCoverage)
SET(GCC_COMPILE_FLAGS "-Wall -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

add_subdirectory(lib/libconfig)
find_package(Curses REQUIRED)

if (NOT CURSES_FOUND)
    message("FATAL ERROR: curses not found")
endif()


set(SRC
        actors.cpp
        cui.cpp
        game.cpp
        menu.cpp
        main.cpp
        map.cpp
        event_system.cpp
        colors.cpp)

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS} lib/libconfig/lib)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${CURSES_LIBRARIES} libconfig++)

