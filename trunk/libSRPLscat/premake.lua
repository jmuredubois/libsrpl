project.name = "libSRPLscat"
project.guid = "4ABEECF0-7031-4790-8A05-C7DFE64BD37E"

--addoption("use-vtk", "Enable building against VTK(tm) for rendering")

--[[if (options["use-vtk"]) then
	project.configs = {"DebugVTK", "ReleaseVTK"}
		--tinsert(project.config,   {"DebugVTK", "ReleaseVTK"})
	project.config["DebugVTK"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
	project.config["ReleaseVTK"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")

	dopackage("wxSRparlab/wxSRparlabVTK")
else--]]	
	project.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
	project.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")

	dopackage("libSRPLscat")	
--end
