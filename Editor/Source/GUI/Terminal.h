#pragma once

#include <Atom.h>

namespace Editor
{
    class Terminal : public Atom::Layer
    {
    public:

        // constructor
        Terminal();

        // destructor
        virtual ~Terminal();

        // layer loop
        virtual void Update() override;
    };
}