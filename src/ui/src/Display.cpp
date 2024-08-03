#include "ui/Display.h"

#include <functional>
#include <iostream>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>// Will drag system OpenGL headers

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

static void _glfw_error_callback(int err, const char* desc);
static bool _initialize_backend();

typedef struct {
    bool is_ready;
    GLFWwindow* window;
    int width;
    int height;
    std::string glsl_version;
    std::string name;
} WMContext;

WMContext ctx = {.is_ready = false};

bool lled::is_ready() { return ctx.is_ready; }

void lled::wm::init(std::string name, int width, int height)
{
    ctx.name = name;
    _initialize_backend();
    // Create window with graphics context
    ctx.window =
        glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (ctx.window == nullptr) {
        throw std::runtime_error("GLFW Window Creation Failure");
    }
    glfwMakeContextCurrent(ctx.window);
    glfwSwapInterval(1);// Enable vsync
                        //  // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard /* Enable Keyboard Controls*/
        | ImGuiConfigFlags_NavEnableGamepad /* Enable Gamepad Controls */;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(ctx.window, true);
    ImGui_ImplOpenGL3_Init(ctx.glsl_version.c_str());
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(4, 4);
    style.FrameRounding = 4.0f;
    style.FrameBorderSize = 1.0f;
}

void lled::wm::loop(std::function<void(void)> fn)
{
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    int display_w, display_h;
    while (!glfwWindowShouldClose(ctx.window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        fn();
        ImGui::Render();
        glfwGetFramebufferSize(ctx.window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
                     clear_color.y * clear_color.w,
                     clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(ctx.window);
    }
}

void lled::wm::close()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(ctx.window);
    glfwTerminate();
}

static void _glfw_error_callback(int err, const char* desc)
{
    std::cerr << "GLFW Error Code: " << err << ", Message: " << desc;
}

bool _initialize_backend()
{
    if (ctx.is_ready) { return true; }
    glfwSetErrorCallback(_glfw_error_callback);
    if (!glfwInit()) throw std::runtime_error("GLFW Initialization Failed");

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE);           // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// Required on Mac
#else
    // GL 3.0 + GLSL 130
    ctx.glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// 3.0+ only
#endif
    ctx.is_ready = true;
    return ctx.is_ready;
}
