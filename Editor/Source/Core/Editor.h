#pragma once

#include <Atom.h>

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

    };
}