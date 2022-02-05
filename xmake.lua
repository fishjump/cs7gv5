set_languages("c17", "cxx17")
set_warnings("all", "error")

add_includedirs("common/include","common/include/imgui")
add_files("common/src/**.cpp", "common/src/**.c")

if is_plat("macosx") then
  add_includedirs(
    "/opt/homebrew/Cellar/glm/0.9.9.8/include",
    "/opt/homebrew/Cellar/glew/2.2.0_1/include",
    "/opt/homebrew/Cellar/glfw/3.3.6/include",
    "/opt/homebrew/Cellar/boost/1.76.0/include",
    "/opt/homebrew/Cellar/assimp/5.2.0/include",
    "/opt/homebrew/Cellar/freetype/2.11.1/include/freetype2"
  )

  add_linkdirs(
    "/opt/homebrew/Cellar/glm/0.9.9.8/lib",
    "/opt/homebrew/Cellar/glew/2.2.0_1/lib",
    "/opt/homebrew/Cellar/glfw/3.3.6/lib",
    "/opt/homebrew/Cellar/boost/1.76.0/lib",
    "/opt/homebrew/Cellar/assimp/5.2.0/lib",
    "/opt/homebrew/Cellar/freetype/2.11.1/lib"
  )

  add_links()

  add_frameworks("OpenGL", "GLUT", "Cocoa")
end

target("cs7gv5")
  set_kind("binary")
  add_includedirs("main/include")
  add_files("main/src/**.cpp")
  add_links("boost_program_options", "boost_filesystem", "GLEW", "assimp", "glfw", "freetype")