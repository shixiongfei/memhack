-- A solution contains projects, and defines the available configurations
solution ( "MemHack" )
	configurations { "Debug", "Release" }


	project ( "MemHack" )
		kind ( "ConsoleApp" )
		language ( "C" )
		files { "**.h", "**.c" }
		flags { "StaticRuntime" }

		configuration ( "Debug" )
			targetdir ( "bin" )
			objdir ( "obj/debug" )
			defines { "DEBUG", "_DEBUG" }
			flags { "Symbols" }
			targetsuffix ( "d" )

		configuration ( "Release" )
			targetdir ( "bin" )
			objdir ( "obj/release" )
			defines { "NDEBUG", "_NDEBUG" }
			flags { "Optimize" }
		
		configuration ( "vs*" )
			defines { "WIN32", "_WIN32", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
