#pragma once

#include <Atom.h>

namespace Editor
{
    class Terminal : public Atom::Layer
    {
    public:

        // constructor
        Terminal(Atom::SharedRef<Atom::Compiler>& compiler);

        // destructor
        virtual ~Terminal();

        // layer loop
        virtual void Update() override;

    private:

        Atom::SharedRef<Atom::Compiler>& m_Compiler;
        bool* m_Show = nullptr;
    };
}