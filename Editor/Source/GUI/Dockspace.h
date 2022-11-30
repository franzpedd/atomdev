#pragma once

#include <Atom.h>

namespace Editor
{
    class Dockspace : public Atom::Layer
    {
    public:

        // constructor
        Dockspace();

        // destructor
        virtual ~Dockspace();

        // layer loop
        virtual void Update() override;
    };
}