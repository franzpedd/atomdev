project "glad"
    location "glad"
    kind "StaticLib"
    language "C"
    cdialect "C11"

    targetdir(dir)
    objdir(obj)

    files
    {
        "glad/include/glad/gl.h",
        "glad/include/KHR/khrplatform.h",
        "glad/src/gl.c"
    }

    includedirs
    {
        "glad/include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"