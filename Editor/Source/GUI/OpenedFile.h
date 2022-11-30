#pragma once

#include <Atom.h>

namespace Editor
{
    class OpenedFile : public Atom::Layer
    {
    public:

        // constructor
        OpenedFile();

        // destructor
        virtual ~OpenedFile();

        // layer loop
        virtual void Update() override;

    public:

        // opens a new file
        void OpenFile(const char* path);

    private:

        // shows a default message if theres no available file
        void ShowDefaultMessage();

    private:

        const char* m_Path;
        bool m_ShowDefaultWindow = true;
    };
}