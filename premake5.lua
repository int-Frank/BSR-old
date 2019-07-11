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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Glad"] = "Vendor/GLAD/include"
IncludeDir["SDL2"] = "Vendor/SDL2-2.0.9/include"

group "Depenencies"
  include "Vendor/GLAD/premake5_GLAD.lua"
  
group ""

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

  links
  {
    "DgLib",
    "Glad",
    "Core",
    "Vendor/SDL2-2.0.9/lib/x64/SDL2.lib",
    "Vendor/SDL2-2.0.9/lib/x64/SDL2main.lib"
  }

  includedirs
  {
    "%{wks.location}/Vendor/DgLib/src",
    "%{wks.location}/Core/src",
    "%{wks.location}/Vendor/spdlog/include",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.SDL2}"
  }
  
  filter "configurations:Debug"
		defines "BSR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BSR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BSR_DIST"
		runtime "Release"
		optimize "on"

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
  
  filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
    
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
    "%{wks.location}/Core/src",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.SDL2}"
  }
  
  links
  {
    "DgLib",
    "Glad",
    "Core"
  }
  
  filter "configurations:Debug"
		defines "BSR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BSR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BSR_DIST"
		runtime "Release"
		optimize "on"
  
 project "DgLib"
  location "Vendor/DgLib"
  kind "StaticLib"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  
  include "Vendor/DgLib/premake5_DgLib.lua"