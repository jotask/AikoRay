
baseName = path.getbasename(os.getcwd());

project (workspaceName)
  	kind "ConsoleApp"
    location "../_build"
    targetdir "../_bin/%{cfg.buildcfg}"
	language "C++"
	cppdialect "C++17"
	
    filter "configurations:Release"
		kind "WindowedApp"
		entrypoint "mainCRTStartup"

	filter "action:vs*"
        debugdir "$(SolutionDir)"
		
	filter {"action:vs*", "configurations:Release"}
			kind "WindowedApp"
			entrypoint "mainCRTStartup"
	filter {}

    files {"**.c", "**.cpp", "**.h", "**.hpp"}

    includedirs {
		"src",
		"%{wks.location}/submodules/spdlog/include",
		"%{wks.location}/submodules/entt/single_include/entt"
	}
	link_raylib();
	
	-- To link to a lib use link_to("LIB_FOLDER_NAME")