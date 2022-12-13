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
        inline bool IsActive() { return m_Show; }

        // returns if example is active/shown
        inline void SetActive(bool value) { m_Show = value; }

        
    private:

        bool m_Show = false;
    };
}