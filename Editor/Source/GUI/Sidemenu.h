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
        bool isDir = false;
    };

    class Sidemenu : public Atom::Layer
    {
    public:

        // constructor
        Sidemenu(Atom::SharedRef<Atom::Compiler>& compiler, OpenedFiles* openedfiles);

        // destructor
        ~Sidemenu();

        // layer loop
        virtual void Update() override;

    public:

        // returns the list of project files inside the working directory
        std::vector<const char*> GetAllProjectFiles();

    private:

        // adds files to the list of project files
        void PopulateProjectFiles(const Dir& parent);

        // adds all the directories given a parent folder
        void AddDirs(Dir& parent, std::filesystem::directory_iterator it);

        // creates directories given a root
        Dir CreateDirs(const std::filesystem::path& root);

        // draws a directory
        void DisplayDir(const Dir& parent);

    private:

        Atom::SharedRef<Atom::Compiler>& m_Compiler;
        OpenedFiles* m_OpenedFiles;
        Dir m_Root;
        std::vector<const char*> m_AllProjectFiles;
    };
}