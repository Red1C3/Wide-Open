#version 450
#extension GL_ARB_separate_shader_objects : enable
layout(location=0) in vec3 position_modelSpace;
layout(location=1) in vec3 normal_modelSpace;
layout(set=0,binding=0) uniform Block{
	mat4 VP;
	mat4 model;
	vec3 diffuse;
	float spec;
	vec3 ambient;
	vec3 light;
	vec3 view;
};
layout(location=0)out vec3 normal;
layout(location=1)out vec3 vertex_worldSpace;
void main(){
	gl_Position=VP*model*vec4(position_modelSpace,1);
	normal=vec3(model*vec4(normal_modelSpace,1));
	vertex_worldSpace=vec3(model*vec4(position_modelSpace,1));
}