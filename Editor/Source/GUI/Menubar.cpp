#include "Menubar.h"

#include <string>
#include <sstream>

namespace Editor
{
    Menubar::Menubar(Atom::SharedRef<Atom::Compiler>& compiler, Sidemenu* sidemenu, OpenedFiles* openedfiles)
        : Atom::Layer("Menubar"), m_Compiler(compiler), m_Sidemenu(sidemenu), m_OpenedFiles(openedfiles)
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
            std::ostringstream edit;
            edit << ICON_FA_FILE_CODE_O;
            edit << " Editar";

            bool enabled = m_OpenedFiles->GetLastOpenedFile() != nullptr;
            if(enabled) enabled &= m_OpenedFiles->GetLastOpenedFile()->GetActive();

            TextEditor& texteditor = m_OpenedFiles->GetLastOpenedFile()->GetTextEditor();

            if(ImGui::BeginMenu(edit.str().c_str(), enabled))
            {
                if (ImGui::MenuItem("Desfazer", "ALT-Backspace", nullptr, texteditor.CanUndo()))
                {
                    texteditor.Undo();
                }

                if (ImGui::MenuItem("Refazer", "Ctrl-Y", nullptr, texteditor.CanRedo()))
                {
                    texteditor.Redo();
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Copiar", "Ctrl-C", nullptr, texteditor.HasSelection()))
                {
                    texteditor.Copy();
                }

                if (ImGui::MenuItem("Colar", "Ctrl-V", nullptr, ImGui::GetClipboardText() != nullptr))
                {
                    texteditor.Paste();
                }

                if (ImGui::MenuItem("Recortar", "Ctrl-X", nullptr, texteditor.HasSelection()))
                {
                    texteditor.Cut();
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Deletar", "Del", nullptr, texteditor.HasSelection()))
                {
                    texteditor.Delete();
                }

                if(ImGui::MenuItem("Selecionar Tudo"))
                {
                    texteditor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(texteditor.GetTotalLines(), 0));
                }

                ImGui::EndMenu();
            }

            std::ostringstream compile;
            compile << ICON_FA_PLAY;
            compile << " Compilar";

            if(ImGui::BeginMenu(compile.str().c_str()))
            {
                bool compileFile = m_OpenedFiles->GetLastOpenedFile() != nullptr;
                if (compileFile) compileFile &= m_OpenedFiles->GetLastOpenedFile()->GetActive();

                if(ImGui::MenuItem("Compilar Arquivo", "F5", nullptr, compileFile))
                {
                    m_Compiler->GetErrorSystem()->Clear();
                    m_Compiler->CompileFile(m_OpenedFiles->GetLastOpenedFile()->GetPath());
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Limpar Terminal", "F4"))
                {
                    m_Compiler->GetErrorSystem()->Clear();
                }

                ImGui::EndMenu();
            }

            ImGui::Separator();

            std::ostringstream customize;
            customize << ICON_FA_PAINT_BRUSH;
            customize << " Customizar";

            if (ImGui::BeginMenu(customize.str().c_str()))
            {
                ImGui::EndMenu();
            }

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