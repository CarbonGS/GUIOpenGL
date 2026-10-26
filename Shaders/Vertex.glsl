#version 460 core

layout(location = 0) in vec3 positionIN;
layout(location = 1) in vec3 normalIN;
layout(location = 2) in vec2 uv;
layout(location = 3) in int inmaterialId;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 uvOut;
out vec3 fragPos;
out vec3 normal;

flat out int materialId;

void main() {
	uvOut = uv;
	materialId = inmaterialId;
	vec4 worldPos = modelMatrix * vec4(positionIN, 1.0);
	fragPos = worldPos.xyz;
	normal = normalize(mat3(transpose(inverse(modelMatrix))) * normalIN);

	gl_Position = projectionMatrix * viewMatrix * worldPos;

}