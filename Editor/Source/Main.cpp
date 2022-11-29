#include "Core/Editor.h"

int main(int argc, char* argv[])
{
    Editor::Editor* app = new Editor::Editor();
    
    app->Init();
    app->Run();
    
    return 0;
}