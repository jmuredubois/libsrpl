-- Set package name.
project "libSRPLcoordTrf"
	-- if you want a different name for your target than the projects's name.
	outName						= "libSRPLcoordTrf"
	uuid						"A203A7A9-3D46-4b46-A30A-A85B03B30DB2"
	-- Set the files to include/exclude.
	files						{ "*.cpp", "*.h" }
	--excludes					{ "" }
	-- Setup the output directory options.
	--		Note: Use 'libdir' for "lib" kind only.
	--bindir					"../lib"
	--libdir					"../lib"
	-- Set the defines.
	defines						{ "_SRPL_API_DLL" }
	implibprefix 				""
	targetprefix 				""
	-- Common setup
	language					"C++"
	flags						{ "NoPCH" }
	--******* GENAERAL SETUP **********
	--*	Settings that are not dependant
	--*	on the operating system.
	--*********************************

-- OPTIONS ---------------------------------------------------------------------
	-- -- Package options
	newoption
	{
		trigger = "libSRPLcoordTrf-static",
		description = "Build libSRPLcoordTrf as static lib rather than dll"
	}

	-- -- libSRPLavg dll
	if not _OPTIONS["libSRPLcoordTrf-static"] then
		kind 					"SharedLib"
	else
		kind 					"StaticLib"
	end

	-- -- Dynamic Runtime
	if not _OPTIONS["dynamic-runtime"] then
		flags					{ "StaticRuntime" }
	end

-- CONFIGURATIONS -------------------------------------------------------------
--
	-- -- Unicode
	configuration "unicode"
		flags					{ "Unicode" }
		defines					{ "UNICODE", "_UNICODE" }

	-- -- GCC compilers
	configuration { "code*", "gmake" }
		objdir					".obj"
		buildoptions			{ "-O0" }

	-- -- Visual Studio
	configuration "vs*"
		-- Windows and Visual C++ 2005/2008
		defines					{ "_CRT_SECURE_NO_DEPRECATE" }

	-- Release/Debug
	-- Set the default targetName if none is specified.
	if #outName <= 0 then outName = project().name end
	print( outName )
	print( project().name )
	-- Set the targets.
	configuration "Release"
		targetname				( outName )
		targetdir				(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin"))
		objdir					(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/obj"))
		implibdir				(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/lib")) 
		defines					{ "NDEBUG" }
		flags					{ "OptimizeSpeed" }
	configuration "Debug"
		targetname 				( outName.."d" )
		targetdir				(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin"))
		objdir					(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/obj"))
		implibdir				(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/lib"))
		defines					{ "DEBUG", "_DEBUG" }
		flags					{ "Symbols" }

	-- -- Operating Systems specific
	if configuration "windows" then
		defines					{ "WIN32", "_WINDOWS" }
	else
		excludes				{ "**.rc" }		-- Ignore resource files in Linux.
		buildoptions			{ "-fPIC" }
	end

	configuration{"macosx", "codeblocks"}
		libdirs					(string.format('%s', "/usr/local/lib"))
		
	configuration "macosx"
		targetextension		".dylib"
		linkoptions			"-dynamiclib"



