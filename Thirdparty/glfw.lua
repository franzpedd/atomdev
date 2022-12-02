project "GLFW"
    location "glfw"
    kind "SharedLib"
    language "C"
    cdialect "C11"

    targetdir(dir)
    objdir(obj)

    files
    {
        "glfw/include/**.h",
        "glfw/src/glfw_config.h",
        "glfw/src/context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/monitor.c",
        "glfw/src/vulkan.c",
        "glfw/src/window.c",
        "glfw/src/egl_context.c",
        "glfw/src/osmesa_context.c"
    }

    includedirs
    {
        "glfw/include"
    }

    defines
    {
        "_GLFW_BUILD_DLL"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    filter "system:windows"
        files
        {
            "glfw/src/win32_init.c",
            "glfw/src/win32_joystick.c",
            "glfw/src/win32_monitor.c",
            "glfw/src/win32_thread.c",
            "glfw/src/win32_time.c",
            "glfw/src/win32_window.c",
            "glfw/src/wgl_context.c"
        }

        defines
        {
            "_GLFW_WIN32"
        }

        disablewarnings
        {
            "4996"
        }

        links
        {
            "gdi32",
            "opengl32"
        }
    
    filter "system:linux"
        files
        {
            "glfw/src/x11_init.c",
            "glfw/src/linux_joystick.h",
            "glfw/src/x11_monitor.c",
            "glfw/src/posix_thread.h",
            "glfw/src/posix_time.c",
            "glfw/src/x11_window.c",
            "glfw/src/glx_context.c"
        }

        defines
        {
            "_GLFW_X11"
        }

    filter "system:macosx"
        files
        {
            "glfw/src/cocoa_init.m",
            "glfw/src/cocoa_joystick.h",
            "glfw/src/cocoa_monitor.m",
            "glfw/src/posix_thread.h",
            "glfw/src/cocoa_time.h",
            "glfw/src/cocoa_window.m",
            "glfw/src/nsgl_context.m"
        }

        defines
        {
            "_GLFW_COCOA"
        }