find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)

IF(NOT LCOV_PATH)
    MESSAGE(FATAL_ERROR "lcov not found! Aborting...")
ENDIF() # NOT LCOV_PATH

IF(NOT GENHTML_PATH)
    MESSAGE(FATAL_ERROR "genhtml not found! Aborting...")
ENDIF() # NOT GENHTML_PATH

add_custom_target(coverage
        COMMAND "${CMAKE_BINARY_DIR}/bin/tests"
        COMMAND ${LCOV_PATH} --directory ${CMAKE_BINARY_DIR}/tests/CMakeFiles/tests.dir --capture --output-file ${CMAKE_BINARY_DIR}/coverage.info
        COMMAND ${GENHTML_PATH} -o ${CMAKE_BINARY_DIR}/coverage/ ${CMAKE_BINARY_DIR}/coverage.info

)
