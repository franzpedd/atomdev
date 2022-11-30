#pragma once

#include <Atom.h>

namespace Editor
{
    class Menubar : public Atom::Layer
    {
    public:

        // constructor
        Menubar();

        // destructor
        virtual ~Menubar();

        // layer loop
        virtual void Update() override;
    };
}