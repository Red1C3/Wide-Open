#version 450
layout(location=0) in vec3 vertex_modelSpace;
layout(set=0,binding=0) uniform Block{
	mat4 MVP;
};
void main(){
	gl_Position=MVP*vec4(vertex_modelSpace,1);
}
