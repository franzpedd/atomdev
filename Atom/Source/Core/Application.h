#pragma once

#include "GUI.h"
#include "Window.h"

namespace Atom
{
    class Application
    {
    public:

        // constructor
        Application();

        // destructor
        virtual ~Application();

        // returns the window object
        inline SharedRef<Window>& GetWindow() { return m_Window; }

        // returns the general user interface
        inline SharedRef<GUI>& GetGUI() { return m_GUI; }
    
    public:

        // initialization and setups
        virtual void Init() = 0;
        
        // render loop
        void Run();
    
    protected:

        SharedRef<Window> m_Window;
        SharedRef<GUI> m_GUI;
    };
}