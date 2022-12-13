project "Editor"
    kind "ConsoleApp"
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
        "%{IncludeList.Editor}",
        "%{IncludeList.ImGui}"
    }

    links
    {
        "Atom",
        "ImGui"
    }

    defines
    {
        "_IMPORT_SHARED_LIB"
    }

    filter "configurations:Debug"
        defines { "EDITOR_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "EDITOR_RELEASE" }
        runtime "Release"
        optimize "Full"

    filter "system:windows"
        defines { "_CRT_SECURE_NO_WARNINGS" }
        disablewarnings { "4251", "26451", "6031", "33010", "6011", "6387", "26819", "2662" }