project('Wide-Open','cpp')
Common=subproject('Common')
libCommon_dep=Common.get_variable('libCommon_dep')
BlinnPhongWithShadows=subproject('BlinnPhongWithShadows')
libBlinnPhongWithShadows_dep=BlinnPhongWithShadows.get_variable('libBlinnPhongWithShadows_dep')
Cubemap=subproject('Cubemap')
libCubemap_dep=Cubemap.get_variable('libCubemap_dep')
pc=subproject('pc')
libPC_dep=pc.get_variable('libPC_dep')
glfw=dependency('glfw3')
vulkan=dependency('vulkan')
executable('Wide-Open','src/main.cpp',dependencies:[libBlinnPhongWithShadows_dep,libCommon_dep,glfw,vulkan,libCubemap_dep,libPC_dep])