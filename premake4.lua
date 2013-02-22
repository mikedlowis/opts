-------------------------------------------------------------------------------
-- Option Parsing Library Configuration
-------------------------------------------------------------------------------
solution "OPTS Option Parsing Library"
configurations { "Release" }
targetdir "build"

---------------------------------------------------------------------------------
---- Opts - A simple option parsing library
---------------------------------------------------------------------------------
project "opts"
    kind "SharedLib"
    language "C"
    location "build"
    files { "source/**.*" }

project "opts-tests"
    kind "ConsoleApp"
    language "C++"
    location "build"
    links { "UnitTest++", "opts" }
    includedirs { "source/", "tools/UnitTest++/**" }
    files { "tests/**.c*", "source/**.c*" }
    postbuildcommands { "./opts-tests" }

-------------------------------------------------------------------------------
-- UnitTest++ - A C++ unit testing library
-------------------------------------------------------------------------------
project "UnitTest++"
    kind "SharedLib"
    language "C++"
    location "build"
    files {
        "tools/UnitTest++/src/*.*",
    }
    if os.is "windows" then
        files { "tools/UnitTest++/src/Win32/**.*" }
    else
        files { "tools/UnitTest++/src/Posix/**.*" }
    end

-------------------------------------------------------------------------------
-- Miscellaneous
-------------------------------------------------------------------------------
if _ACTION == "clean" then
    os.rmdir("build")
end

