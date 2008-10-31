package.name = "libSRPLscat"
package.guid = "E023DAB0-E153-4ba8-95FA-C6C9F14EE27F"
package.kind = "dll"
package.language = "c++"

package.configs = {"Debug", "Release"}

package.targetprefix = ""
--package.config["DebugVTK"].target = string.format('/%s%s%s',os.getenv("JMU_BUILDS"), "/Debug/bin",package.name)
--package.config["ReleaseVTK"].target = string.format('%s%s%s',os.getenv("JMU_BUILDS"), "/Release/bin",package.name)

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
  ----tinsert(package.defines, { "WIN32", "_WINDOWS", "WINVER=0x400", "_MT", "wxUSE_GUI=1" })
  --tinsert(package.links,   { "comctl32", "rpcrt4", "winmm", "advapi32", "wsock32"})

  --tinsert(package.config["Debug"].defines , "_DEBUG")
  tinsert(package.config["Release"].defines, "NDEBUG")
end

---END OF package includes for wxWidgets

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

--package includes for TICPP
tinsert(package.config["Debug"].links,   { "ticppd"})
tinsert(package.config["Release"].links,   { "ticpp"})
tinsert(package.includepaths,
  {
    string.format('%s',os.getenv("JMU_TICPP"))
  }
)
tinsert( package.libpaths, 
  {
    string.format('%s%s',os.getenv("JMU_TICPP"), "/lib")
  }
)
--END OF package includes for TICPP

--package includes for FFTW
if (OS == "windows") then
  tinsert(package.config["Debug"].links,   { "libfftw3-3"})
  tinsert(package.config["Release"].links,   { "libfftw3-3"})
  tinsert(package.includepaths,
  {
    string.format('%s',os.getenv("JMU_FFTW3"))
  }
  )
  tinsert( package.libpaths, 
  {
    string.format('%s',os.getenv("JMU_FFTW3"))
  }
  )
end
if (OS == "macosx") then
  tinsert(package.config["Debug"].links,   { "libfftw3"})
  tinsert(package.config["Release"].links,   { "libfftw3"})
  tinsert(package.includepaths,
  {
    string.format('%s%s',os.getenv("JMU_FFTW3"), "/include")
  }
  )
  tinsert( package.libpaths, 
  {
    string.format('%s%s',os.getenv("JMU_FFTW3"), "/lib")
  }
  )
end
--END OF package includes for FFTW

