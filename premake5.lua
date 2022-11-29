workspace "Everything"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    startproject "Editor"

    dir = "%{wks.location}/_Build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    obj = "%{wks.location}/_Build/temp/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeList = {}
    IncludeList["Atom"] = "%{wks.location}/Atom/Source"
    IncludeList["Editor"] = "%{wks.location}/Editor/Source"

include "Atom"
include "Editor"