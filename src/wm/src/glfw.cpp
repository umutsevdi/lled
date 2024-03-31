#include "wm/display.h"
#include "wm/editor.h"

#include <iostream>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>// Will drag system OpenGL headers

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

bool ctx_loaded = false;
GLFWwindow* window;
std::string glsl_version;

bool lled::is_ready() { return ctx_loaded; }

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error Code: " << error << ", Message: " << description;
}

int _initialize_backend()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// Required on Mac
#else
    // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// 3.0+ only
#endif
    ctx_loaded = true;
    return 0;
}

lled::WindowManager::WindowManager()
{
    if (!ctx_loaded) { _initialize_backend(); }
    // Create window with graphics context
    window =
        glfwCreateWindow(960, 720, "lled", nullptr, nullptr);
    if (window == nullptr) { throw "GLFW couldn't create window"; }
    glfwMakeContextCurrent(window);
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
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    bool show_demo_window = true;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    lled::Editor& editor = lled::Editor::instance();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (show_demo_window) {
            static int counter = 0;
            ImGui::Begin("Hello, world!");
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Button")) { counter++; }
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            ImGui::End();
        }

        bool my_window = true;
        ImGui::Begin("My Window", &my_window);
        ImGui::Text("Hello");
        static int item = 0;
        if (ImGui::Combo("Combo", &item, "Test\0Test2\0Test3\0", -1)) {
            std::cout << item << " is selected" << std::endl;
        }
        ImGui::End();
        if (show_another_window) {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me")) show_another_window = false;
            ImGui::End();
        }
        editor.context();
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
                     clear_color.y * clear_color.w,
                     clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

lled::WindowManager::~WindowManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
lled::WindowManager& lled::WindowManager::instance()
{
    static WindowManager instance;
    return instance;
}
