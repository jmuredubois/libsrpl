--*	NOTES:
--*		- use the '/' slash for all paths.
--*****************************************************************************
solution "libSRPLsegm"
	configurations { "Debug", "Release" }

-- This is for including other Premake scripts.
dofile( "libSRPLsegm_prem4.lua" )

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

--project.name = "libSRPLsegm"
--project.guid = "089F867E-40B6-447d-9399-AF8096D2869A"
--project.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
--project.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")
--dopackage("libSRPLsegm")	

