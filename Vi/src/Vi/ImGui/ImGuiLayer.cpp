#include "Vipch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

//#define IMGUI_IMPL_API
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Vi/Core/Application.h"

// Temporary
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui_internal.h>


#if Threading == 1

static void glfw_error_callback(int error, const char* description)
{
    VI_CORE_ERROR("Glfw Error {0}: {1}\n", error, description);
}

Vi::ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer")
{
    /*m_LayerThread = std::thread([&]
        {

            while (!m_DettachedSuccess)
            {

                if (m_ShouldAttach && !m_AttachedSuccess)
                {
                    OnAttach();
                    m_ShouldAttach = false;
                    m_AttachedSuccess = true;
                }
                if (m_ShouldRun && m_AttachedSuccess)
                {
                    Begin();
                    OnImGuiRender();
                    End();

                }
                if (m_ShouldDetach && m_AttachedSuccess) {
                    OnDetach();
                    m_ShouldDetach = false;
                    m_DettachedSuccess = true;
                    m_AttachedSuccess = false; 
                }


            }


        });
*/

}



void Vi::ImGuiLayer::OnAttach()
{
    VI_PROFILE_FUNCTION();

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


    // Create window with graphics context
    m_ImGuiWindow = glfwCreateWindow(640, 480, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (m_ImGuiWindow == NULL)
        VI_CORE_ASSERT(m_ImGuiWindow, "ImGui couldn't create window")

    glfwMakeContextCurrent(m_ImGuiWindow);
    glfwSwapInterval(0); // Enable vsync


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
   
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic(); // to fix // to do // dark
    SetImGuiTheme(m_DarkImgui);
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
      //  style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    //Application& app = Application::Get();
 // //  GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
    //GLFWwindow* window = glfwCreateWindow(640, 480, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
   
    glfwMakeContextCurrent(m_ImGuiWindow);
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(m_ImGuiWindow, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void Vi::ImGuiLayer::Begin()
{
    VI_PROFILE_FUNCTION();
    
    
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
   
    //
    //VI_CORE_WARN("ImGui Begin() ThreadID : {0}", std::hash<std::thread::id>{}(std::this_thread::get_id()));

    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Vi::ImGuiLayer::OnImGuiRender() 

{
    VI_CORE_WARN("ImGui OnImGuiRender() ThreadID : {0}", std::hash<std::thread::id>{}(std::this_thread::get_id()));
 //   const auto& io = ImGui::GetIO();
 //   m_CapturingMouse = io.WantCaptureMouse;
 //   m_CapturingKeyboard = io.WantCaptureKeyboard;


    // dock
    bool tr = false;
    bool* p_open = &tr;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }


    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();
 
            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();


 
            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                *p_open = false;
            ImGui::EndMenu();
        }
 
 
        ImGui::EndMenuBar();
    }

    ImGui::End();
    
   // LayerStack m_LayerStack = Application::Get().GetLayerStack();
   // for (Layer* layer : m_LayerStack)
   // {
   //     if (layer == this) continue;
   //         layer->OnImGuiRender();
   // }
   

}


void Vi::ImGuiLayer::End()
{
    VI_PROFILE_FUNCTION();
   //
    //VI_CORE_WARN("ImGui End() ThreadID : {0}", std::hash<std::thread::id>{}(std::this_thread::get_id()));

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGuiIO& io = ImGui::GetIO();
       // Application& app = Application::Get();
    io.DisplaySize = ImVec2((float)640, (float)480);

    ImGui::Render();
    //Rendering
    //glfwPollEvents();
    //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    int display_w, display_h;
    glfwGetFramebufferSize(m_ImGuiWindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }


    glfwSwapBuffers(m_ImGuiWindow);

}

void Vi::ImGuiLayer::OnDetach()
{
    VI_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_ImGuiWindow);
}

void Vi::ImGuiLayer::SetImGuiTheme(bool bStyleDark) {

    m_DarkImgui = bStyleDark;

    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    if (bStyleDark) {

        

        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.94f);
       
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.43f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.32f, 0.32f, 0.32f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.59f, 0.59f, 0.59f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.59f, 0.59f, 0.59f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.59f, 0.59f, 0.59f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.59f, 0.59f, 0.95f);
        colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
        colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
        colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
        colors[ImGuiCol_DockingPreview] =  ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.59f, 0.59f, 0.59f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
    else {
        

        colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.59f, 0.59f, 0.59f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.59f, 0.59f, 0.59f, 0.78f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0.54f, 0.54f, 0.60f);
        colors[ImGuiCol_Button] = ImVec4(0.59f, 0.59f, 0.59f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.59f, 0.59f, 0.59f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.59f, 0.59f, 0.95f);
        colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.90f);
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
        colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
        colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.30f, 0.30f, 0.30f, 0.26f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.68f, 0.68f, 0.68f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.59f, 0.59f, 0.59f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.59f, 0.59f, 0.59f, 0.95f);
        colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    }
    


}

void Vi::ImGuiLayer::JoinThread()
{
    //m_LayerThread.join();
}

#else

static void glfw_error_callback(int error, const char* description)
{
    VI_CORE_ERROR("Glfw Error {0}: {1}\n", error, description);
}

Vi::ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer")
{
}



void Vi::ImGuiLayer::OnAttach()
{
    VI_PROFILE_FUNCTION();

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        VI_CORE_ASSERT(window, "ImGui couldn't create window")

        glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic(); // to fix // to do // dark
    SetImGuiTheme(m_DarkImgui);
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        //  style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    //Application& app = Application::Get();
 // //  GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
    //GLFWwindow* window = glfwCreateWindow(640, 480, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);

    glfwMakeContextCurrent(window);
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void Vi::ImGuiLayer::OnDetach()
{
    VI_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Vi::ImGuiLayer::OnImGuiRender()
{
    VI_CORE_WARN("ImGui OnImGuiRender() ThreadID : {0}", std::hash<std::thread::id>{}(std::this_thread::get_id()));
    const auto& io = ImGui::GetIO();



    m_CapturingMouse = io.WantCaptureMouse;

    m_CapturingKeyboard = io.WantCaptureKeyboard;
}



void Vi::ImGuiLayer::Begin()
{
    VI_PROFILE_FUNCTION();


    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    VI_CORE_WARN("ImGui Begin() ThreadID : {0}", std::hash<std::thread::id>{}(std::this_thread::get_id()));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Vi::ImGuiLayer::End()
{
    VI_PROFILE_FUNCTION();
    VI_CORE_WARN("ImGui End() ThreadID : {0}", std::hash<std::thread::id>{}(std::this_thread::get_id()));
    ImGuiIO& io = ImGui::GetIO();
    // Application& app = Application::Get();
    io.DisplaySize = ImVec2((float)640, (float)480);

    //Rendering
    //glfwPollEvents();
    //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    {
        {

        }
    }
}

void Vi::ImGuiLayer::SetImGuiTheme(bool bStyleDark) {

    m_DarkImgui = bStyleDark;

    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    if (bStyleDark) {



        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.94f);

        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.43f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.32f, 0.32f, 0.32f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.59f, 0.59f, 0.59f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.59f, 0.59f, 0.59f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.59f, 0.59f, 0.59f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.59f, 0.59f, 0.95f);
        colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
        colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
        colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
        colors[ImGuiCol_DockingPreview] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.59f, 0.59f, 0.59f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
    else {


        colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.59f, 0.59f, 0.59f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.59f, 0.59f, 0.59f, 0.78f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0.54f, 0.54f, 0.60f);
        colors[ImGuiCol_Button] = ImVec4(0.59f, 0.59f, 0.59f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.59f, 0.59f, 0.59f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.59f, 0.59f, 0.95f);
        colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.90f);
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
        colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
        colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.30f, 0.30f, 0.30f, 0.26f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.68f, 0.68f, 0.68f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.59f, 0.59f, 0.59f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.59f, 0.59f, 0.59f, 0.95f);
        colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    }



}

#endif