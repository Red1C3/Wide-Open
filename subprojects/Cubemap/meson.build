project('cubemap','cpp')
inc=include_directories('include')
srcAssembler=run_command('srcAssembler.sh')
sources=srcAssembler.stdout().strip().split('\n')
libCommon=subproject('Common')
libCommon_dep=libCommon.get_variable('libCommon_dep')
vulkan = dependency('vulkan')
glfw=dependency('glfw3')
assimp=dependency('assimp')
lodepng=meson.get_compiler('cpp').find_library('lodepng',dirs:join_paths(meson.source_root(),'subprojects/Cubemap'))#use your own path
libCubemap=library('cubemap',sources,include_directories:[inc],dependencies:[libCommon_dep,assimp,glfw,vulkan,lodepng],soversion:0)
libCubemap_dep=declare_dependency(include_directories:inc,link_with:libCubemap)