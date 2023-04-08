
add_executable(fptfd
    fptfd.cc
)

add_custom_command(
    OUTPUT fptfd_binary.hh
    COMMAND xxd -i fptfd > fptfd_binary.hh
    DEPENDS fptfd
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(generate_fptfd_binary ALL
    DEPENDS fptfd_binary.hh
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(fptf SHARED
    fptf.cc
)

add_dependencies(fptf
    generate_fptfd_binary
)

target_sources(fptf PUBLIC FILE_SET HEADERS
  BASE_DIRS
    ${CMAKE_SOURCE_DIR}/include
  FILES
    ${CMAKE_SOURCE_DIR}/include/fptf/fptf.hh
    ${CMAKE_SOURCE_DIR}/include/fptf/fptf.h
)

install(
    TARGETS fptf
    LIBRARY DESTINATION lib
    FILE_SET HEADERS
)