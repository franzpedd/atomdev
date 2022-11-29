workspace "Everything"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    startproject "Editor"

    dir = "%{wks.location}/_Build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    obj = "%{wks.location}/_Build/temp/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeList = {}
    IncludeList["Atom"] = "%{wks.location}/Atom/Source"
    IncludeList["Editor"] = "%{wks.location}/Editor/Source"
    IncludeList["GLAD"] = "%{wks.location}/Thirdparty/glad/include"
    IncludeList["GLFW"] = "%{wks.location}/Thirdparty/glfw/include"
    IncludeList["ImGui"] = "%{wks.location}/Thirdparty/imgui"
    
group "Thirdparty"
    include "Thirdparty/glad.lua"
    include "Thirdparty/glfw.lua"
    include "Thirdparty/imgui.lua"
group ""

include "Atom"
include "Editor"