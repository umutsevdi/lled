file(GLOB SOURCES main.cpp)

add_subdirectory(common)
add_subdirectory(runtime)
add_subdirectory(wm)

include_directories(./)
include_directories(../imgui/)
include_directories(../imgui/backends/)

add_executable(lled ${SOURCES})
# pull in common dependencies
target_link_libraries(lled
    runtime
    wm

)

