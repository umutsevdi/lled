file(GLOB SOURCES main.cpp)

add_subdirectory(common)
add_subdirectory(runtime)
add_subdirectory(ui)

include_directories(./)

add_executable(lled ${SOURCES})
# pull in common dependencies
target_link_libraries(lled
    runtime
    ui
)

