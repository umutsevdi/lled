#!/usr/bin/env sh
STOP=

if ! dpkg -l | grep -q '^ii  libglfw3-dev'; then
    sudo apt-get install -y libglfw3-dev
fi

if [ ! -f "external/lua/onelua.c" ]; then
    echo "Lua submodule is missing!"
    exit 1
fi

cat >external/lua/CMakeLists.txt << EOF
file(GLOB LUA_SRC onelua.c)
add_library(lua \${LUA_SRC})

target_include_directories(lua PRIVATE ./)
target_compile_options(lua PRIVATE
    -O3
    -ldl -lreadline
    -fno-stack-protector
    -fno-common
    -march=native
)
EOF


cat >external/imgui/CMakeLists.txt << EOF
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


cat >external/tinyfiledialogs/CMakeLists.txt <<EOF
file(GLOB TFD_RES 
tinyfiledialogs.c
more_dialogs/tinyfd_moredialogs.c
)
add_library(tfd \${TFD_RES})
include_directories(./)
include_directories(./more_dialogs)
EOF

echo "CMakeLists.txt files have been generated. Now comment out the main function in the lua.c"
echo "Press any key to open the text editor $STOP"
read STOP
${VISUAL-${EDITOR-nano}} external/lua/lua.c
