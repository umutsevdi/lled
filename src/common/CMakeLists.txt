file(GLOB COMMON_RES src/*.cpp)
include_directories(../)
add_library(common ${COMMON_RES})
