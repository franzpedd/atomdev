#pragma once

#include <Atom.h>

#include "File.h"

#define MAX_OPENED_FILES 20

namespace Editor
{
    class OpenedFiles : public Atom::Layer
    {
    public:

        // constructor
        OpenedFiles(Atom::SharedRef<Atom::GUI>& gui);

        // destructor
        virtual ~OpenedFiles();

        // layer loop
        virtual void Update() override;

    public:

        // returns the list of opened files
        inline std::vector<File*>& GetFiles() { return m_Files; }

        // returns the last oepened file
        inline File* GetLastOpenedFile() { return m_LastOpenedFile; }

    public:

        // opens a new file
        void Open(const char* path, const char* filename);

        // closes a file from the list of opened files
        void Close(const char* path, const char* filename);

        // changes the pallete for all files
        void ChangePallete(int id);

    private:

        // shows a default message if theres no available file
        void ShowDefaultMessage();

    private:

        Atom::SharedRef<Atom::GUI>& m_GUI;
        std::vector<File*> m_Files;
        File* m_LastOpenedFile = nullptr;
    };
}