#include "Example.h"

namespace Editor
{
    Example::Example()
        : Layer("Example GUI Layer")
    {
        LOGGER_TRACE("Creating Example");
        
    }

    Example::~Example()
    {

    }

    void Example::Update()
    {
        if (m_Show)
        {
            ImGui::Begin("##style", &m_Show);
            ImGui::ShowStyleEditor();
            ImGui::End();
        }
    }
}