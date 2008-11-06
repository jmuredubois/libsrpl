project.name = "libSRPLavg"
project.guid = "F18FE6C0-5B86-49b1-9F6D-EB2B99C61DFD"
project.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
project.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")
dopackage("libSRPLavg")	

