-- premake4 script file to automate building of testLibSRPLalign
--
--    Copyright 2009 James Mure-Dubois, Heinz H�gli and Institute of Microtechnology of EPFL.
--      http://parlab.epfl.ch/
--
--    This program is free software: you can redistribute it and/or modify
--    it under the terms of the GNU General Public License as published by
--    the Free Software Foundation, either version 3 of the License, or
--    (at your option) any later version.
--
--    This program is distributed in the hope that it will be useful,
--    but WITHOUT ANY WARRANTY; without even the implied warranty of
--    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--    GNU General Public License for more details.
--
--    You should have received a copy of the GNU General Public License
--    along with this program.  If not, see <http://www.gnu.org/licenses/>.
--
-- $Rev$:
-- $Author$:
-- $Date$:
-- Set package name.
project "testLibSRPLalign"
	-- if you want a different name for your target than the projects's name.
	outName						= "testLibSRPLalign"
	uuid						"CA045B0C-7970-4c44-B5BF-4509370BDC19"
	-- Set the files to include/exclude.
	files						{ "test*.cpp", "*libSR*.h" }
	--excludes					{ "test*.cpp" }
	-- Setup the output directory options.
	--		Note: Use 'libdir' for "lib" kind only.
	--bindir					"../lib"
	--libdir					"../lib"
	-- Set the defines.
	--defines						{ "_SRPL_API_DLL" }
	--implibprefix 				""
	targetprefix 				""
	-- Common setup
	language					"C++"
	flags						{ "NoPCH" }
	--******* GENAERAL SETUP **********
	--*	Settings that are not dependant
	--*	on the operating system.
	--*********************************
    kind						"ConsoleApp"

-- OPTIONS ---------------------------------------------------------------------
	-- -- Package options

	-- -- Dynamic Runtime
	if _OPTIONS["dynamic-runtime"] then
		flags					{ "StaticRuntime" }
	end
	
	--includedirs					{string.format('%s',os.getenv("JMU_EIGEN"))}-- package includes for EIGEN

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
        if configuration "windows" then
            links					{"libSRPLalign"}
        else
            links					{"SRPLalign"}
        end
		--libdirs					{(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin"))}
	configuration "Debug"
		targetname 				( outName.."d" )
		targetdir				(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin"))
		objdir					(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/obj"))
		implibdir				(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/lib"))
		defines					{ "DEBUG", "_DEBUG" }
		flags					{ "Symbols" }
        if configuration "windows" then
            links					{"libSRPLalignd"}
        else
            links					{"SRPLalignd"}
        end
		--libdirs					{(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin"))}

	-- -- Operating Systems specific
	if configuration "windows" then
		defines					{ "WIN32", "_WINDOWS" }
	else
		excludes				{ "**.rc" }		-- Ignore resource files in Linux.
		buildoptions			{ "-fPIC" }
	end

	configuration{"macosx", "codeblocks"}
		libdirs					(string.format('%s', "/usr/local/lib"))
		


