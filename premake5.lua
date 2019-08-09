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
IncludeDir["Glad"] = "%{wks.location}/Vendor/GLAD/include"
IncludeDir["SDL2"] = "%{wks.location}/Vendor/SDL2-2.0.9/include"
IncludeDir["imgui"] = "%{wks.location}/Vendor/imgui"
IncludeDir["spdlog"] = "%{wks.location}/Vendor/spdlog/include"
IncludeDir["DgLib"] = "%{wks.location}/Vendor/DgLib/src"

group "Depenencies"

  include "Vendor/GLAD/premake5_GLAD.lua"
  include "Vendor/imgui/premake5_imgui.lua"
    
  project "DgLib"
    location "Vendor/DgLib"
    kind "StaticLib"
    targetdir (projOutput)
    objdir (projOutputInt)
    systemversion "latest"
    include "Vendor/DgLib/premake5_DgLib.lua"
    
    filter "configurations:Debug"
		  runtime "Debug"
		  symbols "on"

	  filter "configurations:Release"
		  runtime "Release"
		  optimize "on"

	  filter "configurations:Dist"
		  runtime "Release"
		  optimize "on"
  
group ""

project "Engine"
  location "Engine"
  kind "StaticLib"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  
  files 
  {
    "Engine/src/**.h",
    "Engine/src/**.cpp",
  }

  links
  {
    "DgLib",
    "Glad",
    "imgui",
    "Vendor/SDL2-2.0.9/lib/x64/SDL2.lib",
    "Vendor/SDL2-2.0.9/lib/x64/SDL2main.lib"
  }

  includedirs
  {
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.DgLib}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.imgui}"
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

project "Common"
  location "Common"
  kind "StaticLib"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  
  files 
  {
    "Common/src/**.h",
    "Common/src/**.cpp",
  }
  
  links
  {
    "DgLib"
  }
  
  includedirs
  {
		"%{IncludeDir.DgLib}"
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
    
project "Editor"
  location "Editor"
  kind "ConsoleApp"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  
  files 
  {
    "Editor/src/**.h",
    "Editor/src/**.cpp",
  }
  
  includedirs
  {
    "%{wks.location}/Common/src",
    "%{IncludeDir.spdlog}",
		"%{IncludeDir.DgLib}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.imgui}"
  }
  
  links
  {
    "DgLib",
    "Glad",
    "Common"
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
  
 