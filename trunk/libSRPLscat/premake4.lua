--*	NOTES:
--*		- use the '/' slash for all paths.
--*****************************************************************************
solution "libSRPLscat"
	configurations { "Debug", "Release" }

-- This is for including other Premake scripts.
dofile( "libSRPLscat_prem4.lua" )

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

--project.name = "libSRPLscat"
--project.guid = "4ABEECF0-7031-4790-8A05-C7DFE64BD37E"
--	project.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
--	project.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")
--	dopackage("libSRPLscat")	
