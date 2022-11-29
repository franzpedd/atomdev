#pragma once

#include "Util/Memory.h"

struct GLFWwindow;

namespace Atom
{
    struct WindowSpecification
    {
        const char* Title;
        int Width;
        int Height;
        bool Maximized;
        bool VerticalSync;

        // constructor
        WindowSpecification
        (
            const char* title = "Atom IDE",
            int width = 1920,
            int height = 1080,
            bool maxmized = true,
            bool verticalsync = true
        ) 
        : Title(title), Width(width), Height(height), Maximized(true), VerticalSync(true) {}
    };
    class Window
    {
    public:

        // returns a smart pointer to a new window
        static SharedRef<Window> Create(WindowSpecification& specs);

        // constructor
        Window(WindowSpecification& specs);

        // destructor
        ~Window();

        // returns the glfw's window object
        inline GLFWwindow* GetNativeWindow() { return m_Window; }
    
    public:

        // returns the window's specification
        inline WindowSpecification& GetSpecification() { return m_Specification; }

        // sets new window specifications
        inline void SetSpecification(WindowSpecification specs) { m_Specification = specs; }

    public:

        // returns true when window close event is called
        bool ShouldClose();

        // updates the window events
        void Update();

        // clears the background color
        void Clear();

        // setup the glfw callbacks
        void SetCallbacks();

    private:

        GLFWwindow* m_Window;
        WindowSpecification m_Specification;
    };
}