#include "Editor.h"

namespace Editor
{
    Editor::Editor()
    {
        LOGGER_TRACE("Creating Editor");

        m_Example = new Example();
        m_Dockspace = new Dockspace();
        m_Menubar = new Menubar();
        m_OpenedFiles = new OpenedFiles(Application::m_GUI);
        m_Sidemenu = new Sidemenu(m_OpenedFiles);
    }

    Editor::~Editor()
    {
        delete m_Sidemenu;
        delete m_OpenedFiles;
        delete m_Menubar;
        delete m_Dockspace;
        delete m_Example;
    }

    void Editor::Init()
    {
        m_GUI->Add(m_Example);
        m_GUI->Add(m_Dockspace);
        m_GUI->Add(m_Menubar);
        m_GUI->Add(m_OpenedFiles);
        m_GUI->Add(m_Sidemenu);
    }
}