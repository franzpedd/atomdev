#include "OpenedFiles.h"

#include <string>
#include <sstream>

namespace Editor
{
    OpenedFiles::OpenedFiles(Atom::SharedRef<Atom::GUI>& gui)
        : Layer("Opened Files"), m_GUI(gui)
    {
        LOGGER_TRACE("Creating Opened Files");
        
    }

    OpenedFiles::~OpenedFiles()
    {
        m_Files.clear();
    }

    void OpenedFiles::Update()
    {
        std::ostringstream title;
        title << ICON_FA_CODE;
        title << " Bem vindo";

        // verify active windows
        size_t count = 0;
        for (size_t i = 0; i < m_Files.size(); i++)
        {
            if (m_Files[i]->GetActive()) count++;
        }

        if (count == 0)
        {
            ImGui::Begin(title.str().c_str());
            ShowDefaultMessage();
            ImGui::End();

            return;
        }

        // update windows
        else
        {
            for (auto file : m_Files)
            {
                file->Update();
            }
        }
    }

    void OpenedFiles::ShowDefaultMessage()
    {
        std::stringstream message;
        message << "Este projeto foi desenvolvido por Edson e Felipe durante a disciplina de Compiladores" << std::endl;
        message << "com o intuito de implementar etapas do processo de compilação" << std::endl;
        message << "As etapas são:" << std::endl;
        message << "Gramática" << std::endl;
        message << "Análise léxica" << std::endl;
        message << "Tabela de símbolos" << std::endl;
        message << "Análise sintática" << std::endl;
        message << "Análise semântica" << std::endl;
        message << std::endl;
        message << "Esta IDE se trata de um extra com o objetivo de englobar a linguagem de programção Atom" << std::endl;
        message << "Uma linguagem criada por ambos alunos para englobar as etapas dentro de uma IDE" << std::endl;
        message << std::endl;
        message << "Info:" << std::endl;
        message << "Python 3.10 com tqdm e requests: usado para baixar as dependencias" << std::endl;
        message << "Premake5: gerador de projeto para MakeFile, Visual Studio e xCode" << std::endl;
        message << "GLAD: loader para OpenGL" << std::endl;
        message << "GLFW: criar janelas em desktops para Windows, Linux e macOS" << std::endl;
        message << "ImGui: interface grafica imediata "<< std::endl;
        message << "ImGuiColorTextEdit: editor de texto para ImGui" << std::endl;
        message << "IconFontCppHeaders: Awesome font header para C++" << std::endl;
        message << std::endl;
        message << "Use o menu de diretorio para abrir um arquivo e edita-lo" << std::endl;
        message << "Use o menu principal para navegar" << std::endl;
        message << "Use o botao Compilar para compilar arquivo ou projeto" << std::endl;

        ImGui::Text(message.str().c_str());
    }

    void OpenedFiles::Open(const char* path, const char* filename)
    {
        if(m_Files.size() >= MAX_OPENED_FILES)
        {
            LOGGER_WARN("%d is the maximum files opened at a given time", MAX_OPENED_FILES);
            return;
        }

        File* ref = nullptr;
        for (auto file : m_Files)
        {
            if (file->GetPath() == path)
            {
                ref = file;
            }
        }

        if (ref == nullptr)
        {
            m_Files.push_back(new File(path, filename));
        }

        m_Files[m_Files.size() - 1]->SetActive(true);
        m_LastOpenedFile = m_Files[m_Files.size() - 1];
    }

    void OpenedFiles::Close(const char* path, const char* filename)
    {

    }
}