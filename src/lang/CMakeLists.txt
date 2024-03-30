file(GLOB LANG_RES src/*.cpp )
include_directories(../)
include_directories(../../lua/)
add_library(lang ${LANG_RES})
target_link_libraries(lang
    lua
)

