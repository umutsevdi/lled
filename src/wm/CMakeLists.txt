file(GLOB GRAPHIC_LIBRARY src/*.cpp )
include_directories(../)
include_directories(../../imgui/)
include_directories(../../imgui/backends/)
add_library(wm ${GRAPHIC_LIBRARY})
target_link_libraries(wm
    imgui
)

