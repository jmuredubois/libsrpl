-- premake4 default script file to automate building of libSRPLavg
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
--*	NOTES:
--*		- use the '/' slash for all paths.
--*****************************************************************************
solution "libSRPLavg"
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

