#pragma once
#include "Core.h"
#include "Window.h"
#include "Util/Layer.h"
#include "Util/Memory.h"

#include <imgui.h>

namespace Atom
{
    class API GUI
    {
    public:

        // returns a smart pointer to a new general user interface
        static SharedRef<GUI> Create(SharedRef<Window>& window);

        // constructor
        GUI(SharedRef<Window>& window);

        // destructor
        ~GUI();

        // returns the stack of gui objects
        inline LayerStack& GetObjects() { return m_Objects; }

        // returns the gui context
        inline ImGuiContext* GetContext() { return m_Context; }

    public:

        // adds a new gui object to the stack
        inline void Add(Layer* object) { m_Objects.Push(object); }

        // removes a given object from the stack
        inline void Remove(Layer* object) { m_Objects.Pop(object); }

    public:

        // refreshes the frame
        void Update();

        // changes the theme to dark
        void SetDarkTheme();
        
        // changes the theme to light
        void SetLightTheme();

    private:

        SharedRef<Window>& m_Window;
        LayerStack m_Objects;
        ImGuiContext* m_Context;
    };
}