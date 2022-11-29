project "Atom"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"

    targetdir(dir)
    objdir(obj)

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

    filter "configurations:Debug"
        defines { "ATOM_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "ATOM_RELEASE" }
        runtime "Release"
        optimize "Full"

    filter "toolset:msc"
        prebuildcommands
        {
            "mkdir " .. dir .. "/Assets",
            "copy /y Assets/Fonts" .. " " .. dir .. "/Assets"
        }
    
    filter "toolset:gcc"
        prebuildcommands
        {
            "mkdir -p " .. dir .. "/Assets",
            "cp -f -r Assets/Fonts" .. " " .. dir .. "/Assets"
        }