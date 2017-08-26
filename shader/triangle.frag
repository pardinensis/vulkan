#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;


void main() {
	vec3 lightDir = normalize(vec3(0.3, 1.0, 0.5));
	vec3 lightAmbient = vec3(0.1, 0.1, 0.2);
	vec3 lightDiffuse = vec3(0.8, 0.8, 0.6);

	vec3 light = lightAmbient;
	float ndotl = dot(lightDir, fragNormal);
	if (ndotl > 0) {
		light += ndotl * lightDiffuse;
	}

	outColor = vec4(light, 1.0);
}