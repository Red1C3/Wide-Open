#version 450
layout(location=0) in vec3 vertex_modelSpace;
layout(set=0,binding=0) uniform Block{
	mat4 MVP;
};
layout(set=0,binding=1)uniform samplerCube cubeMap;
layout(location=0) out vec3 TexCoords;
void main(){
	TexCoords=vertex_modelSpace;
	mat4 m_MVP=mat4(
	vec4(-0.950664878,0.731822193,-0.578506112,-0.577350259),
	vec4(0.950664878,0.731822193,-0.578506112,-0.577350259),
	vec4(0,-1.46364439,-0.578506112,-0.577350259),
	vec4(0,0,5.00635481,5.19615221));
	gl_Position=MVP*vec4(vertex_modelSpace,1);
}
