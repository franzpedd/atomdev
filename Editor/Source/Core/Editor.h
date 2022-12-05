#pragma once

#include <Atom.h>

#include "GUI/Dockspace.h"
#include "GUI/Example.h"
#include "GUI/File.h"
#include "GUI/Menubar.h"
#include "GUI/OpenedFiles.h"
#include "GUI/Sidemenu.h"
#include "GUI/Terminal.h"

namespace Editor
{
    class Editor : public Atom::Application
    {
    public:

        // constructor
        Editor();

        // destructor
        virtual ~Editor();
    
    public:

        // initialization and setups
        virtual void Init() override;

    private:

        Atom::SharedRef<Atom::Compiler> m_Compiler;
        Example* m_Example;
        Dockspace* m_Dockspace;
        Menubar* m_Menubar;
        OpenedFiles* m_OpenedFiles;
        Sidemenu* m_Sidemenu;
        Terminal* m_Terminal;
    };
}