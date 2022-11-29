project "Atom"
    kind "StaticLib"
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
        "%{IncludeList.Atom}"
    }

    filter "configurations:Debug"
        defines { "ATOM_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "ATOM_RELEASE" }
        runtime "Release"
        optimize "Full"