package.name = "libSRPLransac"
package.guid = "C8C28BD8-B75B-4151-875F-B049DCB25523"
package.kind = "dll"
package.language = "c++"

package.configs = {"Debug", "Release"}

package.targetprefix = ""

package.config["Debug"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin")
package.config["Release"].bindir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/bin")
package.config["Debug"].objdir = string.format('%s%s%s',os.getenv("JMU_BUILDS"), "/Debug/obj/", package.name)
package.config["Release"].objdir = string.format('%s%s%s',os.getenv("JMU_BUILDS"), "/Release/obj/", package.name)
package.config["Debug"].libdir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Debug/lib")
package.config["Release"].libdir = string.format('%s%s',os.getenv("JMU_BUILDS"), "/Release/lib")

package.files = {
  matchfiles("*.h", "*.cpp")
}

tinsert(package.buildflags, {"no-pch","no-main"})
tinsert(package.defines, { "_SRPL_API_DLL" } )

tinsert(package.config["Release"].buildflags, {"optimize-speed"})

-- package includes for EIGEN
tinsert(package.includepaths,
  {
    string.format('%s',os.getenv("JMU_EIGEN"))
  }
)
-- END OF package includes for EIGEN

if (OS == "windows") then
  tinsert(package.config["Release"].defines, "NDEBUG")
end

if (OS == "macosx") then
  tinsert(package.buildflags, {"dylib"})
  tinsert(package.linkoptions, {"-dynamiclib"})
  if (target =="cb-gcc") then
	tinsert( package.libpaths, 
      {
      	string.format('%s', "/usr/local/lib")
        --string.format('%s%s',os.getenv("WXWIN"), "/build-release")
      }
	)
  end
end


