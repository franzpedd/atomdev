#include "Terminal.h"

namespace Editor
{
    Terminal::Terminal()
        : Layer("Example GUI Layer")
    {
        LOGGER_TRACE("Creating Terminal");
        
    }

    Terminal::~Terminal()
    {

    }

    void Terminal::Update()
    {
        ImGui::Begin("Output");

        ImGui::Text("In here, stuff will appear soon");

        ImGui::End();
    }
}