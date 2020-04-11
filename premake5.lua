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
IncludeDir["cppunitlite"] = "%{wks.location}/Vendor/cppunitlite"

group "Depenencies"

  include "Vendor/GLAD/premake5_GLAD.lua"
  include "Vendor/imgui/premake5_imgui.lua"
    
  project "DgLib"
    location "Vendor/DgLib"
    kind "StaticLib"
    targetdir (projOutput)
    objdir (projOutputInt)
    systemversion "latest"
    language "C++"
    cppdialect "C++17"
  
    files 
    {
      "Vendor/DgLib/src/**.h",
      "Vendor/DgLib/src/**.cpp",
    }
  
    include "./DgLib_vpaths.lua"
    
    filter "configurations:Debug"
		  runtime "Debug"
		  symbols "on"

	  filter "configurations:Release"
		  runtime "Release"
		  optimize "on"

	  filter "configurations:Dist"
		  runtime "Release"
		  optimize "on"
  
  project "cppunitlite"
    location "Vendor/cppunitlite"
    kind "StaticLib"
    targetdir (projOutput)
    objdir (projOutputInt)
    systemversion "latest"
    include "Vendor/cppunitlite/premake5_cppunitlite.lua"
    
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

project "Tests"
  location "Tests"
  kind "ConsoleApp"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  
  files 
  {
    "Tests/**.h",
    "Tests/**.cpp",
  }

  links
  {
    "cppunitlite",
    "DgLib",
    "Core",
    "Engine",
    "GameCommon",
  }

  includedirs
  {
    "%{IncludeDir.cppunitlite}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.DgLib}",
    "%{wks.location}/Core/src",
    "%{wks.location}/Engine/src",
    "%{wks.location}/GameCommon/src"
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
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.DgLib}"
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
    "Core",
    "Vendor/SDL2-2.0.9/lib/x64/SDL2.lib",
    "Vendor/SDL2-2.0.9/lib/x64/SDL2main.lib"
  }

  includedirs
  {
    "%{wks.location}/Core/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.DgLib}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.SDL2}"
  }
  
  include "./Engine_vpaths.lua"
  
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

project "GameCommon"
  location "GameCommon"
  kind "StaticLib"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  
  files 
  {
    "GameCommon/src/**.h",
    "GameCommon/src/**.cpp",
  }
  
  links
  {
    "DgLib",
    "Core"
  }
  
  includedirs
  {
    "%{wks.location}/Core/src",
		"%{IncludeDir.spdlog}",
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
    "%{wks.location}/Core/src",
    "%{wks.location}/GameCommon/src",
    "%{wks.location}/Engine/src/include",
    "%{IncludeDir.spdlog}",
		"%{IncludeDir.DgLib}",
		"%{IncludeDir.imgui}"
  }
  
  links
  {
    "DgLib",
    "Core",
    "Engine",
    "GameCommon"
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
    
project "Converter"
  location "Converter"
  kind "ConsoleApp"
  targetdir (projOutput)
  objdir (projOutputInt)
  systemversion "latest"
  language "C++"
  cppdialect "C++17"
  
  files 
  {
    "Converter/src/**.h",
    "Converter/src/**.cpp",
  }
  
  includedirs
  {
    "%{wks.location}/GameCommon/src",
    "%{wks.location}/Core/src",
		"%{IncludeDir.DgLib}"
  }
  
  links
  {
    "DgLib",
    "GameCommon",
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
      "%{wks.location}/Core/src",
      "%{wks.location}/GameCommon/src",
      "%{wks.location}/Engine/src",
      "%{wks.location}/Engine/src/Engine",
      "%{IncludeDir.spdlog}",
      "%{IncludeDir.DgLib}"
    }
    
    links
    {
      "Core",
      "Engine",
      "DgLib",
      "GameCommon"
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
  
 