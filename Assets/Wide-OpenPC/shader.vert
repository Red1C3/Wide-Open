#version 450
layout(location=0) in vec3 pos;
layout(push_constant) uniform pushConstant{
	mat4 MVP;
};
void main(){
	gl_Position=MVP*vec4(pos,1);
}