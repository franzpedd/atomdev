#pragma once

#include <Atom.h>

#include "OpenedFiles.h"

namespace Editor
{
    class Menubar : public Atom::Layer
    {
    public:

        // constructor
        Menubar(OpenedFiles* openedfiles);

        // destructor
        virtual ~Menubar();

        // layer loop
        virtual void Update() override;
    
    private:

        OpenedFiles* m_OpenedFiles;
    };
}