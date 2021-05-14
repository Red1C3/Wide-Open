#version 450
layout(set=0,binding =0) uniform Block{
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
layout(set=0,binding=1)uniform sampler2D depthMap;
layout(location=0) out vec4 color;
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
void main(){
	vec3 normal_worldSpace=mat3(model)*vec3(normal);
	normal_worldSpace=normalize(normal_worldSpace);
	vec4 frag_lightSpace=lightVP*model*vec4(position,1);
	vec3 projCoords=frag_lightSpace.xyz/frag_lightSpace.w;
	vec3 specCoords=projCoords*0.5+0.5;
	float closestDepth=texture(depthMap,specCoords.xy).x;
	float currentDepth=projCoords.z;
	float shadow=currentDepth-0.0001>closestDepth?0.0:1.0;
	color=vec4(shadow,shadow,shadow,1.0);
	vec3 vertex_worldSpace=vec3(model*vec4(position,1));
	vec3 lightDir=normalize(light-vertex_worldSpace);
	vec3 diffuseFin=diffuse;
	diffuseFin*=dot(normalize(normal),lightDir);
	vec3 viewVec=normalize(view-vertex_worldSpace);
	vec3 halfWay=normalize(lightDir+viewVec);
	float specFactor=pow(max(dot(halfWay,normalize(normal)),0.0),spec);
	color=vec4(diffuseFin,1)*(6.8/10.0)*shadow+vec4(ambient,1)*(1.2/10.0)+vec4(1.0f)*specFactor*(2.0/10.0)*shadow;
	color=vec4(color.xyz,1);
}
