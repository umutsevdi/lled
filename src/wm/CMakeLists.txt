file(GLOB GRAPHIC_LIBRARY src/*.cpp )
include_directories(../)
include_directories(../../imgui/)
include_directories(../../imgui/backends/)
include_directories(../../tinyfiledialogs-code/)
include_directories(../../tinyfiledialogs-code/more_dialogs)
add_library(wm ${GRAPHIC_LIBRARY})
target_link_libraries(wm
    imgui
    tfd
    common
)

