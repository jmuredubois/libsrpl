-- Set package name.
project "libSRPLalign"
	-- if you want a different name for your target than the projects's name.
	outName						= "libSRPLalign"
	uuid						"60286734-4D0C-4035-9DC7-656C28B6E38F"
	-- Set the files to include/exclude.
	files						{ "*.cpp", "*.h" }
	excludes					{ "test*.cpp" }
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
		trigger = "libSRPLalign-static",
		description = "Build libSRPLalign as static lib rather than dll"
	}

	-- -- libSRPLavg dll
	if not _OPTIONS["libSRPLalign-static"] then
		kind 					"SharedLib"
	else
		kind 					"StaticLib"
	end

	-- -- Dynamic Runtime
	if not _OPTIONS["dynamic-runtime"] then
		flags					{ "StaticRuntime" }
	end
	
	includedirs					{string.format('%s',os.getenv("JMU_EIGEN"))}-- package includes for EIGEN
	includedirs				{string.format('%s',os.getenv("JMU_TICPP"))} --TICPP includes
	libdirs					{string.format('%s%s',os.getenv("JMU_TICPP"), "/lib")} -- TICPP libs

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
		links					{ "ticpp" }	-- link TICPP
		flags					{ "OptimizeSpeed" }
	configuration "Debug"
		targetname 				( outName.."d" )
		targetdir				(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin"))
		objdir					(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/obj"))
		implibdir				(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/lib"))
		defines					{ "DEBUG", "_DEBUG" }
		links					{ "ticppd" }	-- link TICPP
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
		
		

