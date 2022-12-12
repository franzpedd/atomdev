project "Atom"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"

    targetdir(dir)
    objdir(obj)
    debugdir(dir);
    
    files
    {
        "Source/**.h",
        "Source/**.cpp"
    }

    includedirs
    {
        "%{IncludeList.Atom}",
        "%{IncludeList.GLAD}",
        "%{IncludeList.GLFW}",
        "%{IncludeList.ImGui}"
    }

    links
    {
        "GLAD",
        "GLFW",
        "ImGui"
    }

    defines
    {
        "_EXPORT_SHARED_LIB"
    }

    filter "configurations:Debug"
        defines { "ATOM_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "ATOM_RELEASE" }
        runtime "Release"
        optimize "Full"

    filter "action:vs*"
        prebuildcommands
        {
            "mkdir " .. dir .. "\\Assets\\Fonts",
            "Xcopy /y Assets\\Fonts" .. " " .. dir .. "\\Assets\\Fonts",
            "mkdir " .. dir .. "\\Workspace",
            "Xcopy /y Workspace" .. " " .. dir .. "\\Workspace"
        }
    
    filter { "toolset:gcc" }
        prebuildcommands
        {
            "mkdir -p " .. dir .. "/Assets",
            "cp -f -r Assets/Fonts" .. " " .. dir .. "/Assets",
            "mkdir -p " .. dir .. "/Workspace",
            "cp -f -r Workspace" .. " " .. dir
        }

    filter { "toolset:clang" }
        prebuildcommands
        {
            "mkdir -p " .. dir .. "/Assets",
            "cp -f -r Assets/Fonts" .. " " .. dir .. "/Assets",
            "mkdir -p " .. dir .. "/Workspace",
            "cp -f -r Workspace" .. " " .. dir
        }

    filter "system:windows"
        defines { "_CRT_SECURE_NO_WARNINGS" }
        disablewarnings { "4251", "26451", "6031", "33010", "6011", "6387" }