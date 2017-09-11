#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

// per vertex
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// per instance
layout(location = 3) in vec3 instanceOffset;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTexCoord;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	vec4 worldPos = ubo.model * vec4(inPosition, 1.0);
	vec4 instancePos = worldPos + vec4(instanceOffset, 0.0);
	vec4 viewPos = ubo.view * instancePos;
	vec4 projPos = ubo.proj * viewPos;

	gl_Position = projPos;
	fragNormal = inNormal;
	fragTexCoord = inTexCoord;
}
