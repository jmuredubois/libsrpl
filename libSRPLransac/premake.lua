project.name = "libSRPLcoordTrf"
project.guid = "5535BA46-82CC-4fff-A166-A83C65FFC8A3"
project.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
project.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")
dopackage("libSRPLcoordTrf")	

