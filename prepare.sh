#!/bin/sh

if [ -z "$(apt-get search libglfw3-dev | grep 'installed')" ]; then
    sudo apt-get install libglfw3-dev
fi

if [ ! -f "lua/onelua.c" ]; then
    echo "Lua submodule is missing!"
    exit 1
fi

cat >lua/CMakeLists.txt << EOF
file(GLOB LUA_SRC onelua.c)
add_library(lua \${LUA_SRC})
include_directories(./)
EOF


cat >imgui/CMakeLists.txt << EOF
file(GLOB IMGUI_RES ./*.cpp
./backends/imgui_impl_glfw.cpp
./backends/imgui_impl_opengl3.cpp)

add_library(imgui \${IMGUI_RES})

include_directories(./)
include_directories(./backends/)

# Find and link GLFW
find_package(glfw3 REQUIRED)

# For OpenGL on Linux
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PRIVATE OpenGL::GL glfw)

# For OpenGL on macOS
if(APPLE)
    target_link_libraries(imgui PRIVATE "-framework OpenGL")
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(imgui PRIVATE   )
endif()

# Optional: Copy shader files to build directory
# configure_file(shader.vert shader.vert COPYONLY)
# configure_file(shader.frag shader.frag COPYONLY)
EOF


cat >tinyfiledialogs-code/CMakeLists.txt <<EOF
file(GLOB TFD_RES 
tinyfiledialogs.c
more_dialogs/tinyfd_moredialogs.c
)
add_library(tfd \${TFD_RES})
include_directories(./)
include_directories(./more_dialogs)
EOF

echo "CMakeLists.txt files have been generated. Now comment out the main function in the lua.c"
echo "Press any key to open the text editor"
read x
${VISUAL-${EDITOR-nano}} lua/lua.c
