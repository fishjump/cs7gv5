set_languages("c17", "cxx17")
set_warnings("all", "error")

add_includedirs("resource", "module/figine/include", "/opt/homebrew/Cellar/glm/0.9.9.8/include",
  "/opt/homebrew/Cellar/glfw/3.3.6/include", "/opt/homebrew/Cellar/boost/1.76.0/include",
  "/opt/homebrew/Cellar/assimp/5.2.1/include", "/opt/homebrew/Cellar/freetype/2.11.1/include/freetype2")

add_linkdirs("/opt/homebrew/Cellar/glm/0.9.9.8/lib", "/opt/homebrew/Cellar/glfw/3.3.6/lib",
  "/opt/homebrew/Cellar/boost/1.76.0/lib", "/opt/homebrew/Cellar/assimp/5.2.1/lib",
  "/opt/homebrew/Cellar/freetype/2.11.1/lib")

add_frameworks("OpenGL", "Cocoa")

includes("module/figine")

target("cs7gv5")
  set_kind("binary")
  add_deps("figine")
  add_includedirs("include")
  add_files("src/**.cpp", "resource/**.cpp")
  add_links("figine", "boost_program_options", "boost_filesystem")
