#include "Editor.h"

#include "GUI/Example.h"

namespace Editor
{
    Editor::Editor()
    {
        LOGGER_TRACE("Creating Editor");
    }

    Editor::~Editor()
    {
        
    }

    void Editor::Init()
    {
        m_GUI->Add(new Example);
    }
}