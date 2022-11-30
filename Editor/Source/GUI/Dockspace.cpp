#include "Dockspace.h"

namespace Editor
{
    Dockspace::Dockspace()
        : Layer("Dockspace")
    {
        LOGGER_TRACE("Creating Dockspace")
    }

    Dockspace::~Dockspace()
    {

    }

    void Dockspace::Update()
    {
        ImGui::DockSpaceOverViewport();
    }
}