#pragma once

#include <Atom.h>

#include "OpenedFiles.h"
#include "Sidemenu.h"
#include "Terminal.h"
#include "Example.h"

namespace Editor
{
    class Menubar : public Atom::Layer
    {
    public:

        // constructor
        Menubar(Atom::SharedRef<Atom::Compiler>& compiler, Sidemenu* sidemenu, OpenedFiles* openedfiles, Example* example);

        // destructor
        virtual ~Menubar();

        // layer loop
        virtual void Update() override;

    public:
    
    private:

        Atom::SharedRef<Atom::Compiler>& m_Compiler;
        OpenedFiles* m_OpenedFiles;
        Sidemenu* m_Sidemenu;
        Example* m_Example;
    };
}