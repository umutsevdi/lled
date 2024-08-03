file(GLOB GRAPHIC_LIBRARY src/*.cpp )
add_subdirectory(window)
include_directories(../)
include_directories(../../external/imgui)
include_directories(../../external/tinyfiledialogs)
add_library(ui ${GRAPHIC_LIBRARY})
target_link_libraries(ui
    window
    imgui
    tfd
    common
)

