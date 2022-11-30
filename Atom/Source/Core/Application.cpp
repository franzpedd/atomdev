#include "Application.h"
#include "Util/Logger.h"

namespace Atom
{
    Application::Application()
    {
        LOGGER_TRACE("Creating Application");

        WindowSpecification specs;

        m_Window = Window::Create(specs);
        m_GUI = GUI::Create(m_Window);
    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        while(!m_Window->ShouldClose())
        {
            m_GUI->Update();
            m_Window->Update();
        }
    }
}