# Wide-Open
This project is meant for educational purpose, it contains basic renderers that use common technics using Vulkan API,still, the project is not optimized and is just for showing how to do some stuff in Vulkan API.
## Blinn-Phong
This subproject implements the Blinn-Phong lighting system into Vulkan API,it simply uses a single color attachment (or image if you don't know what an attachment is), and a depth attachment for depth testing,the shaders are simple and straightforward, the shaders recieve vertices position and normal data, and outputs a single color per fragment, no textures support at all,and there is only a single light and a single mesh (which I think it'll be easy to expand by a few changes).
## Common
This subproject contains common objects and functions to be shared among other subprojects.
## Roadmap
New lighting systems and implementations are planned but no promises.
## Prerequisites
- Assimp library
- Vulkan SDK
- GLFW library
### Note : meson.build files are left to make building the project easier.
### Also Assets folder exists with a cube mesh and the shader used in the pipeline in both GLSL and SPIR-V.
## Docs
The source code is commented with information about the functions which are kinda vague,the Vulkan specs is a create place to read from if you didn't know what a function is for.
## Find this software useful? support the developer !
### [Become a Patron !](https://www.patreon.com/RedDeadAlice)