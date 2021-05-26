#version 450
layout(location=0) out vec4 color;
layout(set=0,binding=1)uniform samplerCube cubeMap;
layout(location=0) in vec3 TexCoords;
void main(){
	color=texture(cubeMap,TexCoords);
}