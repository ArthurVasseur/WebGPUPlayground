add_rules("plugin.vsxmake.autoupdate")
add_rules("mode.debug", "mode.release")

add_repositories('Concerto-xrepo https://github.com/ConcertoEngine/xmake-repo.git main')

add_requires("wgpu-native", "concertocore", "glfw", "glfw3webgpu", {configs = {debug = is_mode("debug"), with_symbols = true}})

if is_plat("windows") then
    set_runtimes(is_mode("debug") and "MDd" or "MD")
end

target("WebGpuTest")
    set_kind("binary")
    add_files("src/*.cpp")
    set_warnings("allextra")
    add_includedirs("Include")
    add_headerfiles("Include/*.hpp")
    set_languages("cxx20")
    add_packages("wgpu-native", "concertocore", "glfw", "glfw3webgpu")
    add_cxxflags("cl::/Zc:preprocessor", { public = true })