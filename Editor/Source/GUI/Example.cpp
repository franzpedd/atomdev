#include "Example.h"

namespace Editor
{
    Example::Example()
        : Layer("Example GUI Layer")
    {
        LOGGER_TRACE("Creating Example GUI Object");
        
    }

    Example::~Example()
    {

    }

    void Example::Update()
    {
        ImGui::ShowDemoWindow();
    }
}