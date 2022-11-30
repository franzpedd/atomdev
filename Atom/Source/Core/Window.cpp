#include "Window.h"
#include "Util/Logger.h"

#define GLFW_INCLUDE_NONE 
#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace Atom
{
    SharedRef<Window> Window::Create(WindowSpecification& specs)
    {
        return CreateSharedPointer<Window>(specs);
    }

    Window::Window(WindowSpecification& specs)
        : m_Specification(specs)
    {
        LOGGER_TRACE("Creating window %s with %d:%d dimensions", m_Specification.Title, m_Specification.Width, m_Specification.Height);
        
        ASSERT(glfwInit() == 1, "Failed to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        if(specs.Maximized) glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        else glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);

        #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        m_Window = glfwCreateWindow(m_Specification.Width, m_Specification.Height, m_Specification.Title, nullptr, nullptr);
        ASSERT(m_Window != nullptr, "Failed to create window");

        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(m_Specification.VerticalSync);

        ASSERT(gladLoadGL(glfwGetProcAddress) != 0, "Failed to load glad");
        
        glfwSetWindowUserPointer(m_Window, reinterpret_cast<void *>(this));
        SetCallbacks();
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::Update()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void Window::SetCallbacks()
    {
        // internal error
        glfwSetErrorCallback([](int error, const char* desc)
        {
            LOGGER_WARN("GLFW Warning %d:%s", error, desc);
        });

        // framebuffer changed dimensons
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            Window* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

            WindowSpecification specs = win->GetSpecification();
            specs.Width = width;
            specs.Height = height;

            win->SetSpecification(specs);

            glViewport(0, 0, width, height);
        });

        // currentsize changed dimensions
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            Window* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            
            WindowSpecification specs = win->GetSpecification();
            specs.Width = width;
            specs.Height = height;

            win->SetSpecification(specs);
        });
    }
}