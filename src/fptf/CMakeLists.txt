
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
