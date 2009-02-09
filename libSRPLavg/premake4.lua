--*	NOTES:
--*		- use the '/' slash for all paths.
--*****************************************************************************
solution "libSRPLavg4"
	configurations { "Debug", "Release" }

-- This is for including other Premake scripts.
dofile( "libSRPLavg_prem4.lua" )

-- Add options here.
newoption
{
	trigger = "dynamic-runtime",
	description = "Use the dynamicly loadable version of the runtime."
}

newoption
{
	trigger = "unicode",
	description = "Use the Unicode character set"
}


--project.name = "libSRPLavg"
--project.guid = "F18FE6C0-5B86-49b1-9F6D-EB2B99C61DFD"
--project.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
--project.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")
--dopackage("libSRPLavg")	

