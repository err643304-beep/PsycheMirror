#include <Python.h>
#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <QApplication>
#include <memory>

#include "../../../src/navbar.hpp"

int main(int argc, char** argv) {
    QApplication qt_app(argc, argv);
    Py_Initialize();

    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "PyWGL - Tabbed Interface", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        glfwTerminate();
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui::StyleColorsDark();
    
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.TabRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(8, 6);
    style.ItemSpacing = ImVec2(8, 6);
    
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.0f, 0.5f, 0.3f, 0.4f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.6f, 0.35f, 0.6f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.5f, 0.3f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.0f, 0.5f, 0.3f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.6f, 0.35f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.7f, 0.4f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.0f, 0.5f, 0.3f, 0.8f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.6f, 0.35f, 0.9f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.7f, 0.4f, 1.0f);
    colors[ImGuiCol_Text] = ImVec4(0.0f, 1.0f, 0.53f, 1.0f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.0f, 0.5f, 0.27f, 1.0f);
    colors[ImGuiCol_Border] = ImVec4(0.0f, 0.7f, 0.4f, 0.5f);
    colors[ImGuiCol_Separator] = ImVec4(0.0f, 0.5f, 0.3f, 0.5f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 1.0f, 0.53f, 1.0f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.0f, 0.7f, 0.4f, 1.0f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 1.0f, 0.53f, 1.0f);
    colors[ImGuiCol_Tab] = ImVec4(0.0f, 0.4f, 0.25f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.0f, 0.6f, 0.35f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.7f, 0.4f, 1.0f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    auto navbar = std::make_unique<NavBar>();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        qt_app.processEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        navbar->render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.04f, 0.04f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);
    glfwTerminate();

    if (Py_IsInitialized()) Py_Finalize();
    
    return 0;
}
