
add_executable(test_app_cpp
    test_app.cc
)

target_link_libraries(test_app_cpp
    PRIVATE
        fptf
)

add_executable(test_app_c
    test_app.c
)

target_link_libraries(test_app_c
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
