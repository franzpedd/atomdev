#include "OpenedFile.h"

#include <string>
#include <sstream>

namespace Editor
{
    OpenedFile::OpenedFile()
        : Layer("Opened File")
    {
        LOGGER_TRACE("Creating Opened File");
        
    }

    OpenedFile::~OpenedFile()
    {

    }

    void OpenedFile::Update()
    {
        std::ostringstream title;
        title << ICON_FA_CODE;
        title << " Bem vindo";

        ImGui::Begin(title.str().c_str());

        if(m_ShowDefaultWindow)
        {
            ShowDefaultMessage();
        }

        else
        {
            
        }
        
        // update texeditor here

        ImGui::End();
    }

    void OpenedFile::ShowDefaultMessage()
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
        message << "Use o botao Compilar para rodar as etapas no arquivo atual" << std::endl;

        ImGui::Text(message.str().c_str());
    }

    void OpenedFile::OpenFile(const char* path)
    {
        LOGGER_TRACE("Opening %s", path);
        m_ShowDefaultWindow = false;
    }
}