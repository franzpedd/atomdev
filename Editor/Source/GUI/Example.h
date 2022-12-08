#pragma once

#include <Atom.h>

namespace Editor
{
    class Example : public Atom::Layer
    {
    public:

        // constructor
        Example();

        // destructor
        virtual ~Example();

        // layer loop
        virtual void Update() override;

        // returns if example is active/shown
        bool IsActive() { return m_Show; }
        
    private:

        bool* m_Show = nullptr;

    };
}