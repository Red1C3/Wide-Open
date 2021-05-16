# Wide-Open
This project is far from being robust and lacks tons of optimizations,BUT it's good for Vulkan beginners to understand the concept of using the API,however,this project also introduces the Wide-Open Common API, which is an extendable API that makes it easier to write stuff in Vulkan once you understand how it works (and I hope you find it as easy as I do).
Other than the Common API,this project,so far,contains two models for rendering,Blinn-Phong,and Blinn-Phong with shadows, which is a very basic single shadow map based shadowing thingy,assets are available for shaders too.
## The Wide-Open Common API
Since there is many repeated coding in Vulkan, and I'll say it,THIS API WILL RESTRICT VULKAN USAGE, it will make a good example for stuff that you might want to set as fixed in your own Vulkan projects,however,with extendablity in mind, if you want to use it while learning Vulkan,I don't think it will be a bad idea,just keep on mind that it is NOT optimized so far
## Blinn-Phong With Shadows
This model extends The Common API, and it's not documented yet (it'll be soon enough),briefly, it uses two render passes, the first render pass renders the shadow map, a depth image that renders from the light perspective, and the second render pass that uses a basic framebuffer with a single color image, and a single depth image,the second render pass uses the image rendered in the first render pass to sample depth values from and decide which fragments are being shadowed, it also uses the Blinn-Phong system in the second render pass which is described below.
## Blinn-Phong
This subproject implements the Blinn-Phong lighting system into Vulkan API,it simply uses a single color attachment , and a depth attachment for depth testing,the shaders are simple and straightforward, the shaders recieve vertices position and normal data, and outputs a single color per fragment, no textures support at all,and there is only a single light and a single mesh (which I think it'll be easy to expand by a few changes).
Note: this model doesn't use the most of the Common API (it was made before it was refined, so basically,this project is more Pure-Vulkan than Common-extended project)
## Prerequisites
- Assimp library
- Vulkan SDK
- GLFW library
- GLM library
### Note : meson.build files are left to make building the project easier.
### Also Assets folder exists with a cube mesh and the shaders used in the pipelines in both GLSL and SPIR-V.
## Docs
The Blinn-Phong source code is commented with information about the functions which are kinda vague,the Vulkan specs is a great place to read from if you didn't know what a function is for.
The Wide-Open Common API docs can be found in /subprojects/Common/docs, Blinn-Phong With Shadows is commented with assuming that you've read the Wide-Open Common API docs, pure Vulkan is and will not be explained since it IS explained in the Vulkan specs.
## Find this software useful? support the developer !
### [Become a Patron !](https://www.patreon.com/RedDeadAlice)
### Made with love and human-sized kittens