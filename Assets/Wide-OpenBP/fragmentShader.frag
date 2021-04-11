#version 450
#extension GL_ARB_separate_shader_objects : enable
layout(set=0,binding=0) uniform Block{
	mat4 VP;
	mat4 model;
	vec3 diffuse;
	float spec;
	vec3 ambient;
	vec3 light;
	vec3 view;
};
layout(location=0) out vec4 color;
layout(location=0)in vec3 normal;
layout(location=1)in vec3 vertex_worldSpace;
void main(){
	vec3 lightDir=normalize(light-vertex_worldSpace);
	vec3 diffuseFin=diffuse;
	diffuseFin*=dot(normalize(normal),lightDir);
	vec3 viewVec=normalize(vertex_worldSpace-view);
	vec3 halfWay=normalize(lightDir-viewVec);
	float specFactor=pow(max(dot(halfWay,normalize(normal)),0.0),spec);
	color=vec4(diffuseFin,1)*(7.0/10.0)+vec4(ambient,1)*(1.0/10.0)+vec4(1.0f)*specFactor*(2.0/10.0);
	color=vec4(color.xyz,1);
}