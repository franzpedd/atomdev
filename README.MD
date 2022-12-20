## Atom programing language and IDE
This is a project was developed at college's "Compiler" subject. It makes lexical/syntatic/semantic/ analyses on a hypoteticall programing language invented by the students and this particular one borrowed a lot from C. 

This is far from a real programing language but it may be usefull for study pourpuses.

## Requirements and Dependencies

Must be installed on machine:
* A C++ Compiler
* Python with [tqdm](https://github.com/tqdm/tqdm) and [requests](https://pypi.org/project/requests/)

Downloaded at runtime:

* [Premake5](https://github.com/premake/premake-core)
* [glad](https://github.com/Dav1dde/glad)
* [GLFW](https://github.com/glfw/glfw)
* [ImGui](https://github.com/ocornut/imgui)

Already on project:

* [ImGuiColorTextEdit](https://github.com/BalazsJako/ImGuiColorTextEdit)
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
* [IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders)

## Usage
Once with a C++ compiler and python with tqdm and requests is present on the system,  exec as follow: 
 ```bash
py Setup.py ARG
```

where ARG may be one of: ```vs2017``` ```vs2019``` ```vs2022``` ```gmake2``` ```xcode4``` where vs20xx is solution for Visual Studio, gmake2 for a MakeFile and xcode4 for XCode.

A solution will be generated and may be compiled under the choosen way.

This was tested with Visual Studio 2022 and msys2/MinGW.

##  Image
![IDE](https://github.com/franzpedd/atomdev/blob/main/Atom/Assets/Images/ide.jpg?raw=true)
