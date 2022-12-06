#include "Editor.h"

namespace Editor
{
    Editor::Editor()
    {
        LOGGER_TRACE("Creating Editor");

        ImGui::SetCurrentContext(m_GUI->GetContext());

        m_Compiler = Atom::Compiler::Create();
        m_Example = new Example();
        m_Dockspace = new Dockspace();
        m_OpenedFiles = new OpenedFiles(m_GUI);
        m_Terminal = new Terminal(m_Compiler);
        m_Sidemenu = new Sidemenu(m_Compiler, m_OpenedFiles);
        m_Menubar = new Menubar(m_Compiler, m_Sidemenu, m_OpenedFiles);
    }

    Editor::~Editor()
    {
        delete m_Menubar;
        delete m_Sidemenu;
        delete m_Terminal;
        delete m_OpenedFiles;
        delete m_Dockspace;
        delete m_Example;
    }

    void Editor::Init()
    {
        m_GUI->Add(m_Example);
        m_GUI->Add(m_Dockspace);
        m_GUI->Add(m_OpenedFiles);
        m_GUI->Add(m_Sidemenu);
        m_GUI->Add(m_Menubar);
        m_GUI->Add(m_Terminal);
    }
}