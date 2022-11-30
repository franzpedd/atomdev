#include "GUI.h"
#include "Util/Logger.h"
#include "GUI/FontAwesome.h"

#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

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
        this->m_Context = ImGui::CreateContext();

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
}