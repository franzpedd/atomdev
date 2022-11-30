#include "Sidemenu.h"

#include <exception>
#include <iostream>

namespace Editor
{
    Sidemenu::Sidemenu(OpenedFile* openedfile)
        : Atom::Layer("Sidemenu"), m_OpenedFile(openedfile)
    {
        LOGGER_TRACE("Creating Sidemenu");

        std::filesystem::path workSpace = std::filesystem::current_path() / "Workspace";
        
        if(!std::filesystem::exists(workSpace))
        {
            std::filesystem::create_directory(workSpace);
        }

        m_Root = CreateDirs(workSpace);
    }

    Sidemenu::~Sidemenu()
    {
        
    }

    void Sidemenu::Update()
    {
        ImGui::Begin("Explorer");
        ImGui::BeginChild("##Explorer");

        DisplayDir(m_Root);

        ImGui::EndChild();
        ImGui::End();
    }

    void Sidemenu::AddDirs(Dir& parent, std::filesystem::directory_iterator it)
    {
        for(const auto& entry : it)
        {
            Dir& child = parent.Subdirs.emplace_back();
            child.Fullpath = entry.path().u8string();
            child.Filename = entry.path().filename().u8string();

            if(child.isDir = entry.is_directory(); child.isDir)
            {
                AddDirs(child, std::filesystem::directory_iterator(entry));
            }

            auto moveToFront = [](const Dir& a, const Dir& b)
            {
                return (a.isDir > b.isDir);
            };

            std::sort(parent.Subdirs.begin(), parent.Subdirs.end(), moveToFront);
        }
    }

    Dir Sidemenu::CreateDirs(const std::filesystem::path& root)
    {
        Dir d;
        d.Fullpath = root.u8string();
        d.Filename = root.filename().u8string();

        if(d.isDir == std::filesystem::is_directory(root); d.isDir)
        {
            AddDirs(d, std::filesystem::directory_iterator(root));
        }

        return d;
    }

    void Sidemenu::DisplayDir(const Dir& parent)
    {
        ImGui::PushID(&parent);

        if(parent.isDir)
        {
            ImGui::Text(ICON_FA_FOLDER);
            ImGui::SameLine();

            if(ImGui::TreeNodeEx(parent.Filename.c_str()))
            {
                for(const Dir& child : parent.Subdirs)
                {
                    DisplayDir(child);
                }

                ImGui::TreePop();
            }
        }

        else
        {
            std::filesystem::path f = parent.Filename;

            bool display;
            display |= f.extension() == ".atom";

            if(!display)
            {
                ImGui::PopID();
                return;
            }

            if(f.extension() == ".atom")
            {
                ImGui::Text(ICON_FA_FILE);
            }

            ImGui::SameLine();

            if(ImGui::TreeNodeEx(parent.Filename.c_str(), ImGuiTreeNodeFlags_Leaf))
            {
                // open new file or opened file
                if(ImGui::IsItemClicked() && !std::filesystem::is_directory(parent.Fullpath))
                {
                    m_OpenedFile->OpenFile(parent.Fullpath.c_str());
                }

                ImGui::TreePop();
            }
        }

        ImGui::PopID();
    }
}