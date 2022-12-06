#include "Terminal.h"

namespace Editor
{
    Terminal::Terminal(Atom::SharedRef<Atom::Compiler>& compiler)
        : Layer("Example GUI Layer"), m_Compiler(compiler)
    {
        LOGGER_TRACE("Creating Terminal");
        
    }

    Terminal::~Terminal()
    {

    }

    void Terminal::Update()
    {
        ImGui::Begin("Output", m_Show, ImGuiWindowFlags_HorizontalScrollbar);

        for (size_t i = 0; i < m_Compiler->GetErrorSystem()->GetFeedbacks().size(); i++)
        {
            ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), m_Compiler->GetErrorSystem()->GetFeedbacks()[i]->Format().c_str());
        }

        for (size_t i = 0; i < m_Compiler->GetErrorSystem()->GetErrors().size(); i++)
        {
            if (m_Compiler->GetErrorSystem()->GetErrors()[i]->Level == Atom::Error::Severity::WARNING)
            {
                ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.1f, 1.0f), m_Compiler->GetErrorSystem()->GetErrors()[i]->Format().c_str());
            }
            
            if (m_Compiler->GetErrorSystem()->GetErrors()[i]->Level == Atom::Error::Severity::FATAL)
            {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.2f, 1.0f), m_Compiler->GetErrorSystem()->GetErrors()[i]->Format().c_str());
            }
        }

        ImGui::End();
    }
}