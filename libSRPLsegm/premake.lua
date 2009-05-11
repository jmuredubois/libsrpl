project.name = "libSRPLsegm"
project.guid = "089F867E-40B6-447d-9399-AF8096D2869A"
project.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
project.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")
dopackage("libSRPLsegm")	

