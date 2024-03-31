#include "wm/display.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>// Will drag system OpenGL headers
                       //

void gl::setup()
{
    auto& instance = gl::WindowManager::instance();
    instance.instance().instance().instance();
    std::cout << "Hello gl";
}
