#include "File.h"

#include <fstream>
#include <sstream>

namespace Editor
{
    File::File(const char* path, const char* name)
        : Layer("File"), m_Path(path), m_Name(name)
    {
        LOGGER_TRACE("Creating File");
        
        std::ifstream file;
        file.open(m_Path);

        if(file.good())
        {
            std::stringstream ss;
            ss << file.rdbuf();

            m_TextEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Atom());
            m_TextEditor.SetText(ss.str());
        }

        file.close();
    }

    File::~File()
    {

    }

    void File::Update()
    {
        if(m_Show)
        {
            if(!ImGui::Begin(m_Name, &m_Show))
            {
                ImGui::End();
            }

            else
            {
                m_TextEditor.Render(m_Name);
                ImGui::End();
            }
        }
    }
}