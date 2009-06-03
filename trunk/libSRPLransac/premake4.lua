--*	NOTES:
--*		- use the '/' slash for all paths.
--*****************************************************************************
solution "libSRPLransac"
	configurations { "Debug", "Release" }

-- This is for including other Premake scripts.
dofile( "libSRPLransac_prem4.lua" )

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

--project.name = "libSRPLransac"
--project.guid = "02097215-5D50-473a-8B0A-ABC766960858"
--project.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
--project.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")
--dopackage("libSRPLransac")	

