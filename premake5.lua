workspace "BSR"
  location ""
  architecture "x64"
  
  startproject "Game"
  
  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }
  
projOutput = "%{wks.location}/build/%{prj.name}-%{cfg.buildcfg}"
projOutputInt = "%{wks.location}/build/intermediate/%{prj.name}-%{cfg.buildcfg}"

project "Game"
  location "Game"
  kind "ConsoleApp"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  
  files 
  {
    "Game/src/**.h",
    "Game/src/**.cpp",
  }

  includedirs
  {
    "%{wks.location}/Core/src"
  }
  
  links
  {
    "DgLib",
    "Core"
  }

  includedirs
  {
    "%{wks.location}/Vendor/DgLib/src",
    "%{wks.location}/Core/src",
    "%{wks.location}/Vendor/spdlog/include"
  }

project "Core"
  location "Core"
  kind "StaticLib"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  
  files 
  {
    "Core/src/**.h",
    "Core/src/**.cpp",
  }
  
  links
  {
    "DgLib"
  }
  
  includedirs
  {
    "%{wks.location}/Vendor/DgLib/src",
    "%{wks.location}/Vendor/spdlog/include"
  }
  
project "Tools"
  location "Tools"
  kind "ConsoleApp"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  
  files 
  {
    "Tools/src/**.h",
    "Tools/src/**.cpp",
  }
  
  includedirs
  {
    "%{wks.location}/Vendor/DgLib/src",
    "%{wks.location}/Core/src"
  }
  
  links
  {
    "DgLib",
    "Core"
  }
  
 project "DgLib"
  location "Vendor/DgLib"
  kind "StaticLib"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  
  include "Vendor/DgLib/premake5_DgLib.lua"