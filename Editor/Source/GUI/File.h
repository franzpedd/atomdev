#pragma once

#include <Atom.h>

#include "Core/TextEditor.h"

namespace Editor
{
    class File : public Atom::Layer
    {
    public:

        // constructor
        File(const char* path, const char* name);

        // destructor
        virtual ~File();

        // layer loop
        virtual void Update() override;

    public:

        // return file's path
        inline const char* GetPath() { return m_Path; }

        // return file's name
        inline const char* GetName() { return m_Name; }

        // returns if the window is visible or not
        inline bool GetActive() { return m_Show; }

        // sets the window active or inactive
        inline void SetActive(bool value) { m_Show = value; }

    private:

        const char* m_Path;
        const char* m_Name;
        bool m_Show = true;
        TextEditor m_TextEditor;
    };
}