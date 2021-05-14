#version 450
layout(location=0) in vec3 position_modelSpace;
layout(location=1) in vec3 normal_modelSpace;
layout(set=0,binding=0)  uniform Block{
	mat4 MVP;
	mat4 model;
	mat4 lightVP;
	mat4 VP;
	vec3 diffuse;
	float spec;
	vec3 ambient;
	vec3 light;
	vec3 view;
};
layout(location=0) out vec3 position;
layout(location=1) out vec3 normal;
void main(){
	position=position_modelSpace;
	normal=normal_modelSpace;
	gl_Position=VP*model*vec4(position_modelSpace,1);
}
