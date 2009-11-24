-- premake4 script file to automate building of libSRPLscat
--
--    Copyright 2009 James Mure-Dubois, Heinz Hügli and Institute of Microtechnology of EPFL.
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
project "libSRPLscat"
	-- if you want a different name for your target than the projects's name.
	outName						= "libSRPLscat"
	uuid						"E023DAB0-E153-4ba8-95FA-C6C9F14EE27F"
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
		trigger = "libSRPLscat-static",
		description = "Build libSRPLsegm as static lib rather than dll"
	}

	-- -- libSRPLavg dll
	if not _OPTIONS["libSRPLscat-static"] then
		kind 					"SharedLib"
	else
		kind 					"StaticLib"
	end

	-- -- Dynamic Runtime
	if _OPTIONS["dynamic-runtime"] then
		flags					{ "StaticRuntime" }
	end
	
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
		flags					{ "OptimizeSpeed" }
		links					{ "ticpp" }	-- link TICPP
	configuration "Debug"
		targetname 				( outName.."d" )
		targetdir				(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin"))
		objdir					(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/obj"))
		implibdir				(string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/lib"))
		defines					{ "DEBUG", "_DEBUG" }
		flags					{ "Symbols" }
		links					{ "ticppd" }	-- link TICPP

	-- -- Operating Systems specific
	if configuration "windows" then
		defines					{ "WIN32", "_WINDOWS" }
		includedirs				{string.format('%s',os.getenv("JMU_FFTW3"))} --FFTW3 includes
		libdirs				{string.format('%s',os.getenv("JMU_FFTW3"))} --FFTW3 binaries
		links					{ "libfftw3-3" } -- FFTW3 lib name
	else
		excludes				{ "**.rc" }		-- Ignore resource files in Linux.
		buildoptions			{ "-fPIC" }
	end

	configuration{"macosx", "codeblocks"}
		libdirs					(string.format('%s', "/usr/local/lib"))
		
	configuration "macosx"
		targetextension		".dylib"
		linkoptions			"-dynamiclib"
		includedirs				{string.format('%s%s',os.getenv("JMU_FFTW3"), "/include")} --FFTW3 includes
		libdirs				{string.format('%s%s',os.getenv("JMU_FFTW3"), "/lib")} --FFTW3 binaries
		links					{ "fftw3" } -- FFTW3 lib name
		

