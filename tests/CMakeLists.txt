
add_executable(test_app
    test_app.cc
)

target_link_libraries(test_app
    PRIVATE
        fptf
)

find_package(PythonInterp 3 REQUIRED)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/TestFptf.py
    ${CMAKE_CURRENT_BINARY_DIR}/TestFptf.py
    COPYONLY
)

add_test(
    NAME TestFptf
    COMMAND ${PYTHON_EXECUTABLE} -m unittest TestFptf
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
