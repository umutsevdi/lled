file(GLOB WINDOW_LIBRARY src/*.cpp )
include_directories(../)
include_directories(external/imgui)
add_library(window ${WINDOW_LIBRARY})
target_link_libraries(window
    imgui
    tfd
    common
)

