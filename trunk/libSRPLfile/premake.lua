project.name = "libSRPLfile"
project.guid = "BD6ED18B-4357-4ABE-B885-5531AB137619"
project.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
project.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")
dopackage("libSRPLfile")	
