file(GLOB RUNTIME_RES src/*.cpp)
include_directories(../)
include_directories(../../external/lua)
add_library(runtime ${RUNTIME_RES})
target_link_libraries(runtime
    lua
    common
)
