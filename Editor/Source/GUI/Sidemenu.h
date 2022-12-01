#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>

#include <Atom.h>

#include "OpenedFiles.h"

namespace Editor
{
    struct Dir
    {
        std::string Fullpath;
        std::string Filename;
        std::vector<Dir> Subdirs;
        bool isDir;
    };

    class Sidemenu : public Atom::Layer
    {
    public:

        // constructor
        Sidemenu(OpenedFiles* openedfiles);

        // destructor
        ~Sidemenu();

        // layer loop
        virtual void Update() override;

    private:

        // adds all the directories given a parent folder
        void AddDirs(Dir& parent, std::filesystem::directory_iterator it);

        // creates directories given a root
        Dir CreateDirs(const std::filesystem::path& root);

        // draws a directory
        void DisplayDir(const Dir& parent);

    private:

        Dir m_Root;
        OpenedFiles* m_OpenedFiles;
    };
}