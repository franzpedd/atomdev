#include "Menubar.h"

#include <string>
#include <sstream>

namespace Editor
{
    Menubar::Menubar()
        : Atom::Layer("Menubar")
    {
        LOGGER_TRACE("Creating Menubar");
    }

    Menubar::~Menubar()
    {

    }

    void Menubar::Update()
    {
        std::ostringstream title;

        if(ImGui::BeginMainMenuBar())
        {
            std::ostringstream project;
            project << ICON_FA_FILE_CODE_O;
            project << " Projeto";

            if(ImGui::BeginMenu(project.str().c_str()))
            {
                ImGui::EndMenu();
            }

            std::ostringstream edit;
            edit << ICON_FA_FILE_O;
            edit << " Editar";

            if(ImGui::BeginMenu(edit.str().c_str()))
            {
                ImGui::EndMenu();
            }

            std::ostringstream compile;
            compile << ICON_FA_PLAY;
            compile << " Compilar";

            if(ImGui::BeginMenu(compile.str().c_str()))
            {
                ImGui::EndMenu();
            }

            ImGui::Separator();

            std::ostringstream help;
            help << ICON_FA_QUESTION_CIRCLE_O;
            help << " Ajuda";

            if(ImGui::BeginMenu(help.str().c_str()))
            {
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }
}