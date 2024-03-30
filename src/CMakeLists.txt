file(GLOB SOURCES main.cpp)

add_subdirectory(runtime)
add_subdirectory(gl)

include_directories(./)

add_executable(revdb ${SOURCES})
# pull in common dependencies
target_link_libraries(revdb
    runtime
    gl
)

