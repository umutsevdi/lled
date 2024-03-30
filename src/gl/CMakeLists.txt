file(GLOB GRAPHIC_LIBRARY src/*.cpp )
include_directories(../)
include_directories(../../imgui/)
add_library(gl ${GRAPHIC_LIBRARY})
target_link_libraries(gl
    imgui
)

