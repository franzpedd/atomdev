#include "GUI.h"
#include "Util/Logger.h"
#include "GUI/FontAwesome.h"

#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>
#include <imgui_demo.cpp>

namespace Atom
{
    SharedRef<GUI> GUI::Create(SharedRef<Window>& window)
    {
        return CreateSharedPointer<GUI>(window);
    }

    GUI::GUI(SharedRef<Window>& window)
        : m_Window(window)
    {
        LOGGER_TRACE("Creating General User Interface");

        IMGUI_CHECKVERSION();
        m_Context = ImGui::CreateContext();
        ImGui::SetCurrentContext(m_Context);

        ImGuiIO &io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/OpenSans-Regular.ttf", 20.0f);
        io.IniFilename = "editor.ini";
        io.FontAllowUserScaling = true;

        ImFontConfig cfg;
        cfg.MergeMode = true;
        cfg.GlyphMinAdvanceX = 20.0f; // monospace
        static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        io.Fonts->AddFontFromFileTTF("Assets/Fonts/fontawesome-webfont.ttf", 20.0f, &cfg, icon_ranges);

        ImGui::StyleColorsDark();
        SetDarkTheme();
        SetStyle();

        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    GUI::~GUI()
    {
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }

    void GUI::Update()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for(Layer* layer : m_Objects)
        {
            layer->Update();
        }

        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)m_Window->GetSpecification().Width, (float)m_Window->GetSpecification().Height);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GUI::SetDarkTheme()
    {
        ImGui::StyleColorsDark();
    }
    
    void GUI::SetLightTheme()
    {
        ImGui::StyleColorsLight();
    }

    void GUI::SetStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();

        // Color palette
        ImVec4 _black = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        ImVec4 _white = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        ImVec4 _grey = ImVec4(0.60f, 0.60f, 0.60f, 0.35f);
        ImVec4 _dark = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        ImVec4 _darkgrey = ImVec4(0.23f, 0.23f, 0.23f, 0.35f);
        ImVec4 _lighgrey = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

        // Color array
        style.Colors[ImGuiCol_Text] = _white;
        style.Colors[ImGuiCol_TextDisabled] = _grey;
        style.Colors[ImGuiCol_WindowBg] = _dark;
        style.Colors[ImGuiCol_ChildBg] = _dark;
        style.Colors[ImGuiCol_PopupBg] = _dark;
        style.Colors[ImGuiCol_Border] = _grey;
        style.Colors[ImGuiCol_BorderShadow] = _black;
        style.Colors[ImGuiCol_FrameBg] = _darkgrey;
        style.Colors[ImGuiCol_FrameBgHovered] = _grey;
        style.Colors[ImGuiCol_FrameBgActive] = _grey;
        style.Colors[ImGuiCol_TitleBg] = _darkgrey;
        style.Colors[ImGuiCol_TitleBgActive] = _darkgrey;
        style.Colors[ImGuiCol_TitleBgCollapsed] = _darkgrey;
        style.Colors[ImGuiCol_MenuBarBg] = _darkgrey;
        style.Colors[ImGuiCol_ScrollbarBg] = _darkgrey;
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = _grey;
        style.Colors[ImGuiCol_ScrollbarGrabActive] = _grey;
        style.Colors[ImGuiCol_CheckMark] = _lighgrey;
        style.Colors[ImGuiCol_SliderGrab] = _lighgrey;
        style.Colors[ImGuiCol_SliderGrabActive] = _white;
        style.Colors[ImGuiCol_Button] = _darkgrey;
        style.Colors[ImGuiCol_ButtonHovered] = _grey;
        style.Colors[ImGuiCol_ButtonActive] = _darkgrey;
        style.Colors[ImGuiCol_Header] = _darkgrey;
        style.Colors[ImGuiCol_HeaderHovered] = _grey;
        style.Colors[ImGuiCol_HeaderActive] = _grey;
        style.Colors[ImGuiCol_Separator] = _grey;
        style.Colors[ImGuiCol_SeparatorHovered] = _grey;
        style.Colors[ImGuiCol_SeparatorActive] = _grey;
        style.Colors[ImGuiCol_ResizeGrip] = _darkgrey;
        style.Colors[ImGuiCol_ResizeGripHovered] = _grey;
        style.Colors[ImGuiCol_ResizeGripActive] = _grey;
        style.Colors[ImGuiCol_Tab] = _darkgrey;
        style.Colors[ImGuiCol_TabHovered] = _grey;
        style.Colors[ImGuiCol_TabActive] = _grey;
        style.Colors[ImGuiCol_TabUnfocused] = _grey;
        style.Colors[ImGuiCol_TabUnfocused] = _grey;
        style.Colors[ImGuiCol_TabUnfocusedActive] = _grey;
        style.Colors[ImGuiCol_DockingPreview] = _grey;
        style.Colors[ImGuiCol_DockingEmptyBg] = _grey;
        style.Colors[ImGuiCol_PlotLines] = _white;
        style.Colors[ImGuiCol_PlotLinesHovered] = _grey;
        style.Colors[ImGuiCol_PlotHistogram] = _white;
        style.Colors[ImGuiCol_PlotHistogramHovered] = _grey;
        style.Colors[ImGuiCol_TableHeaderBg] = _dark;
        style.Colors[ImGuiCol_TableBorderStrong] = _darkgrey;
        style.Colors[ImGuiCol_TableBorderLight] = _grey;
        style.Colors[ImGuiCol_TableRowBg] = _black;
        style.Colors[ImGuiCol_TableRowBgAlt] = _white;
        style.Colors[ImGuiCol_TextSelectedBg] = _darkgrey;
        style.Colors[ImGuiCol_DragDropTarget] = _darkgrey;
        style.Colors[ImGuiCol_NavHighlight] = _grey;
        style.Colors[ImGuiCol_NavWindowingHighlight] = _grey;
        style.Colors[ImGuiCol_NavWindowingDimBg] = _grey;
        style.Colors[ImGuiCol_ModalWindowDimBg] = _grey;

        // Style
        style.FrameRounding = 3;
        style.WindowPadding = ImVec2(10.0f, 10.0f);
        style.FramePadding = ImVec2(10.00f, 10.00f);
        style.CellPadding = ImVec2(10.00f, 5.00f);
        style.ItemSpacing = ImVec2(10.00f, 5.00f);
        style.ItemInnerSpacing = ImVec2(5.00f, 5.00f);
        style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
        style.IndentSpacing = 15;
        style.ScrollbarSize = 18;
        style.GrabMinSize = 10;
        style.WindowBorderSize = 0;
        style.ChildBorderSize = 0;
        style.PopupBorderSize = 0;
        style.FrameBorderSize = 0;
        style.TabBorderSize = 0;
        style.WindowRounding = 5;
        style.ChildRounding = 5;
        style.PopupRounding = 0;
        style.ScrollbarRounding = 5;
        style.GrabRounding = 0;
        style.LogSliderDeadzone = 0;
        style.TabRounding = 0;
        style.WindowTitleAlign = ImVec2(0.50f, 0.50f);
        style.WindowMenuButtonPosition = -1;
        style.ColorButtonPosition = 0;
    }
}