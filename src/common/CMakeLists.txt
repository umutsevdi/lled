file(GLOB COMMON_RES src/*.cpp)
include_directories(../)
include_directories(../../lua/)
add_library(common ${COMMON_RES})
