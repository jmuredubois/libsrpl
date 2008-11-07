package.name = "libSRPLcoordTrf"
package.guid = "A203A7A9-3D46-4b46-A30A-A85B03B30DB2"
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

package.buildflags = {"no-pch","no-main"}
--package.defines = {""}
tinsert(package.defines, { "_SRPL_API_DLL" } )

tinsert(package.config["Release"].buildflags, {"optimize-speed"})



if (OS == "windows") then
  tinsert(package.config["Release"].defines, "NDEBUG")
end

if (OS == "macosx") then
  if (target =="cb-gcc") then
	tinsert( package.libpaths, 
      {
      	string.format('%s', "/usr/local/lib")
        --string.format('%s%s',os.getenv("WXWIN"), "/build-release")
      }
	)
  end
end


