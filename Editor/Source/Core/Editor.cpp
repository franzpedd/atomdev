#include "Editor.h"

namespace Editor
{
    Editor::Editor()
    {
        LOGGER_TRACE("Creating Editor");

        m_Example = new Example();
        m_Dockspace = new Dockspace();
        m_Menubar = new Menubar();
        m_OpenedFile = new OpenedFile();
        m_Sidemenu = new Sidemenu(m_OpenedFile);
    }

    Editor::~Editor()
    {
        delete m_Sidemenu;
        delete m_OpenedFile;
        delete m_Menubar;
        delete m_Dockspace;
        delete m_Example;
    }

    void Editor::Init()
    {
        m_GUI->Add(m_Example);
        m_GUI->Add(m_Dockspace);
        m_GUI->Add(m_Menubar);
        m_GUI->Add(m_OpenedFile);
        m_GUI->Add(m_Sidemenu);
    }
}