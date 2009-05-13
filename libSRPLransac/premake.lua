project.name = "libSRPLransac"
project.guid = "02097215-5D50-473a-8B0A-ABC766960858"
project.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
project.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")
dopackage("libSRPLransac")	

