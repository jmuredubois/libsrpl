--*	NOTES:
--*		- use the '/' slash for all paths.
--*****************************************************************************
solution "libSRPLcoordTrf"
	configurations { "Debug", "Release" }

-- This is for including other Premake scripts.
dofile( "libSRPLcoordTrf_prem4.lua" )

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

	
--project.name = "libSRPLcoordTrf"
--project.guid = "5535BA46-82CC-4fff-A166-A83C65FFC8A3"
--project.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
--project.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")
--dopackage("libSRPLcoordTrf")	

